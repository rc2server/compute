cmake_minimum_required (VERSION 2.6)
project (rcompute2)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (common)

add_executable(rsession src/rsession.cpp)
add_executable(rserver src/rserver.cpp)

target_link_libraries(rsession common)
