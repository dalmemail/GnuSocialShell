New autoconf build system
-------------------------

Due to optional introduction of readline support, GNU autotools are used:

    touch config.rpath
    autoreconf -iv
    ./configure --prefix=/usr/local
    make

(for example).
