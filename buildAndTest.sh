#!/bin/sh

mkdir build
cd build
cmake ../compute
rc=$?; if [ $rc != 0 ]; then exit $rc; fi
make
rc=$?; if [ $rc != 0 ]; then exit $rc; fi
make package
rc=$?; if [ $rc != 0 ]; then exit $rc; fi
make test CTEST_OUTPUT_ON_FAILURE=TRUE
rc=$?; if [ $rc != 0 ]; then exit $rc; fi
