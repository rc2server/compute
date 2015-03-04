cmake_minimum_required (VERSION 2.6)
project (rcompute2-tests)

find_package(GTest REQUIRED)
include_directories (${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_SOURCE_DIR}
)

add_executable(rserver-test RServerTests.cpp ../src/RServer.cpp)

target_link_libraries(rserver-test event common uuid boost_filesystem boost_system ${GTEST_BOTH_LIBRARIES} pthread)
