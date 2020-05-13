#pragma once

#include <string>
#include <map>
#include "common/ZeroInitializedStruct.hpp"
#include "parser/RmdParser.hpp"

using std::string;

namespace RC2 {	
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

	struct PreviewData: ZeroInitializedStruct {
		int				previewId;
		FileInfo		fileInfo;
		ChunkCacheMap	chunkMap;
		RmdParser		parser;
		
		PreviewData(int pid, FileInfo* finfo)
		: previewId(pid), fileInfo(finfo)
		{}
		
		std::vector<Chunk*> currentChunks() const { return currentChunks_; }
		
		void update(FileInfo& updatedInfo, int targetChunkId, bool includePrevious = false);
	private:
		std::vector<Chunk*> currentChunks_;
	};	
	
}; // end namespace 

