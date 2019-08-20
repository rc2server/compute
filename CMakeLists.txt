cmake_minimum_required(VERSION 3.0)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
#set(CMAKE_DISABLE_SOURCE_CHANGES ON)

project(Rc2Compute LANGUAGES CXX)

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

set_property(GLOBAL PROPERTY CXX_STANDARD 14)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++14)
SET(CMAKE_CXX_FLAGS "-DBOOST_LOG_DYN_LINK -g")
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

# build GTest
configure_file(vendor/GTest.CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
	${CMAKE_CURRENT_BINARY_DIR}/googletest-build
	EXCLUDE_FROM_ALL)

# build json-schema-validator
include(ExternalProject)
SET(VALIDATE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/json-schema-validator/")
SET(VALIDATE_BINARY_DIR "${CMAKE_BINARY_DIR}/vendor")
ExternalProject_Add(jsonschema DOWNLOAD_COMMAND "" 
	SOURCE_DIR "${VALIDATE_SOURCE_DIR}"
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/vendor #
	BINARY_DIR "${VALIDATE_BINARY_DIR}"
	INSTALL_COMMAND ""
	CMAKE_ARGS "-DNLOHMANN_JSON_DIR=${CMAKE_CURRENT_SOURCE_DIR}/vendor/"
#	BUID_ALWAYS FALSE
)

#R installed from sources uses /usr/local/lib/R/library while it uses /usr/local/lib/R/site-library if installed via rpm
include_directories (${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_ROOT}
	${CMAKE_SOURCE_DIR}/vendor
	${VALIDATE_SOURCE_DIR}/src/
	/usr/include
	/usr/include/postgresql
	/usr/share/R/include
	/usr/local/lib/R/site-library/RInside/include
	/usr/local/lib/R/site-library/Rcpp/include
	/usr/local/lib/R/include)

link_directories (
#	${VALIDATE_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/src
	/usr/local/lib/R/site-library/RInside/lib
	/usr/local/lib/R/library/RInside/libs
	/usr/local/lib/R/lib
	/usr/local/lib
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
add_library(json-validate STATIC IMPORTED)
set_target_properties(json-validate PROPERTIES IMPORTED_LOCATION "${VALIDATE_BINARY_DIR}/libjson-schema-validator.a")
set_target_properties(event PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libevent.a)
set_target_properties(event_pthreads PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libevent_pthreads.a)
set_target_properties(boost_system PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libboost_system.a)
set_target_properties(boost_filesystem PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libboost_filesystem.a)
set_target_properties(boost_regex PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libboost_regex.a)
set_target_properties(uuid PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libuuid.a)
set_target_properties(RInside PROPERTIES IMPORTED_LOCATION /usr/local/lib/R/site-library/RInside/libs/RInside.so)

add_executable(rsession src/RSession-main.cpp)

add_executable(rserver src/RServer-main.cpp)

#add_dependencies(rsession g3log)
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
