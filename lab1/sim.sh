#!/bin/bash
set -e

if [ $# -lt 1 ]; then
    echo "Usage: $0 <ruby_file> [args...]"
    exit 1
fi

RUBY_FILE="$1"
shift

if [ ! -f "$RUBY_FILE" ]; then
    echo "Error: Ruby file '$RUBY_FILE' not found"
    exit 1
fi


echo "[1/3] Running Ruby Assembler script: $RUBY_FILE"
ruby "$RUBY_FILE" "$@"

BIN_FILE="$(basename "$RUBY_FILE" .rb).bin"

if [ ! -f "$BIN_FILE" ]; then
    echo "Error: binary file '$BIN_FILE' not found"
    exit 1
fi


echo "[2/3] Building simulator..."
sh build.sh

BUILD_DIR="build"
SIM_EXECUTABLE="$BUILD_DIR/main.out"

if [ ! -f "$SIM_EXECUTABLE" ]; then
    echo "Error: executable '$SIM_EXECUTABLE' not found"
    exit 1
fi

echo "[3/3] Running simulator with binary: $BIN_FILE"
"$SIM_EXECUTABLE" "$BIN_FILE" "$@"
