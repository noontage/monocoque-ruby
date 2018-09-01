#pragma once
#include <common.h>

namespace mqrb {
namespace java_script {

  extern int initialize(mrb_state* mrb);

}  // namespace java_script
}  // namespace mqrb

#ifdef __cplusplus
extern "C"
{
#endif

  extern void mqrb_jsf_new(mrb_int id);
  extern void mqrb_jsf_new_copy(mrb_int src, mrb_int dest);
  extern void mqrb_jsf_put_obj(mrb_int id);
  extern void mqrb_jsf_eval(const char* script);
  extern void mqrb_jsf_funcall_by_id(mrb_int id, const char* script);
  extern void mqrb_jsf_inxop_by_id(mrb_int id, const char* script);
  extern void mqrb_jsf_set_value_by_id(mrb_int id, const char* script);
  extern void mqrb_jsf_get_value_by_id(mrb_int id, char* buf, mrb_int* len, size_t max_len);

// extern "C"
#ifdef __cplusplus
}
#endif
