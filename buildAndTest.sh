#!/bin/sh

mkdir build
cd build
cmake ../compute
make test
