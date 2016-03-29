cmake_minimum_required (VERSION 2.6)
project (rcompute2-tests)

find_package(GTest REQUIRED)
include_directories (${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_SOURCE_DIR}
)

configure_file(test1.Rmd test1.Rmd COPYONLY)
configure_file(test1.Rnw test1.Rnw COPYONLY)
configure_file(test1.R test1.R COPYONLY)

SET(TESTS
	rserver
	inputbuffer
	dbfilesource
	rsession
)

FOREACH(test ${TESTS})
	add_executable(${test}-t ${test}-t.cpp)
	target_link_libraries(${test}-t src event common R -l:RInside.so ${Compute_Libs} ${GTEST_BOTH_LIBRARIES} pthread)
	add_test(${test} ${test}-t)
ENDFOREACH()
