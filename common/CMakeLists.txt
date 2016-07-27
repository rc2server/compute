cmake_minimum_required (VERSION 2.6)
project (rcompute-common)

add_library (common FormattedException.cpp RC2Utils.cpp PGDBConnection.cpp)
