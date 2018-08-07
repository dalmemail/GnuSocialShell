New autoconf build system
-------------------------

Due to optional introduction of readline support, GNU autotools are used:

    NOCONFIGURE=1 ./autogen.sh
    ./configure --prefix=/usr/local
    # (or ./autogen.sh --prefix=/usr/local)
    make

(for example).
