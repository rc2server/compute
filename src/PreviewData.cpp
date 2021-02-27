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
#include "vendor/cpp-base64/base64.h"

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

	RC2::PreviewData::PreviewData ( int pid, FileManager* fmanager, FileInfo& finfo, EnvironmentWatcher* globalEnv, PreviewDelegate *delegate )
	: previewId ( pid ), fileManager ( fmanager ), fileInfo ( finfo ), delegate_( delegate ), previewEnv ( globalEnv ) 
	{
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
	LOG_INFO << "got update request ident:" << updateIdent;
	fileInfo = updatedInfo;
	bool usePrevious = includePrevious;
	int targetId = targetChunkId;
	try {
		string contents = SlurpFile ( fileInfo.name.c_str() );
		currentChunks_ = parser.parseRmdSource ( contents );
		// if target is less than zero, execute all chunks
		if ( targetId < 0) {
			targetId = currentChunks_.size() - 1;
			usePrevious = true;
		}
		auto chunks2Update = whichChunksNeedUpdate ( targetId, usePrevious );
		// send update started message
		nlohmann::json results;
		results["msg"] = "previewUpdateStarted";
		results["updateIdentifier"] = currentUpdateIdentifier_;
		results["activeChunks"] = chunks2Update;
		results["previewId"] = previewId;
		delegate_->sendPreviewJson(results.dump());
		// start execution
		executeChunks ( chunks2Update );
		currentUpdateIdentifier_ = "";
	} catch (std::exception &error) {
		LOG_INFO << "error parsing for update: " << error.what();
	}
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
	LOG_INFO << "executing " << chunksToUpdate.size() << " chunks";
	// TODO: handle exceptions
	for ( auto idx: chunksToUpdate ) {
		Chunk* aChunk = currentChunks_[idx];
		if ( aChunk->type() == markdown ) continue; // TODO: handle inline code chunks
		auto cacheEntry = chunkMap[idx].get();
		auto oldCrc = cacheEntry->crc;
		try {
			LOG_INFO << "executing " << aChunk->content() << std::endl;
			executeChunk ( aChunk, cacheEntry );
			nlohmann::json results;
			results["msg"] = "previewUpdated";
			results["chunkId"] = idx;
			results["updateIdentifier"] = currentUpdateIdentifier_;
			results["previewId"] = previewId;
			results["content"] = cacheEntry->lastOutput;
			results["complete"] = false;
			string str = results.dump(2);
			delegate_->sendPreviewJson(str);
		} catch ( GenericException& e ) {
			LOG_INFO << "generic exception: " << e.code();
		}
	}
	json finalResults;
	finalResults["chunkId"] = -1;
	finalResults["msg"] = "previewUpdated";
	finalResults["updateIdentifier"] = currentUpdateIdentifier_;
	finalResults["previewId"] = previewId;
	finalResults["complete"] = true;
	finalResults["content"] = "";
	delegate_->sendPreviewJson(finalResults.dump(2));
}

string
RC2::PreviewData::executeChunk ( Chunk* chunk, ChunkCacheEntry* cacheEntry ) {
	SEXP answer;
	Rcpp::Environment env;
	// TODO: use environment from cacheEntry
	previewEnv.captureEnvironment();
	previewEnv.getEnvironment ( env );
	string userCode = chunk->innerContent();
	boost::algorithm::trim_if ( userCode, &isSpaceOrTab );
	env.assign ( "rc2.code", userCode );
	env.assign ( "rc2.env", env );
	// watch for files
	fileManager->resetWatch();
	string code = "rc2.evaluateCode(rc2.code, parent = rc2.env)";
	try {
		delegate_->executePreviewCode(code, answer, &env);
	} catch (const RException& e) {
		LOG_INFO << "preview query failed:" << e.getCode();
		return ""; // FIXME: should get error message from sexp, use that as what()

	} catch (const std::exception& e) {
		LOG_WARNING << "error executing code";
	}
	env.remove("rc2.code");
	env.remove("rc2.env");
	env.remove("allItems"); // should have been removed by R package. wasn't for some reason
	if ( (answer == nullptr) || Rf_isVector(answer) == FALSE) {
		Rcpp::StringVector emsg = Rcpp::as<Rcpp::StringVector>(answer);
		LOG_INFO << "type=" << TYPEOF(answer) << " error; " << emsg[0];
		throw GenericException ( "query failed", kError_QueryFailed );
	}
	std::stringstream outStr;
	Rcpp::List results ( answer );
	for ( auto itr = results.begin(); itr != results.end(); ++itr ) {
		Rcpp::List curList ( *itr );
		string elType = curList.attr ( "class" );
		if ( elType == "rc2src" ) {
			Rcpp::StringVector strs ( Rcpp::as<Rcpp::StringVector> ( curList["src"] ) );
			outStr << "<div style=\"rc2-preview-src\">" << strs[0] << "</div>" << endl;
		} else if ( elType == "rc2value" ) {
			Rcpp::StringVector strs ( Rcpp::as<Rcpp::StringVector> ( curList["str"] ) );
			outStr << "<div style=\"rc2-preview-out\">" << strs[0] << "</div>" << endl;
		} else if ( elType == "rc2err" ) {
			Rcpp::StringVector strs ( Rcpp::as<Rcpp::StringVector> ( curList["message"] ) );
			Rcpp::StringVector calls ( Rcpp::as<Rcpp::StringVector> ( curList["call"] ) );
			outStr << "<div style=\"rc2-error\">" << strs[0] << "(location: " << calls[0] << ")</div>" << endl;
		} else if ( elType == "rc2plot") {
			BinaryData imgData;
			Rcpp::StringVector names ( Rcpp::as<Rcpp::StringVector> ( curList["fname"] ) );
			string fname(names[0]);
			SlurpBinaryFile(fname, imgData);
			string encodedStr = base64_encode((unsigned char*)imgData.data.get(), imgData.getSize(), false);
			outStr << "<div style=\"rc2-plot\"><img src=\"data:image/png;base64," << encodedStr << "\"></div>" << endl;
		} else {
			LOG_INFO << "unsupported ype returned from evaluate: " << elType;
		}
	}
	auto output = outStr.str();
	cacheEntry->cacheOutput(output);
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
RC2::PreviewData::whichChunksNeedUpdate ( int targetChunkId, bool includePrevious ) {
	// TODO: implement more intelligent checking
	vector<int> toExecute;
	int startIndex = targetChunkId;
	// if the number of chunks changed, we'll invalidate everything for now
	// TODO: use the ChunkCacheEntry(s) to determine if the cache can be used
	if ( currentChunks_.size() != chunkMap.size() ) {
		chunkMap.clear();
		// insert new cache entries
		checkCache();
	}
	if ( includePrevious ) {
		startIndex = 0;
	} else {
		// adjust targetId if any previous chunks need to be executed
		for ( int i = startIndex - 1; i >= 0; --i ) {
			// TODO: to support inline chunks, need to see if mdown chunk has inline code and if the cache of it is empty
			auto centry = chunkMap[i].get();
			auto chunk = currentChunks_[i];
			if ( chunk->type() == code && !centry->outputCached ) {
				startIndex = i;
			}
		}
	}
	for ( int i=startIndex; i <= targetChunkId; ++i ) {
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
