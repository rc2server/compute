if [ $TRAVIS_OS_NAME = linux ]; then

	if [ ! -f /usr/local/lib/R/site-library/RInside/libs/RInside.h ]; then

		(cd vendor/RInside; sudo R CMD INSTALL .)

	fi

fi

