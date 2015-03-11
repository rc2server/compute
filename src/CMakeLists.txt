cmake_minimum_required (VERSION 2.6)
project (rcompute-src)

add_library (src InputBufferManager.cpp 
					RServer.cpp 
					RSession.cpp 
					RSessionCallbacks.cpp )
