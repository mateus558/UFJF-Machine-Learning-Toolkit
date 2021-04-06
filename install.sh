#!/bin/bash

sudo chmod +x install_modules.sh
./install_modules.sh all

if [ ! -e "builddir" ]; then
    ./build.sh
fi

cd builddir
sudo ninja install