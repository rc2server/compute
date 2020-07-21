#include "FileManager.hpp"
#include "parser/RmdParser.hpp"
#include "PreviewData.hpp"
#include "common/RC2Utils.hpp"
#include "common/FormattedException.hpp"
#include <cassert>
#include <algorithm>
#include <RInside.h>
#include <boost/algorithm/string/trim.hpp>
#include <boost/bind.hpp>
#include <sstream>
#include "Errors.hpp"
#include "RC2Logging.h"
#include <nlohmann/json.hpp>

/*
 * 1. if the source file hasn't changed, nothing to do. Note if data files have changed, need to update any chunks that refer to them
 * 2. reparse source file
 * 3. figure out which chunks need updating
 * 4. execute the code
 * 5. create cache entry
 * 6. return results for chunk
 */

using std::unique_ptr;
using std::endl;
using json = nlohmann::json;

//void handleFileChange(PreviewData *data,

bool isSpaceOrTab ( char c ) {
	return c == 32 || c == 9;
}

namespace RC2 {

RC2::PreviewData::PreviewData ( int pid, FileManager* fmanager, FileInfo& finfo, EnvironmentWatcher* globalEnv, SendJsonLambda outputLambda, ExecuteCodeLambda execLambda )
	: previewId ( pid ), fileManager ( fmanager ), fileInfo ( finfo ), jsonOutput_ ( outputLambda ), previewEnv ( globalEnv ), execCode_ ( execLambda ) {
	long fid = fileInfo.id;
	fileManager->addChangeListener ( fid, boost::bind ( &PreviewData::fileChanged, this, fid, ALL ), &fileConnection );
}

RC2::PreviewData::~PreviewData() {
	// FIXME: is this a memory leak?
//	fileConnection->disconnect();
}

void
RC2::PreviewData::update ( FileInfo& updatedInfo, string& updateIdent, int targetChunkId, bool includePrevious ) {
	assert ( updatedInfo.id == fileInfo.id );
	currentUpdateIdentifier_ = updateIdent;
	if ( updatedInfo.version <= fileInfo.version ) return;
	fileInfo = updatedInfo;
	string contents = SlurpFile ( fileInfo.name.c_str() );
	currentChunks_ = parser.parseRmdSource ( contents );

	auto chunks2Update = whichChunksNeedUpdate ( targetChunkId, includePrevious );
	executeChunks ( chunks2Update );
	currentUpdateIdentifier_ = "";
}

void
RC2::PreviewData::fileChanged ( long changedId, ChangeType type ) {
	LOG_INFO << "fileChanged called:" << changedId << endl;
}

void
RC2::PreviewData::executeChunks ( vector<int> chunksToUpdate ) {
	// 1. create cache entry-
	// 2. evaluate
	// 3. turn results into json
	// 4. send results
	(jsonOutput_)("{}");
	LOG_INFO << "executing " << chunksToUpdate.size() << " chunks";
	// TODO: handle exceptions
	for ( auto idx: chunksToUpdate ) {
		Chunk* aChunk = currentChunks_[idx];
		if ( aChunk->type() == markdown ) continue; // TODO: handle inline code chunks
		auto cacheEntry = chunkMap[idx].get();
		auto oldCrc = cacheEntry->crc;
		try {
			executeChunk ( aChunk, cacheEntry );
			if ( oldCrc == cacheEntry->crc ) continue;
			json results;
			results["message"] = "previewUpdate";
			results["updateIdentifier"] = currentUpdateIdentifier_;
			results["previewId"] = previewId;
			results["content"] = cacheEntry->lastOutput;
			string str = results.dump(2);
			jsonOutput_ ( str );
		} catch ( GenericException& e ) {
			LOG_INFO << "generic exception: " << e.code();
		}
	}
}

string
RC2::PreviewData::executeChunk ( Chunk* chunk, ChunkCacheEntry* cacheEntry ) {
	SEXP answer;
	Rcpp::Environment env;
	// TODO: use environment from cacheEntry
	previewEnv.getEnvironment ( env );
	string userCode = chunk->innerContent();
	boost::algorithm::trim_if ( userCode, &isSpaceOrTab );
	env.assign ( "rc2.code", userCode );
	string code = "rc2.evaluateCode(rc2.code, parent = environment())";
	try {
		execCode_ ( code, answer, &env );
	} catch (const RException& e) {
		LOG_INFO << "preview query failed";
		return ""; // FIXME: should get error message from sexp, use that as what()

	} catch (const std::exception& e) {
		LOG_WARNING << "error executing code";
	}
	env.remove ( "rc2.code" );
	if ( (answer == nullptr) || Rf_isVector(answer) == FALSE) {
		Rcpp::StringVector emsg = Rcpp::as<Rcpp::StringVector>(answer);
		LOG_INFO << "type=" << TYPEOF(answer) << " error; " << emsg[0];
		throw GenericException ( "query failed", kError_QueryFailed );
	}
	std::stringstream outStr;
	Rcpp::List results ( answer );
	LOG_INFO << "sexp=" << TYPEOF(answer);
	for ( auto itr = results.begin(); itr != results.end(); ++itr ) {
		LOG_INFO << "results examined";
		Rcpp::List curList ( *itr );
		string elType = curList.attr ( "class" );
		LOG_INFO << " type = " << elType;
		if ( elType == "rc2src" ) {
			Rcpp::StringVector strs ( Rcpp::as<Rcpp::StringVector> ( curList["src"] ) );
			outStr << "<block style=\"rc2-preview-src\">" << strs[0] << "</block>" << endl;
		} else if ( elType == "rc2value" ) {
			Rcpp::StringVector strs ( Rcpp::as<Rcpp::StringVector> ( curList["str"] ) );
			outStr << "<block style=\"rc2-preview-out\">" << strs[0] << "</block>" << endl;
		} else if ( elType == "rc2err" ) {
			Rcpp::StringVector strs ( Rcpp::as<Rcpp::StringVector> ( curList["message"] ) );
			Rcpp::StringVector calls ( Rcpp::as<Rcpp::StringVector> ( curList["call"] ) );
			outStr << "<block style=\"rc2-error\">" << strs[0] << "(location: " << calls[0] << ")";
		} else {
			LOG_INFO << "unsupported ype returned from evaluate: " << elType;
		}
	}
	auto output = outStr.str();
	cacheEntry->lastOutput = output;
	cacheEntry->crc = cacheEntry->generateCRC();
	return output;
}

void
RC2::PreviewData::checkCache() {
	EnvironmentWatcher* parent = &previewEnv;
	for ( int i=0; i < currentChunks().size(); ++i ) {
		if ( chunkMap.count ( i ) > 0 ) continue;
		chunkMap[i] = std::make_unique<ChunkCacheEntry> ( i, parent );
		parent = &chunkMap[i]->envWatcher;
	}
}

vector<int>
RC2::PreviewData::whichChunksNeedUpdate ( int start, bool includePrevious ) {
	// TODO: implement more intelligent checking
	vector<int> toExecute;
	int targetId = start;
	// if the number of chunks changed, we'll invalidate everything for now
	// TODO: use the ChunkCacheEntry(s) to determine if the cache can be used
	if ( currentChunks_.size() != chunkMap.size() ) {
		chunkMap.clear();
		// insert new cache entries
		checkCache();
	}
	if ( includePrevious ) {
		targetId = 0;
	} else {
		// adjust targetId if any previous chunks need to be executed
		for ( int i = targetId; i >= 0; --i ) {
			auto centry = chunkMap[i].get();
			if ( centry->lastSource.size() == 0 || ( currentChunks_[i]->type() == code && centry->lastOutput.size() == 0 ) ) {
				targetId = i;
			}
		}
	}
	for ( int i=targetId; i < currentChunks_.size(); ++i ) {
		auto chunk = currentChunks_[i];
		// include all markdowns, will ignore ones with no inline code chunks
		if ( chunk->type() == code ) {
			toExecute.push_back ( i );
		} else if ( chunk->type() == markdown ) {
			auto mchunk = dynamic_cast<MarkdownChunk*> ( chunk );
			if ( mchunk == nullptr || mchunk->inlineChunkCount() < 1 ) { continue; }
			toExecute.push_back ( i );
		}
	}
	return toExecute;
}
}; //end namespace
