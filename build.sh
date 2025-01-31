#!/bin/sh

if ! [ -e build/ ]; then
  mkdir build
fi
cd build/

cmake ..
make -j8

./yap
