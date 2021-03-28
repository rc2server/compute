# `rc2compute`

This is the R backend portion of the rc2 project. It builds `rserver` which listens for connection requests and then forks a copy of `rsession`. Each rsession process wraps an R session via RInside.
All communication is via json. Files are managed via the PostgreSQL database.

## `antlr4`

### Compile the runtime:
* `cd vendor/antlr `
* `rm -rf build/* run/* antlr-src/* include/* share/* lib/*` (if those folders were created in a previous build).
* `unzip -d ./antlr-src/ antlr4-cpp-runtime-4.8-source.zip `
* `mdkir build; cd build `
* `cmake ../antlr-src `
* `DESTDIR=../run make install `
* `cd ..; rm -rf include/* share/* lib/*; mv run/usr/local/* .; rm lib/libantlr4-runtime.so*; cd ../../; `

### Generate the source:
```
java -Xmx500M -cp vendor/antlr/antlr-4.8-complete.jar org.antlr.v4.Tool -Dlanguage=Cpp -Xexact-output-dir -o parser/generated/ -visitor Rc2Parser/Rc2Lexer.g4 Rc2Parser/Rc2RawParser.g4 Rc2Parser/R.g4 parser/RFilter.g4
```

## Building the package with `docker` runtime:

1. Build the rc2/dev image using the dockerfile. Then select that as the runtime in the Run menu.
    ```
    cd <compute-dev-path>/compute
    docker build -t rc2/dev .
    ```
2. Run `bash` in the container:

    ```{bash}
    docker run -ti --name rc2_build rc2/dev /bin/bash
    ```

3. On the host machine, open another `bash` terminal.
    ```
    cd <compute-dev-path>/compute
    docker cp . rc2_build:/compute
    ```
4. Return to the open `docker` container `rc2_build` in the previous `bash` terminal.
Inside the container, go to `compute` directory.
`cd /compute`

5. To create the tarball in the build directory with volumes mounted to the build
container, run the following: 
    ```
    docker run --rm -w /build/compute --volume <compute-dev-path>/compute:/src/compute \
    --volume <compute-dev-path>/compute-build:/build/compute rc2/dev:latest /usr/bin/make package
    ```
6. To create the tarball inside the container, we will run the container with 
a data volume, then execute the make.  When building is complete, the package should 
be in the volume and can be extracted using the `docker cp` command.
    ```
    docker run --volume rc2_dev_data:/compute rc2/dev:latest /usr/bin/make package
    docker cp rc2_dev_data:rc2compute.tar.gz .
    ```

## building notes

On checkout of project, use `git clone --recursive <URL> <Local Directory>` to checkout submodules, too.

1. make a build directory, cd into it, run `cmake ..` then `make`

In vendor/json-schema-validator need to run `git fetch && git fetch --tags` and then `git checkout 2.0.0`.

In vendor/g3log run `git fetch && git fetch --tags` and then `git checkout 1.3.2`.

When RInside is installed on a development machine, need to go to `/usr/local/lib/R/library/RInside/libs` and `ln -s /usr/local/lib/R/site-library/RInside/libs/RInside.so libRInside.so`
`pushd /usr/local/lib/R/site-library/RInside/libs;  sudo ln -s RInside.so libRInside.so`

the dist is made via call to `make package`.

## tips

* use `addr2line` to turn an address from a deployed stack trace to a source line on the dev machine.

* to build RInside use `R CMD INSTALL RInside -l /usr/local/lib/R/site-library . `

* To do the `cmake` build inside the container, use:
`docker run -ti --name rc2_build --volume rc2_dev_data:/compute rc2/dev:latest /bin/bash`

## notes
working on using ChunkIdentifier to compare chunks uniquely instead of using chunk number (which could change if a non-code chunk is added before the code chunk)
