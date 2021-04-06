if [ ! -e "build" ]; then
    ./build.sh   
fi
cd build
sudo ninja install