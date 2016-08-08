#!/bin/sh

mkdir build
cd build
cmake ../compute
make
make package
make test
