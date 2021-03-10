#!/bin/bash
cmake -S. -B./build
ln -sf build/compile_commands.json .
cd build
make
