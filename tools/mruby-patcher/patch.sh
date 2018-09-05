#!/bin/bash

if [ ! -f vendor/mruby/mrbgems/mnqrb.gembox ]; then
    echo "patched"
    cp -f tools/mruby-patcher/build_config.rb vendor/mruby/build_config.rb
    cp -f tools/mruby-patcher/mnqrb.gembox vendor/mruby/mrbgems/mnqrb.gembox
    git update-index --assume-unchanged vendor/mruby
fi