cmake_minimum_required(VERSION 2.8)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

project(rcompute2)

enable_testing()

set(CMAKE_BUILD_TYPE Debug)

set(CPACK_GENERATOR TGZ)
set(CPACK_PACKAGE_FILE_NAME "rc2compute")
INCLUDE(CPack)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

#if("${rcompute2_SOURCE_DIR}" STREQUAL "${rcompute2_BINARY_DIR}")
#   message(SEND_ERROR "In-source builds are not allowed. (${rcompute2_SOURCE_DIR}) (${rcompute2_BINARY_DIR})")
#endif("${rcompute2_SOURCE_DIR}" STREQUAL "${rcompute2_BINARY_DIR}")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.54 REQUIRED COMPONENTS regex filesystem system)
include_directories(${Boost_INCLUDE_DIRS}) 

set_property(GLOBAL PROPERTY CXX_STANDARD 11)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++11)
SET(CMAKE_CXX_FLAGS "-std=c++11 -DBOOST_LOG_DYN_LINK")
#following fixes compile error because somehow -rdynamic is getting set as a linker flag
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-Wl,--no-export-dynamic -lstdc++")
SET(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib:/usr/local/lib/R/lib:/usr/local/lib/R/site-library/RInside/libs/:")
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

SET(Compute_Libs
	event
	event_pthreads
	uuid
	pq
	RInside
	g3logger
)

include(ExternalProject)

ExternalProject_Add (
	g3log
	GIT_REPOSITORY https://github.com/KjellKod/g3log.git
	BINARY_DIR ${rcompute2_BINARY_DIR}/g3loglib
	BUILD_IN_SOURCE 0
	INSTALL_COMMAND ""
	UPDATE_COMMAND ""
	BUILD_COMMAND make g3logger
	CMAKE_ARGS -DADD_FATAL_EXAMPLE=OFF g3logger 
)	
ExternalProject_Get_Property(g3log source_dir)
ExternalProject_Get_Property(g3log binary_dir)
set(G3LOG_INCLUDE_DIR "${source_dir}/src")
set(G3LOG_LIBRARY_DIR "${binary_dir}")
link_directories(${G3LOG_LIBRARY_DIR})

#R installed from sources uses /usr/local/lib/R/library while it uses /usr/local/lib/R/site-library if installed via rpm
include_directories (${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_ROOT}
	${G3LOG_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}/vendor
	/usr/include
	/usr/include/postgresql
	/usr/share/R/include
	/usr/local/lib/R/site-library/RInside/include
	/usr/local/lib/R/site-library/Rcpp/include
	/usr/local/lib/R/include)

link_directories (
	${CMAKE_CURRENT_SOURCE_DIR}/src
	/usr/local/lib/R/site-library/RInside/lib
	/usr/local/lib/R/library/RInside/libs
	/usr/local/lib/R/lib
)

add_subdirectory (common)
add_subdirectory (src)
add_subdirectory (tests)
add_library(uuid STATIC IMPORTED)
add_library(event STATIC IMPORTED)
add_library(event_pthreads STATIC IMPORTED)
add_library(boost_filesystem STATIC IMPORTED)
add_library(boost_system STATIC IMPORTED)
add_library(boost_regex STATIC IMPORTED)
add_library(RInside SHARED IMPORTED)
set_target_properties(event PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libevent.a)
set_target_properties(event_pthreads PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libevent_pthreads.a)
set_target_properties(boost_system PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libboost_system.a)
set_target_properties(boost_filesystem PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libboost_filesystem.a)
set_target_properties(boost_regex PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libboost_regex.a)
set_target_properties(uuid PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libuuid.a)
set_target_properties(RInside PROPERTIES IMPORTED_LOCATION /usr/local/lib/R/site-library/RInside/libs/RInside.so)

add_executable(rsession src/RSession-main.cpp)

add_executable(rserver src/RServer-main.cpp)

add_dependencies(rsession g3log)
#set_target_properties(rsession PROPERTIES INSTALL_RPATH "\$ORIGIN/lib")
#set_target_properties(rserver rsession PROPERTIES INSTALL_RPATH "$ORIGIN/../:$ORIGIN")

target_link_libraries(rserver src common ${Compute_Libs} ${Boost_LIBRARIES})
target_link_libraries(rsession src common pthread ${Compute_Libs} ${Boost_LIBRARIES} R RInside libg3logger.a -lm)
#target_compile_options(rsession PRIVATE RInside.so)

install(CODE "execute_process(COMMAND \"${CMAKE_SOURCE_DIR}/prepareRInside.sh\" \"${CMAKE_BINARY_DIR}\")")

install(TARGETS rserver rsession  RUNTIME DESTINATION .)
install(DIRECTORY ${CMAKE_BINARY_DIR}/RInside DESTINATION . ${INSTALL_PATH})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/R/rc2 DESTINATION . ${INSTALL_PATH})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/rsrc DESTINATION . ${INSTALL_PATH})
