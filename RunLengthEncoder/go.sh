#!/bin/bash

echo "#include <$1.xbm>" > include/tmp.h
echo "uint8_t buf[sizeof($1_bits)];" >> include/tmp.h
echo "size_t length = rle_compress($1_bits, sizeof($1_bits), buf);" >> include/tmp.h
echo "printf(\"#define $1_rle_length %li\\n\", length);" >> include/tmp.h
echo "printf(\"const unsigned char $1_rle_data[] PROGMEM = {\");" >> include/tmp.h

pushd Debug
make clean all
./RunLengthEncoder > ../../XbmDisplay/include/$1.rle
popd
