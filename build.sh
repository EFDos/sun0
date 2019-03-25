#!/bin/bash

if [ ! -d "build/" ]; then
    meson build/ -Dbuild-demos=true
fi

cd build/
ninja
cd ..
./build/src/demos/sun-sandbox
