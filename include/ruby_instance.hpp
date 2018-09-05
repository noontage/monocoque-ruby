#pragma once
#include <common.h>

#include <unordered_map>
#include <vector>

namespace mqrb {

//
// RubyInstance
//
class RubyInstance
{
public:
  mrb_state* mrb;
  std::vector<GeneralBinaryPtr*> proc_bin;
  static std::unordered_map<mrb_state*, RubyInstance*> tbl_mrb_instance;

  RubyInstance();
  ~RubyInstance();
  size_t register_proc(const mrb_value proc);
  mrb_value get_proc_by_index(size_t index);

  static inline RubyInstance* get_instance_by_mrb_state(mrb_state* mrb) { return tbl_mrb_instance.at(mrb); }
};

}  // namespace mqrb
