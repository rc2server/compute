#!/bin/sh

#if [ $TRAVIS_OS_NAME eq linux ]; then
	echo "checking for RInside"
	if [ ! -f /usr/local/lib/R/site-library/RInside/libs/RInside.h ]; then
		echo "installing RInside"
		(cd vendor/RInside; sudo R CMD INSTALL .)

	fi

#fi

