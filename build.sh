#!/bin/bash

prefix=$1
if [ -z "$1" ]; then
    prefix='/usr/local'
fi
echo $prefix

if [ -e "builddir" ]; then
    meson --prefix=$prefix --buildtype=release --reconfigure builddir
else
    mkdir builddir
    meson --prefix=$prefix --buildtype=release builddir
fi

ninja -C builddir -j 6