#pragma once

#include <string>
#include <map>
#include <memory>
#include <sstream>
#include "common/ZeroInitializedStruct.hpp"
#include "EnvironmentWatcher.hpp"
#include "parser/RmdParser.hpp"
#include "common/RC2Utils.hpp"

using std::string;
using std::vector;
using std::unique_ptr;

namespace RC2 {	
	// need to handle referenced files that changed (csv, etc.)
	struct ChunkCacheEntry: ZeroInitializedStruct {
		int						chunkNumber;
		int						crc;
		string					lastSource;
		string					lastOutput;
		EnvironmentWatcher		envWatcher;
		
		ChunkCacheEntry(int num, string source, string output, EnvironmentWatcher *parentEnv)
		: chunkNumber(num), lastSource(source), lastOutput(output), 
			envWatcher(parentEnv)
			{}
		
		ChunkCacheEntry(int num, EnvironmentWatcher *parentEnv)
			: chunkNumber(num), envWatcher(parentEnv)
			{}
		
		int generateCRC() {
			string csrc = lastSource + lastOutput + std::to_string(chunkNumber);
			return calculateCRCChecksum(csrc);
		}
	};
	
	typedef std::map<int, unique_ptr<ChunkCacheEntry>> ChunkCacheMap;

	struct UpdateResponse: ZeroInitializedStruct {
		int				previewId;
		int				chunkId;
		string			resultText;
		
		UpdateResponse(int preId, int chunkId, string results = "") 
			: previewId(preId), chunkId(chunkId), resultText(results) {};
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
		
		void update(FileInfo& updatedInfo, string& updateIdent, int targetChunkId, bool includePrevious = false);
		
		void fileChanged(long changedId, ChangeType type);
	protected:
		
		vector<int> 		whichChunksNeedUpdate(int start, bool includePrevious);
		void				executeChunks(vector<int> chunksToUpdate);
		void				checkCache();
		
		
		/**
		 * @brief Executes the chunk's code
		 * @param chunk the chunk to executeChunk
		 * @param cacheEntry the cache entry for this chunk
		 * @return the console output for the chunk
		 * @throws GenericException kError_QueryFailed
		 * 
		 */
		string				executeChunk(Chunk* chunk, ChunkCacheEntry* cacheEntry);
		
		EnvironmentWatcher	previewEnv;
		string				currentUpdateIdentifier_;
		vector<Chunk*>		currentChunks_;
		SendJsonLambda 		jsonOutput_;
		ExecuteCodeLambda	execCode_;
	};	
		
}; // end namespace 

