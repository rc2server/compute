cmake_minimum_required (VERSION 2.6)
project (rcompute2-tests)

find_package(GTest REQUIRED)
include_directories (${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_SOURCE_DIR}
)

configure_file(test1.Rmd test1.Rmd COPYONLY)
configure_file(test1.Rnw test1.Rnw COPYONLY)
configure_file(test1.R test1.R COPYONLY)
configure_file(chapter.Rmd chapter.Rmd COPYONLY)
configure_file(dframe.R dframe.R COPYONLY)

SET(TESTS
	pgdbconnection
	inputbuffer
	dbfilesource
	rserver
	rsession
	variables
)

add_subdirectory(testlib)

FOREACH(test ${TESTS})
	add_executable(${test}-t ${test}-t.cpp)
	target_link_libraries(${test}-t testslib src event common R -lRInside ${Compute_Libs} ${Boost_LIBRARIES} ${GTEST_BOTH_LIBRARIES} pthread -lm)
	add_test(NAME ${test} COMMAND ${test}-t)
ENDFOREACH()
