New autoconf build system
-------------------------

Due to optional introduction of readline support, GNU autotools are used:

    touch config.rpath
    # (or cp /usr/share/gettext/config.rpath .)
    autoreconf -iv
    ./configure --prefix=/usr/local

(for example).
