#!/bin/sh

echo "local	all	rc2	trust" >> /etc/postgresql/9.4/main/pg_hba.conf
psql -U postgres -c 'drop database if exists rc2test'
psql -U postgres -c 'drop role if exists rc2'
psql -U postgres -c 'create role rc2 login'
psql -U postgres -c 'create database rc2test owner rc2'
psql -U postgres rc2test -c 'CREATE EXTENSION IF NOT EXISTS pgcrypto WITH SCHEMA public'
psql -U rc2 rc2test < compute/rc2root/rc2.sql
psql -U rc2 rc2test < compute/rc2root/testData.sql
echo "select 1" | psql -U rc2 rc2unittest
rc=$?; if [ $rc != 0 ]; then exit $rc; fi

if [ ! -e build ]; then
	mkdir build
fi
cd build
cmake ../compute
rc=$?; if [ $rc != 0 ]; then exit $rc; fi
make
rc=$?; if [ $rc != 0 ]; then exit $rc; fi
make package
rc=$?; if [ $rc != 0 ]; then exit $rc; fi
make test CTEST_OUTPUT_ON_FAILURE=TRUE
rc=$?; if [ $rc != 0 ]; then exit $rc; fi
