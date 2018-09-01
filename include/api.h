#pragma once
#include <common.h>

#ifdef __cplusplus
extern "C"
{
#endif

  inline int is_exception(mrb_state* mrb)
  {
    if (mrb->exc) {
      mrb_print_backtrace(mrb);
      mrb_print_error(mrb);
      return 1;
    }
    return 0;
  }

  // ============================================ MQRB_API ============================================

  MQRB_API void* mqrb_create_instance();
  MQRB_API void mqrb_delete_instance(void* instance);

// if use ruby compiler
#ifdef USE_RUBY_COMPILER
  MQRB_API int mqrb_exec_script(void* instance, const char* script, size_t length);
#endif
  MQRB_API int mqrb_exec_irep(void* instance, const uint8_t* bin);
  MQRB_API int mqrb_exec_irep_by_callback_index(void* instance, int cb_index, int argc);

  // MQRB_API int mqrb_call_proc_by_id(mqrb::RubyInstance* instance, const mrb_int, const mrb_int argc);  

// extern "C"
#ifdef __cplusplus
}
#endif