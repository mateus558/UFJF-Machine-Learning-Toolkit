prefix=/usr/local
if [ ! -z $1 ]; then
    prefix=$1
fi

sudo rm -r -v $prefix/include/ufjfmltk
sudo rm -v $prefix/lib/x86_64-linux-gnu/libufjfmltk*
sudo rm -v $prefix/lib/x86_64-linux-gnu/pkgconfig/ufjfmltk*