#include <api.h>
#ifdef __EMSCRIPTEN__
#include <java_script.hpp>
#endif

//
// mqrb_exec_mruby_script
//
#ifdef USE_RUBY_COMPILER
MQRB_API
int mqrb_exec_mruby_script(const char* script, size_t length)
{
  mrb_state* mrb = mrb_open();
  int ret = 0;

  mrb_load_string(mrb, script);
  if (is_exception(mrb)) {
    ret = 1;
  }

  mrb_close(mrb);
  return ret;
}
#endif

//
// mqrb_exec_mruby_irep
//
MQRB_API
int mqrb_exec_mruby_irep(const uint8_t* irep, size_t length)
{
  mrb_state* mrb = mrb_open();
  int ret = 0;

  mqrb_initialize(mrb);

  mrb_load_irep(mrb, irep);
  if (is_exception(mrb)) {
    ret = 1;
  }

  mrb_close(mrb);
  return ret;
}

//
// mqrb_initialize
//
MQRB_API int mqrb_initialize(mrb_state* mrb)
{
// JavaScript
#ifdef __EMSCRIPTEN__
  mqrb::java_script::initialize(mrb);
#endif

  return 0;
}