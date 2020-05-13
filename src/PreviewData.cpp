#include "FileManager.hpp"
#include "parser/RmdParser.hpp"
#include "PreviewData.hpp"
#include "common/RC2Utils.hpp"
#include <cassert>

void 
RC2::PreviewData::update(FileInfo& updatedInfo, int targetChunkId, bool includePrevious) {
	assert(updatedInfo.id == fileInfo.id);
	if (updatedInfo.version <= fileInfo.version) return;
	fileInfo = updatedInfo;
	string contents = SlurpFile(fileInfo.name.c_str());
	currentChunks_ = parser.parseRmdSource(contents);
	
}

