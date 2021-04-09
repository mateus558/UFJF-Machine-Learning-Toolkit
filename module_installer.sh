#!/bin/bash

modules=("ufjfmltk-core" "ufjfmltk-classifier" "ufjfmltk-valid" "ufjfmltk-ensemble" "ufjfmltk-featselect"\
 "ufjfmltk-regressor" "ufjfmltk-clusterer" "ufjfmltk-visual")
module=$1
prefix=/usr/local
if [ ! -z $2 ]; then
    prefix=$2
fi

build_module(){
    builddir="build_$1"
    if [ -e $builddir ]; then
        meson --reconfigure --prefix=$prefix $builddir $1 > /dev/null
    else
        mkdir $builddir
        meson --prefix=$prefix $builddir $1 > /dev/null
    fi
    echo $builddir
}

install_module(){
    cd $1
    sudo ninja install
    cd ..
}

clear_build(){
    if [ -d $1 ] ; then
        sudo rm -r $1
    fi
}

if [[ "${modules[@]}" =~ "${module}" ]]; then
    cd subprojects
    if compgen -G "$prefix/lib/x86_64-linux-gnu/lib${module}*" > /dev/null; then
        echo "${module} is already installed."
        exit 0
    fi
    builddir=$(build_module $module)
    ninja -C $builddir
    install_module $builddir
    clear_build $builddir
    cd ..
else
    echo "$module does not belong to ufjfmltk..."
fi