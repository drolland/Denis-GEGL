#!/bin/sh

./autogen.sh --prefix=/home/denis/usr CFLAGS="-O0"i --enable-debug=yes
make 
make install
