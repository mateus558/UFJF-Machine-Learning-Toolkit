#!/bin/bash

prefix=/usr/local
if [ ! -z $1 ]; then
    prefix=$1
fi

sudo chmod +x module_installer.sh

if [ ! -e "builddir" ]; then
    ./build.sh $prefix
fi

sudo ninja install -C builddir