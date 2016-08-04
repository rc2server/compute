#!/bin/sh
set -v

#if [ $TRAVIS_OS_NAME eq linux ]; then
	SRCDIR=$PWD
	echo "cloning root"
	cd $SRCDIR
	git clone https://github.com/wvuRc2/rc2.git root

	echo "checking for RInside"
	if [ ! -f /usr/local/lib/R/site-library/RInside/libs/RInside.h ]; then
		echo "installing RInside"
		pwd
		ls -l vendor
		wget https://cran.r-project.org/src/contrib/RInside_0.2.13.tar.gz
		tar zxf RInside_0.2.13.tar.gz
		cd RInside
		patch -p1 < $SRCDIR/root/containers/dev/rinside.patch
		R CMD INSTALL .
		cd $SRCDIR
		rm -rf RInside_0.2.13.tar.gz RInside
#		(cd vendor/RInside; sudo R CMD INSTALL .)

	fi

	echo "configuring database"
	cd $SRCDIR
	psq l -c 'create user rc2' -U postgres
	psql -c 'create database rc2unittest owner rc2' -U postgres
	psql -U rc2 rc2unittest < $SRCDIR/root/rc2.sql
	psql -U rc2 rc2unittest < $SRCDIR/root/testData.sql
	
#fi

