#!/bin/bash

prefix=$1
if [ -z "$1" ]; then
    prefix='/usr/local'
fi
echo $prefix

if [ -e "builddir" ]; then
    meson --prefix=$prefix --reconfigure builddir
else
    mkdir builddir
    meson --prefix=$prefix builddir
fi

cd builddir
ninja -j 6