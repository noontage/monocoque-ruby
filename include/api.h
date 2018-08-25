#pragma once
#include <common.h>

#ifdef __cplusplus
extern "C"
{
#endif

  inline bool is_exception(mrb_state* mrb)
  {
    if (mrb->exc) {
      mrb_print_backtrace(mrb);
      mrb_print_error(mrb);
      return true;
    }
    return false;
  }

  MQRB_API int mqrb_initialize(mrb_state* mrb);

// if use ruby compiler
#ifdef USE_RUBY_COMPILER
  MQRB_API int mqrb_exec_mruby_script(const char* script, size_t length);
#endif

  MQRB_API int mqrb_exec_mruby_irep(const uint8_t* irep, size_t length);

// extern "C"
#ifdef __cplusplus
}
#endif