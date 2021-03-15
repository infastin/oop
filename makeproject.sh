#!/bin/bash
cmake -D CMAKE_C_COMPILER=gcc -S. -B./build
ln -sf build/compile_commands.json .
cd build
make
