#!/bin/bash

CUR_DIR=tools/mruby-patcher

if [ ! -f build/build_config.rb ]; then
    echo "set mrbgems"
    mkdir -p build
    cat $CUR_DIR/build_config.host.rb $CUR_DIR/build_config.wasm.rb $CUR_DIR/build_config.arm.rb > build/build_config.rb
fi