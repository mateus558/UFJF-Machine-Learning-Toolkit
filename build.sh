#!/bin/bash

if [ -e "build" ]; then
    meson --prefix=/usr --reconfigure build
else
    mkdir build
    meson --prefix=/usr build
fi

cd build
ninja -j 6