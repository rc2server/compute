#!/bin/sh

psql -U postgres -c 'create user rc2'
psql -U postgres -c 'create database rc2unittest owner rc2'
psql -U postgres -U rc2 rcunittest -c 'CREATE EXTENSION IF NOT EXISTS pgcrypto WITH SCHEMA public'
psql -U rc2 rc2unittest < compute/rc2root/rc2.sql
psql -U rc2 rc2unittest < compute/rc2root/testData.sql
echo "select 1" | psql -U rc2 rc2unittest
rc=$?; if [ $rc != 0 ]; then exit $rc; fi

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
