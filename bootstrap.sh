#!/bin/bash
# Part of Project KARR
# (c), released under the BSD license.

STARTDIR=${PWD}
function failed() {
    echo "Failed to build a required component, exiting..;";
    cd ${STARTDIR};
    exit 1;
}

if [ -d genie ]; then
    (cd genie && make) || failed;
    cd ${STARTDIR};
fi
if [ -d bx ]; then
    (cd bx && make) || failed;
    cd ${STARTDIR};
fi

if [ -d bgfx ] ; then
    (cd bgfx && make linux-release64 linux-debug64) || failed;
    cd ${STARTDIR};
fi
