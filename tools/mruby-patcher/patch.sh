#!/bin/bash

CUR_DIR=tools/mruby-patcher
GEMBOX_FILE=mnqrb.gembox

if [ ! -f vendor/mruby/mrbgems/$GEMBOX_FILE ]; then
    echo "set mrbgems"
    mkdir -p build
    cat $CUR_DIR/build_config.host.rb $CUR_DIR/build_config.wasm.rb $CUR_DIR/build_config.arm.rb > build/build_config.rb
fi