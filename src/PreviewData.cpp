#include "FileManager.hpp"
#include "parser/RmdParser.hpp"
#include "PreviewData.hpp"
#include "common/RC2Utils.hpp"
#include <cassert>
#include <algorithm>
#include <RInside.h>
#include <boost/bind.hpp>
//#include "RC2Logging.h"

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

//void handleFileChange(PreviewData *data, 

namespace RC2 {

RC2::PreviewData::PreviewData(int pid, FileManager* fmanager, FileInfo& finfo, RInside *rInside, EnvironmentWatcher* globalEnv, SendJsonLambda outputLamba)
	: previewId(pid), fileManager(fmanager), fileInfo(finfo), rinside(rInside), jsonOutput_(outputLamba), previewEnv(globalEnv)
{
	long fid = fileInfo.id;
	fileManager->addChangeListener(fid, boost::bind(&PreviewData::fileChanged, this, fid, ALL), &fileConnection);
}

RC2::PreviewData::~PreviewData() {
	fileConnection->disconnect();
}
	
unique_ptr<UpdateResponse> 
RC2::PreviewData::update(FileInfo& updatedInfo, string& updateIdent, int targetChunkId, bool includePrevious) {
	assert(updatedInfo.id == fileInfo.id);
	currentUpdateIdentifier_ = updateIdent;
	auto result = std::make_unique<UpdateResponse>(previewId);
	if (updatedInfo.version <= fileInfo.version) return unique_ptr<UpdateResponse>();
	fileInfo = updatedInfo;
	string contents = SlurpFile(fileInfo.name.c_str());
	currentChunks_ = parser.parseRmdSource(contents);
	
	auto chunks2Update = whichChunksNeedUpdate(targetChunkId, includePrevious);
	executeChunks(chunks2Update, result.get());
	currentUpdateIdentifier_ = "";
	return result;
}

void
RC2::PreviewData::fileChanged(long changedId, ChangeType type) {
//	LOG_INFO << "fileChanged called:" << changedId << endl;
}

void
RC2::PreviewData::executeChunks(vector<Chunk*> chunksToUpdate, UpdateResponse* results) {
	// 1. create cache entry-
	// 2. evaluate
	// 3. turn results into json
	// 4. send results
}
 
vector<Chunk*>
RC2::PreviewData::whichChunksNeedUpdate(int start, bool includePrevious) {
	// TODO: implement more intelligent checking
	vector<Chunk*> toExecute;
	int targetId = start;
	// if the number of chunks changed, we'll invalidate everything for now
	// TODO: use the ChunkCacheEntry(s) to determine if the cache can be used
	if (currentChunks_.size() != chunkMap.size())
		chunkMap.clear();
	if (includePrevious) { targetId = 0; }
	for(int i=targetId; i < currentChunks_.size(); ++i) {
		auto chunk = currentChunks_[i];
		// include all markdowns, will ignore ones with no inline code chunks
		if (chunk->type() == code) {
				toExecute.push_back(chunk);
		} else if (chunk->type() == markdown) {
			auto mchunk = dynamic_cast<MarkdownChunk*>(chunk);
			if (mchunk == nullptr || mchunk->inlineChunkCount() < 1) { continue; }
			toExecute.push_back(chunk);
		}
	}
	return toExecute;
}
}; //end namespace
