#pragma once
#define _MNQRB_VERSION_MAJOR_ 1
#define _MNQRB_VERSION_MINOR_ 0
#define _MNQRB_VERSION_REVISION_ 0
#define _MNQRB_VERSION_IS_DEV_ true

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

#include <cstdint>

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/hash.h>
#include <mruby/dump.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <mruby/data.h>
#ifdef USE_RUBY_COMPILER
#include <mruby/compile.h>
#endif


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define MQRB_API EMSCRIPTEN_KEEPALIVE
#else
#define MQRB_API extern
#endif