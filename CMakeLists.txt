cmake_minimum_required (VERSION 2.6)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

project (rcompute2)

enable_testing()

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
SET(CMAKE_CXX_FLAGS "-std=c++11 -DBOOST_LOG_DYN_LINK")
#following fixes compile error because somehow -rdynamic is getting set as a linker flag
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-Wl,--no-export-dynamic -lstdc++")

SET(Compute_Libs
	event
	boost_filesystem
	boost_regex
	boost_system
	uuid
	pq
	glog
)

include(ExternalProject)
ExternalProject_Add (
	cppnetlib 
	URL http://storage.googleapis.com/cpp-netlib-downloads/0.11.1/cpp-netlib-0.11.1-final.tar.bz2
	BINARY_DIR ${rcompute2_BINARY_DIR}/cppnetlib
	BUILD_IN_SOURCE 0
	CMAKE_ARGS -DCPP-NETLIB_BUILD_TESTS:BOOL=OFF -DCPP-NETLIB_BUILD_EXAMPLES:BOOL=OFF
	CONFIGURE_COMMAND
	INSTALL_COMMAND ""
	UPDATE_COMMAND ""
)
ExternalProject_Get_Property(cppnetlib source_dir)
ExternalProject_Get_Property(cppnetlib binary_dir)
set(CPP-NETLIB_INCLUDE_DIR "${source_dir}")
set(CPP-NETLIB_LIBRARY_DIR "${binary_dir}/libs/network/src")
set(CPP-NETLIB_LIBRARIES "cppnetlib-uri" "cppnetlib-client-connections")
link_directories(${CPP-NETLIB_LIBRARY_DIR})

#R installed from sources uses /usr/local/lib/R/library while it uses /usr/local/lib/R/site-library if installed via rpm
include_directories (${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_ROOT}
	${CPPNET-LIB_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}/vendor
	/usr/include
	/usr/include/postgresql
	/usr/local/lib/R/library/RInside/include
	/usr/local/lib/R/library/Rcpp/include
	/usr/local/lib/R/site-library/RInside/include
	/usr/local/lib/R/site-library/Rcpp/include
	/usr/local/lib/R/include)

link_directories (
	/usr/local/lib/R/site-library/RInside/lib
	/usr/local/lib/R/library/RInside/libs
	/usr/local/lib/R/lib
)

add_subdirectory (common)
add_subdirectory (src)
add_subdirectory (tests)

add_executable(rsession src/RSession-main.cpp)

add_executable(rserver src/RServer-main.cpp)

target_link_libraries(rserver src common ${Compute_Libs})
target_link_libraries(rsession src common pthread ${Compute_Libs} R -l:RInside.so ${NETLIB_LIBRARIES} -lm)
