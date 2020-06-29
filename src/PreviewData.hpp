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
		int					chunkNumber;
		size_t				lastHash;
		string				lastSource;
		string				lastOutput;
		size_t				lastOutputHash;
		EnvironmentWatcher	envWatcher;
		
		ChunkCacheEntry(int num, string source, string output)
			: chunkNumber(num), lastSource(source), lastOutput(output), 
			lastHash(std::hash<string>()(source)), lastOutputHash(std::hash<string>()(output))
			{}
	};
	
	typedef std::map<int, ChunkCacheEntry> ChunkCacheMap;

	struct UpdateResponse: ZeroInitializedStruct {
		int				previewId;
		string			resultText;
		
		UpdateResponse(int preId, string results = "") 
			: previewId(preId), resultText(results) {};
	};

	typedef std::function<void (string)>  SendJsonLambda;
	struct PreviewData: ZeroInitializedStruct {
		int								previewId;
		FileManager*					fileManager;
		FileInfo						fileInfo;
		ChunkCacheMap					chunkMap;
		RmdParser						parser;
		boost::signals2::connection*	fileConnection;
		
		PreviewData(int pid, FileManager* fmanager, FileInfo& finfo, RInside *rInside, EnvironmentWatcher* globalEnv, SendJsonLambda outputLamba);
		virtual ~PreviewData();
		
		std::vector<Chunk*> currentChunks() const { return currentChunks_; }
		
		std::unique_ptr<UpdateResponse> update(FileInfo& updatedInfo, string& updateIdent, int targetChunkId, bool includePrevious = false);
		
		void fileChanged(long changedId, ChangeType type);
	protected:
		
		std::vector<Chunk*> whichChunksNeedUpdate(int start, bool includePrevious);
		void				executeCode(std::vector<Chunk*> chunksToUpdate, UpdateResponse* results);
		
		EnvironmentWatcher	previewEnv;
		RInside*			rinside;
		string				currentUpdateIdentifier_;
		std::vector<Chunk*>	currentChunks_;
		std::function<void (string)> jsonOutput_;
	};	
		
}; // end namespace 

