Monocoque Ruby (mqrb)
=============

[![Build Status](https://travis-ci.org/noontage/monocoque-ruby.svg?branch=master)](https://travis-ci.org/noontage/monocoque-ruby) [![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

## Summary

Monocoque-Ruby(mqrb) is quick and easy executing Ruby code that runs on the WebBrowser.

(This project is still a work in progress.)


## More

The final aim of mqrb is a skeleton is generated with one command and try ruby on the webbrowzer.


## How to use

```bash
gem install mqrb
```

```bash
mqrb create-app myapp --runtime=mqrb-with-compiler
```


## How to build

### Native Build

#### Require tools

 - Ruby (CRuby)

 - CMake

 - Clang

 - Emscripten
   - This project use `emcc` command. For details, please refer to the official [website](https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html).

#### Build

`make wasm-production`


### Docker

  1. `docker build --no-cache -t emscripten:mqrb .`
  1. `docker run -dit --name emscripten -v $(pwd):/src emscripten:mqrb bash`
  1. `docker exec -it -e MRUBY_CONFIG="../../tools/mruby-patcher/build_config.wasm.rb" emscripten make wasm-production`


## License

[MIT License](MITL).