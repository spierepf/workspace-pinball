#!/bin/bash

while ! git submodule update --init --recursive ; do echo ; done

pushd fadecandy/server
make
popd

pushd nelson
git checkout master
python generate_shows.py
popd

pushd mpf/mpf/platform
ln -s ../../../kingpyn/kingpyn_platform.py .
popd

pushd mpf
patch -p0 <../patch
popd
