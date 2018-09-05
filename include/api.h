#pragma once
#include <common.h>
#include <ruby_instance.hpp>

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

  MQRB_API mqrb::RubyInstance* mqrb_create_instance();
  MQRB_API void mqrb_delete_instance(mqrb::RubyInstance* instance);

// if use ruby compiler
#ifdef USE_RUBY_COMPILER
  MQRB_API int mqrb_exec_script(mqrb::RubyInstance* instance, const char* script, size_t length);
#endif
  MQRB_API int mqrb_exec_irep(mqrb::RubyInstance* instance, const uint8_t* bin);

// if emscripten
#ifdef __EMSCRIPTEN__
  MQRB_API int mqrb_exec_irep_by_callback_index(mqrb::RubyInstance* instance, int cb_index, int argc);
#endif

// extern "C"
#ifdef __cplusplus
}
#endif