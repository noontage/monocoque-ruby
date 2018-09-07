#pragma once
#include <common.h>

namespace mqrb {
namespace JavaScriptObject {
  extern RClass* RC_JavaScriptObject;
  extern int initialize(mrb_state* mrb);
}  // namespace JavaScriptObject
}  // namespace mqrb

#ifdef __cplusplus
extern "C"
{
#endif

  extern void mqrb_jsf_initialize();
  extern void mqrb_jsf_exec_eval(const char* script);
  extern void mqrb_jsf_new(mrb_int id);
  extern void mqrb_jsf_new_copy(mrb_int src, mrb_int dest);
  extern void mqrb_jsf_print_obj(mrb_int id);
  extern void mqrb_jsf_shift_global_queue(mrb_int obj_id);
  extern void mqrb_jsf_funcall(mrb_int id, const char* script);
  extern void mqrb_jsf_op_ary(mrb_int id, const char* script);
  extern void mqrb_jsf_set_value(mrb_int id, const char* script);
  extern void mqrb_jsf_get_value(mrb_int id, char* buf, mrb_int* len, size_t max_len);

// extern "C"
#ifdef __cplusplus
}
#endif
