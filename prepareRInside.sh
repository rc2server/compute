#!/bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPTDIR=$(dirname "$SCRIPT")
DESTDIR=`pwd`

echo "patching RInside" 1>&2

tar zxf $SCRIPTDIR/tools/RInside_0.2.*.tar.gz -C "$DESTDIR"
cd $DESTDIR/RInside
patch -p1 < $SCRIPTDIR/tools/rinside.patch



