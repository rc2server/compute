cmake_minimum_required (VERSION 2.6)
project (rcompute-src)

add_library (src InputBufferManager.cpp 
					InotifyFileWatcher.cpp
					EnvironmentWatcher.cpp
					FileManager.cpp
					DBFileSource.cpp
					RServer.cpp 
					RSession.cpp 
					RSessionCallbacks.cpp )
