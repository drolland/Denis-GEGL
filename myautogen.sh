#!/bin/sh

export PKG_CONFIG_PATH=/home/denis/usr/lib/pkgconfig
./autogen.sh --prefix=/home/denis/usr CFLAGS="-O0 -DHEAVY_DEBUG" --enable-debug=yes
