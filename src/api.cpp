#include <api.h>
#ifdef __EMSCRIPTEN__
#include <java_script.hpp>
#endif

//
// mqrb_initialize
//
int mqrb_initialize(mrb_state* mrb)
{
// JavaScript
#ifdef __EMSCRIPTEN__
  mqrb::java_script::initialize(mrb);
#endif

  return 0;
}

//
// mqrb_create_instance();
//
MQRB_API
mrb_state* mqrb_create_instance()
{
  auto mrb = mrb_open();
  mqrb_initialize(mrb);
  return mrb;
}

//
// mqrb_delete_instance
//
MQRB_API
void mqrb_delete_instance(mrb_state* mrb)
{
  mrb_close(mrb);
}

//
// mqrb_exec_mruby_script
//
#ifdef USE_RUBY_COMPILER
MQRB_API
int mqrb_exec_mruby_script(mrb_state* mrb, const char* script, size_t length)
{
  int ret = 0;

  mrb_load_string(mrb, script);
  if (is_exception(mrb)) {
    ret = 1;
  }

  return ret;
}
#endif

//
// mqrb_exec_mruby_irep
//
MQRB_API
int mqrb_exec_mruby_irep(mrb_state* mrb, const uint8_t* irep, size_t length)
{
  int ret = 0;

  mrb_load_irep(mrb, irep);
  if (is_exception(mrb)) {
    ret = 1;
  }

  return ret;
}

MQRB_API
int mqrb_call_proc_by_id(mrb_state* mrb, const mrb_int)
{
  return 0;
}