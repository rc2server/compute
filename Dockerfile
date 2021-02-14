FROM ubuntu:bionic

MAINTAINER Mark Lilback <mark@lilback.com>
LABEL maintainer="Mark Lilback <mark@lilback.com>"

ARG DEBIAN_FRONTEND=noninteractive
ARG pgversion=9.6

ENV pgversion=${pgversion}

RUN apt-get update \
    && apt-get install -y \
        software-properties-common wget apt-transport-https \
        gnupg ca-certificates
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys B97B0AFCAA1A47F044F244A07FCC7D46ACCC4CF8 \
    && apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys E298A3A825C0D65DFD57CBB651716619E084DAB9 \
	&& echo "deb http://apt.postgresql.org/pub/repos/apt/ bionic-pgdg main" > /etc/apt/sources.list.d/pgdg.list \
	&& echo "deb http://cran.rstudio.com/bin/linux/ubuntu bionic-cran35/" > /etc/apt/sources.list.d/R.list \
	&& apt-get update \
	&& apt-get install -y curl vim \
	    alien build-essential gcc g++ gfortran cmake git gdb postgresql-client-${pgversion} libpq-dev \
	    libicu-dev libcurl4-openssl-dev libxml2 libminizip-dev \
		libtiff5-dev libbz2-dev uuid-dev \
		libssl-dev libreadline-dev libgoogle-glog-dev \
		xorg-dev libcairo2-dev pandoc perl \
		libboost-all.dev uuid-dev libicu-dev libevent-dev \
		libevent-pthreads-2.1-6 libgtest-dev \
		libcairo2-dev libpango1.0-dev libbz2-dev \
		libcurl4-openssl-dev libssl-dev libreadline-dev \
		libpng-dev libjpeg-dev libjbig-dev default-jdk default-jre \
    && apt-get install -y --no-install-recommends texlive texinfo texlive-fonts-extra texlive-lang-english \
    && apt-get install -y r-base-dev \
    && mkdir /compute \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

ADD tools/RInside_0.2.13.tar.gz /tmp/
##COPY . /compute
## Added above copy line to build inside the container without mounting volumes.
RUN mkdir -p /rc2compute/userlib \
	&& echo 'R_LIBS_USER=/rc2compute/userlib' >> /etc/R/Renviron.site \
	&& echo 'options(repos = c(CRAN = "https://cran.rstudio.com/"), download.file.method = "libcurl")' >> /usr/lib/R/etc/Rprofile.site \
	&& Rscript -e 'install.packages(c("knitr","rmarkdown","Rcpp","inline"), "/usr/local/lib/R/site-library")' \
	&& cd /tmp/RInside \
	&& R CMD INSTALL --library=/usr/local/lib/R/site-library . \
	&& cd / \
	&& rm -rf /tmp/RInside*

# R deepends on the tzdata package, which prompts for the user's location. The first stops this, but sets tz to UTC.
#ENV TZ=America/New_York

# 2.92 GB
#