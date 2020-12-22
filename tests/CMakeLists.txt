cmake_minimum_required (VERSION 2.6)
project (rcompute2-tests)

#find_package(GTest REQUIRED)
#include_directories (${CMAKE_CURRENT_SOURCE_DIR} 
#	${CMAKE_SOURCE_DIR}
#)

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

configure_file(test1.Rmd test1.Rmd COPYONLY)
configure_file(preview1.Rmd preview1.Rmd COPYONLY)
configure_file(previewCacheInvalidate.Rmd COPYONLY)
configure_file(test1.Rnw test1.Rnw COPYONLY)
configure_file(test1.R test1.R COPYONLY)
configure_file(chapter.Rmd chapter.Rmd COPYONLY)
configure_file(dframe.R dframe.R COPYONLY)

include_directories("${gtest_SOURCE_DIR}/include")

SET(TESTS
	pgdbconnection
	inputbuffer
	dbfilesource
	rserver
	rsession
	variables
	environ
	evaluate
	parser
	preview
)

add_subdirectory(testlib)

FOREACH(test ${TESTS})
	add_executable(${test}-t ${test}-t.cpp)
	target_link_libraries(${test}-t testslib src event common parser ${ANTLR_RTIME} R -lRInside ${Compute_Libs} ${Boost_LIBRARIES} ${VALIDATE_LIBRARY} gtest_main pthread -lm)
	add_test(NAME ${test} COMMAND ${test}-t)
ENDFOREACH()
