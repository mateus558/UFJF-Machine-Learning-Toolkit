#!/bin/bash

build_module(){
    mkdir $2
    meson --prefix=/usr $2 $1
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
        rm -r $1
    fi
}


modules=("ufjfmltk_core" "ufjfmltk_classifier" "ufjfmltk_valid" "ufjfmltk_ensemble" "ufjfmltk_featselect" "ufjfmltk_visual")

if [ -z "$1" ]; then
    exit 1
fi

if [ $1 = "build" ]; then
    cd subprojects
    module=${modules[$2]}
    build_folder="build_$module"
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
    if [ $EUID != 0 ]; then
        sudo "$0" "$1"
        exit $?
    fi
    cd subprojects
    module=${modules[$2]}
    build_folder="build_$module"
    install_module $build_folder
fi

if [ $1 = "install_clean" ]; then
    if [ $EUID != 0 ]; then
        exit $?
    fi
    cd subprojects
    install_module $2
    clear_build $2
    cd ..
fi

if [ $1 = "all" ]; then
    cd subprojects
    for module in "${modules[@]}"
    do
        build_folder="build_$module"
        clear_build $build_folder
        build_module $module $build_folder
        compile_module $build_folder
        install_module $build_folder
        clear_build $build_folder
    done
fi