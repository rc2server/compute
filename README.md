working on using ChunkIdentifier to compare chunks uniquely instead of using chunk number (which could change if a non-code chunk is added before the code chunk)

# rc2compute

This is the R backend portion of the rc2 project. It builds `rserver` which listens for connection requests and then forks a copy of `rsession`. Each rsession process wraps an R session via RInside.

All communication is via json. Files are managed via the PostgreSQL database.

## antlr

### to compile the runtime
* cd vendor/antlr
* rm -rf build/* run/* antlr-src/*
* unzip -d ./antlr-src/ antlr4-cpp-runtime-4.8-source.zip 
* cd build
* cmake ../antlr4-src
* DESTDIR=../run make install
* cd ..; mv run/usr/local/* to .; rm lib/libantlr4-runtime.so*

### to generate the souce
`java -Xmx500M -cp vendor/antlr/antlr-4.8-complete.jar org.antlr.v4.Tool -Dlanguage=Cpp -Xexact-output-dir -o parser/generated/ -visitor Rc2Parser/Rc2Lexer.g4 Rc2Parser/Rc2RawParser.g4 Rc2Parser/R.g4 parser/RFilter.g4`

## building package with docker runtime

1. Build the rc2/dev image using the dockerfile. Then select that as the runtime in the Run menu. 
2. Build/Debug/test per normal.
3. Run the following to create the tarball in the build directory: `docker run --rm -w /build/compute --volume /home/mlilback/rc2/compute:/src/compute --volume /home/mlilback/rc2/compute-build:/build/compute rc2/dev:latest /usr/bin/make package`


## building notes

On checkout of project, use `git clone --recursive <URL> <Local Directory>` to checkout submodules, too.

1. make a build directory, cd into it, run `cmake ..` then `make`

In vendor/json-schema-validator need to run `git fetch && git fetch -tags` and then `git checkout 2.0.0`.

When RInside is installed on a development machine, need to go to `/usr/local/lib/R/library/RInside/libs` and `ln -s /usr/local/lib/R/site-library/RInside/libs/RInside.so libRInside.so`
`pushd /usr/local/lib/R/site-library/RInside/libs;  sudo ln -s RInside.so libRInside.so`

the dist is made via call to `make package`.

## tips

* use addr2line to turn an address from a deployed stack trace to a source line on the dev machine.

* to build RInside use `R CMD INSTALL RInside -l /usr/local/lib/R/site-library`
