For installing the libtdi1 library    
    sudo apt-get update
    sudo apt-get install build-essential libusb-1.0-0-dev swig cmake python-dev libconfuse-dev libboost-all-dev
    wget http://www.intra2net.com/en/developer/libftdi/download/libftdi1-1.2.tar.bz2
    tar xvf libftdi1-1.2.tar.bz2
    cd libftdi1-1.2
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX="/usr/" ../
    make
    sudo make install