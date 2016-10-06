#!/bin/bash

while ! git submodule update --init --recursive ; do echo ; done

pushd fadecandy/server
make
popd

pushd nelson2
  git checkout master
  git checkout .
  python generate_shows.py
  pushd sounds
    git checkout master
    git checkout .
    for i in */*.ogg ; do normalize-ogg "$i" ; done
  popd
popd

pushd mpf/mpf/platform
rm -f kingpyn_platform.py
ln -s ../../../kingpyn/kingpyn_platform.py .
popd

pushd mpf
git checkout .
patch -p1 <../patch
popd
