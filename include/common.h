#pragma once
#define MNQRB_VERSION_MAJOR_ 1
#define MNQRB_VERSION_MINOR_ 0
#define MNQRB_VERSION_REVISION_ 0
#define MNQRB_VERSION_IS_DEV_ true

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <cstdint>

#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/dump.h>
#include <mruby/hash.h>
#include <mruby/irep.h>
#include <mruby/proc.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#ifdef USE_RUBY_COMPILER
#include <mruby/compile.h>
#endif

// #ifdef MRB_DEBUG
// #include <mruby/debug.h>
// #endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define MQRB_API EMSCRIPTEN_KEEPALIVE
#else
#define MQRB_API extern
#endif

#define MQRB_PROMISE bool
#define MQRB_SUCCESS true
#define MQRB_FAILURE false

struct GeneralBinaryPtr
{
  uint8_t* p;
  size_t size;
  GeneralBinaryPtr(uint8_t* p, size_t size)
  {
    this->p = p;
    this->size = size;
  }
};