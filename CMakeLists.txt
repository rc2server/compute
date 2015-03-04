cmake_minimum_required (VERSION 2.6)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

project (rcompute2)

set(CMAKE_BUILD_TYPE Debug)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

#if("${rcompute2_SOURCE_DIR}" STREQUAL "${rcompute2_BINARY_DIR}")
#   message(SEND_ERROR "In-source builds are not allowed. (${rcompute2_SOURCE_DIR}) (${rcompute2_BINARY_DIR})")
#endif("${rcompute2_SOURCE_DIR}" STREQUAL "${rcompute2_BINARY_DIR}")

set_property(GLOBAL PROPERTY CXX_STANDARD 11)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++11)

include_directories (${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_ROOT}
	/usr/local/lib/R/site-library/RInside/include
	/usr/local/lib/R/site-library/Rcpp/include
	/usr/share/R/include)

link_directories (
	/usr/local/lib/R/site-library/RInside/lib
)

add_subdirectory (common)
add_subdirectory (tests)

add_executable(rsession src/RSession.cpp 
						src/InputBufferManager.cpp
						src/RSessionCallbacks.cpp 
						src/RSession-main.cpp)

add_executable(rserver 	src/RServer.cpp 
						src/RServer-main.cpp)

target_link_libraries(rserver event common uuid boost_filesystem boost_system)
target_link_libraries(rsession common R event boost_filesystem RInside boost_system uuid)
