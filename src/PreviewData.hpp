#pragma once

#include <string>
#include <map>
#include <memory>
#include "common/ZeroInitializedStruct.hpp"
#include "EnvironmentWatcher.hpp"
#include "parser/RmdParser.hpp"

using std::string;
using std::vector;
using std::unique_ptr;

namespace RC2 {	
	// need to handle referenced files that changed (csv, etc.)
	struct ChunkCacheEntry: ZeroInitializedStruct {
		int						chunkNumber;
		size_t					lastHash;
		string					lastSource;
		string					lastOutput;
		size_t					lastOutputHash;
		EnvironmentWatcher		envWatcher;
		
		ChunkCacheEntry(int num, string source, string output, EnvironmentWatcher *parentEnv)
			: chunkNumber(num), lastSource(source), lastOutput(output), 
			lastHash(std::hash<string>()(source)), lastOutputHash(std::hash<string>()(output)),
			envWatcher(parentEnv)
			{}
	};
	
	typedef std::map<int, ChunkCacheEntry> ChunkCacheMap;

	struct UpdateResponse: ZeroInitializedStruct {
		int				previewId;
		string			resultText;
		
		UpdateResponse(int preId, string results = "") 
			: previewId(preId), resultText(results) {};
	};

	typedef boost::function<void (string)>  SendJsonLambda;
	typedef boost::function<void (string, SEXP&, Rcpp::Environment*)> ExecuteCodeLambda;
	
	struct PreviewData: ZeroInitializedStruct {
		int								previewId;
		FileManager*					fileManager;
		FileInfo						fileInfo;
		ChunkCacheMap					chunkMap;
		RmdParser						parser;
		boost::signals2::connection*	fileConnection;
		
		PreviewData(int pid, FileManager* fmanager, FileInfo& finfo, EnvironmentWatcher* globalEnv, SendJsonLambda outputLambda, 	
					ExecuteCodeLambda execLambda);
		virtual ~PreviewData();
		
		vector<Chunk*> currentChunks() const { return currentChunks_; }
		
		unique_ptr<UpdateResponse> update(FileInfo& updatedInfo, string& updateIdent, int targetChunkId, bool includePrevious = false);
		
		void fileChanged(long changedId, ChangeType type);
	protected:
		
		vector<Chunk*> 		whichChunksNeedUpdate(int start, bool includePrevious);
		void				executeChunks(vector<Chunk*> chunksToUpdate, UpdateResponse* results);
		
		EnvironmentWatcher	previewEnv;
		string				currentUpdateIdentifier_;
		vector<Chunk*>		currentChunks_;
		SendJsonLambda 		jsonOutput_;
		ExecuteCodeLambda	execCode_;
	};	
		
}; // end namespace 

