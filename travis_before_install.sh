if [ $TRAVIS_OS_NAME = linux ]; then

sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys E084DAB9
sudo apt-add-repository "deb http://cran.mirrors.hoobly.com/bin/linux/ubuntu trusty/"

sudo apt-get update -qq
sudo apt-get install -y --no-install-recommends \
	uuid-dev libssl-dev libcurl4-openssl-dev libboost-all-dev libgtest-dev libevent-dev r-base-dev

cd /tmp && wget http://cran.mirrors.hoobly.com/src/contrib/Rcpp_0.12.6.tar.gz && sudo R CMD INSTALL Rcpp_0.12.6.tar.gz
mkdir /tmp/buildgt && cd /tmp/buildgt && cmake /usr/src/gtest && make && sudo mv -f libgtest* /usr/lib/


fi

