cmake_minimum_required (VERSION 2.6)
project (rcompute-src)

add_library (src InputBufferManager.cpp 
					EnvironmentWatcher.cpp
					RC2Logging.cpp
					FileManager.cpp
					DBFileSource.cpp
					RJsonEncoder.cpp
					PreviewData.cpp
					RServer.cpp 
					RSession.cpp 
					RSessionCallbacks.cpp )

#add_dependencies(src common, parser)
