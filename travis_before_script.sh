#!/bin/sh

#if [ $TRAVIS_OS_NAME eq linux ]; then
	echo "checking for RInside"
	if [ ! -f /usr/local/lib/R/site-library/RInside/libs/RInside.h ]; then
		echo "installing RInside"
		wget https://cran.r-project.org/src/contrib/RInside_0.2.13.tar.gz
		tar zxf RInside_0.2.13.tar.gz
		cd RInside
		patch -p1 < vendor/rinside.patch
		R CMD INSTALL .
		cd ..
		rm -rf RInside_0.2.13.tar.gz RInside
#		(cd vendor/RInside; sudo R CMD INSTALL .)

	fi

#fi

