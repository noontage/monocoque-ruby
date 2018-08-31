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

  int mqrb_initialize(mrb_state* mrb);

  // ============================================ MQRB_API ============================================

  MQRB_API mrb_state* mqrb_create_instance();
  MQRB_API void mqrb_delete_instance(mrb_state* mrb);

// if use ruby compiler
#ifdef USE_RUBY_COMPILER
  MQRB_API int mqrb_exec_mruby_script(mrb_state* mrb, const char* script, size_t length);
#endif

  MQRB_API int mqrb_exec_mruby_irep(mrb_state* mrb, const uint8_t* irep, size_t length);

  MQRB_API int mqrb_call_proc_by_id(mrb_state* mrb, const mrb_int);

// extern "C"
#ifdef __cplusplus
}
#endif