#!/bin/bash

CUR_DIR=tools/mruby-patcher

if [ ! -f build/build_config.rb ]; then
    echo "set mrbgems"
    mkdir -p build
    cat $CUR_DIR/*.rb > build/build_config.rb
fi