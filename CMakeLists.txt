cmake_minimum_required (VERSION 2.6)
project (rcompute2)

set_property(GLOBAL PROPERTY CXX_STANDARD 11)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++11)

include_directories (${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_ROOT}
	/usr/local/lib/R/site-library/RInside/include
	/usr/local/lib/R/site-library/Rcpp/include
	/usr/share/R/include)

#link_directories ()

add_subdirectory (common)

add_executable(rsession src/rsession.cpp src/RSessionCallbacks.cpp)
add_executable(rserver src/RServer.cpp)

target_link_libraries(rserver event common uuid boost_filesystem boost_system)
target_link_libraries(rsession common R)
