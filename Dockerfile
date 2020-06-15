FROM library/ubuntu:bionic
MAINTAINER Mark Lilback <mark@lilback.com>

RUN apt-get update && apt-get install -y wget default-jre apt-transport-https gnupg ca-certificates
RUN apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys B97B0AFCAA1A47F044F244A07FCC7D46ACCC4CF8 E298A3A825C0D65DFD57CBB651716619E084DAB9 \
	&& echo "deb http://apt.postgresql.org/pub/repos/apt/ bionic-pgdg main" > /etc/apt/sources.list.d/pgdg.list \
	&& echo "deb http://cran.rstudio.com/bin/linux/ubuntu bionic-cran35/" > /etc/apt/sources.list.d/R.list \
#	&& apt-key adv --keyserver keyserver.ubuntu.com --recv-keys E084DAB9 \
	&& apt-get update \
	&& apt-get install -y curl vim \
		 software-properties-common software-properties-common alien \
		 alien build-essential cmake git gdb \
		 postgresql-client-9.6 \
		libpq-dev clang libicu-dev libcurl4-openssl-dev libxml2 libminizip-dev \
		alien build-essential git libtiff5-dev libbz2-dev uuid-dev \
		libssl-dev libreadline-dev libgoogle-glog-dev 
#	&& apt-get clean
RUN apt-get install -y xorg-dev libcairo2-dev pandoc perl gfortran \
	libboost-all.dev uuid-dev libevent-dev libevent-pthreads-2.1-6 libgtest-dev \
	libtiff5-dev libcairo2-dev libbz2-dev libcurl4-openssl-dev libssl-dev libreadline-dev 

RUN apt-get install -y --no-install-recommends texlive texinfo texlive-fonts-extra texlive-lang-english

# R deepends on the tzdata package, which prompts for the user's location. The first stops this, but sets tz to UTC. 
ENV DEBIAN_FRONTEND=noninteractive 
#ENV TZ=America/New_York

RUN apt-get install -y r-base-dev \
	&& mkdir -p /rc2compute/userlib \
	&& echo 'R_LIBS_USER=/rc2compute/userlib' >> /etc/R/Renviron.site \
	&& echo 'options(repos = c(CRAN = "https://cran.rstudio.com/"), download.file.method = "libcurl")' >> /usr/lib/R/etc/Rprofile.site \
	&& Rscript -e 'install.packages(c("knitr","rmarkdown","Rcpp","inline"), "/usr/local/lib/R/site-library")'

ADD tools/RInside_0.2.13.tar.gz /tmp/ 
RUN cd /tmp/RInside \
	&& R CMD INSTALL --library=/usr/local/lib/R/site-library . \
	&& cd / \
	&& rm -rf /tmp/RInside*
	

