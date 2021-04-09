#!/bin/bash

prefix=/usr/local
if [ ! -z $2 ]; then
    prefix=$2
fi

build_module(){
    mkdir $2
    meson --prefix=$prefix $2 $1
}

compile_module(){
    cd $1
    ninja
    cd ..
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


modules=("ufjfmltk-core" "ufjfmltk-classifier" "ufjfmltk-valid" "ufjfmltk-ensemble" "ufjfmltk-featselect"\
 "ufjfmltk-regressor" "ufjfmltk-clusterer" "ufjfmltk-visual")

if [ -z "$1" ]; then
    exit 1
fi

if [ $1 = "build" ]; then
    cd subprojects
    build_folder="build_${modules[$2]}"
    build_module $module $build_folder
    compile_module $build_folder
fi

if [ $1 = "clean" ]; then
    cd subprojects
    for module in "${modules[@]}"
    do
        build_folder="build_$module"
        clear_build $build_folder
    done
fi

if [ $1 = "install" ]; then
    if compgen -G "$prefix/lib/x86_64-linux-gnu/lib${modules[$2]}*" > /dev/null; then
        echo "${modules[$2]} is installed"
        exit 0
    fi
    cd subprojects
    build_folder="build_${modules[$2]}"
    install_module $build_folder
    cd ..
fi

if [ $1 = "install_clean" ]; then
    if compgen -G "$prefix/lib/x86_64-linux-gnu/lib${modules[$2]}*" > /dev/null; then
        echo "${modules[$2]} is installed"
        exit 0
    fi
    cd subprojects
    build_folder="build_${modules[$2]}"
    install_module $build_folder
    clear_build $build_folder
    cd ..
fi

if [ $1 = "all" ]; then
    cd subprojects
    
    for module in "${modules[@]}"
    do
        if ! compgen -G "${prefix}/lib/x86_64-linux-gnu/lib${module}*" > /dev/null; then
            build_folder="build_$module"
            clear_build $build_folder
            build_module $module $build_folder
            compile_module $build_folder
            install_module $build_folder
            clear_build $build_folder
        else
            echo "$module is installed"
        fi
    done
    cd ..
    exit 0
fi