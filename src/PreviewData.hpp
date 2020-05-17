#pragma once

#include <string>
#include <map>
#include <memory>
#include "common/ZeroInitializedStruct.hpp"
#include "EnvironmentWatcher.hpp"
#include "parser/RmdParser.hpp"
#include <RInside.h>

using std::string;

namespace RC2 {	
	// need to handle referenced files that changed (csv, etc.)
	struct ChunkCacheEntry: ZeroInitializedStruct {
		int			chunkNumber;
		size_t		lastHash;
		string		lastSource;
		string		lastOutput;
		size_t		lastOutputHash;
		
		ChunkCacheEntry(int num, string source, string output)
			: chunkNumber(num), lastSource(source), lastOutput(output), 
			lastHash(std::hash<string>()(source)), lastOutputHash(std::hash<string>()(output))
			{}
	};
	
	typedef std::map<int, ChunkCacheEntry> ChunkCacheMap;

	struct UpdateResponse: ZeroInitializedStruct {
		int				previewId;
		string			resultText;
		
		UpdateResponse(int preId) 
			: previewId(preId) {};
	};

	typedef std::function<void (string)>  SendJsonLambda;
	struct PreviewData: ZeroInitializedStruct {
		int				previewId;
		FileInfo		fileInfo;
		ChunkCacheMap	chunkMap;
		RmdParser		parser;
		
		PreviewData(int pid, FileInfo* finfo, RInside *rInside, EnvironmentWatcher* globalEnv, SendJsonLambda outputLamba)
		: previewId(pid), fileInfo(finfo), rinside(rInside), jsonOutput_(outputLamba), previewEnv(globalEnv)
		{}
		
		std::vector<Chunk*> currentChunks() const { return currentChunks_; }
		
		std::unique_ptr<UpdateResponse> update(FileInfo& updatedInfo, string& updateIdent, int targetChunkId, bool includePrevious = false);
	private:
		
		std::vector<Chunk*> whichChunksNeedUpdate(int start, bool includePrevious);
		void				executeCode(std::vector<Chunk*> chunksToUpdate, UpdateResponse* results);
		
		EnvironmentWatcher previewEnv;
		RInside*		rinside;
		string			currentUpdateIdentifier_;
		std::vector<Chunk*> currentChunks_;
		std::function<void (string)> jsonOutput_;
	};	
		
}; // end namespace 

