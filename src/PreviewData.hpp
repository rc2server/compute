#pragma once

#include <string>
#include "common/ZeroInitializedStruct.hpp"

using std::string;

namespace RC2 {
	
	struct PreviewData: ZeroInitializedStruct {
		int				previewId;
		FileInfo		fileInfo;
		
		PreviewData(int pid, FileInfo* finfo)
		: previewId(pid), fileInfo(finfo)
		{}
	};	
	
}; // end namespace 

