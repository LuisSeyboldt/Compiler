#!/bin/bash
set -e

# Install build toolchain if any argument is given
if [ $# != 0 ]; then
	sudo apt-get install -y build-essential gdb \
                             cmake ninja-build ccache doxygen \
                             flex bison uthash-dev
fi

# build dhbwcc
mkdir -p bin
cd bin
cmake .. -G Ninja
cmake --build . --target all --config Debug
