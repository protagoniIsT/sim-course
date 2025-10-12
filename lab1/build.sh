#!/bin/bash
set -e
mkdir -p build
cd build
cmake ..
make -j$(nproc)
cd ..
echo "[Build] Done."
