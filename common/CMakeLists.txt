cmake_minimum_required (VERSION 2.6)
project (rcompute-common)
set_property(GLOBAL PROPERTY CXX_STANDARD 14)

add_library (common FormattedException.cpp RC2Utils.cpp PGDBConnection.cpp ../vendor/cpp-base64/base64.cpp)

#add_dependencies(common g3log)
