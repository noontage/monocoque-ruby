#include <ruby_instance.hpp>
#ifdef __EMSCRIPTEN__
#include <java_script_object.hpp>
#endif

namespace mqrb {

std::unordered_map<mrb_state*, RubyInstance*> RubyInstance::tbl_mrb_instance;

//
// RubyInstance
//
RubyInstance::RubyInstance()
{
  mrb                   = mrb_open();
  tbl_mrb_instance[mrb] = this;
  proc_bin.reserve(4);

#ifdef __EMSCRIPTEN__
  mqrb::JavaScriptObject::initialize(mrb);  // JavaScriptObject class
#endif
}

//
// ~RubyInstance
//
RubyInstance::~RubyInstance()
{
  for (auto p : proc_bin) {
    mrb_free(mrb, p->p);
    delete p;
  }
  mrb_close(mrb);
}

//
// register_proc
//
size_t RubyInstance::register_proc(const mrb_value proc)
{
  uint8_t* bin;
  size_t size;

  auto rproc = mrb_proc_ptr(proc);
  auto ret   = mrb_dump_irep(mrb, mrb_proc_ptr(proc)->body.irep, DUMP_ENDIAN_NAT, &bin, &size);
  if (ret != MRB_DUMP_OK) {
    std::cout << "[ERROR] Can't dump mruby irep." << std::endl;
    return -1;
  }
  proc_bin.push_back(new GeneralBinaryPtr(bin, size));

  return static_cast<mrb_int>(proc_bin.size() - 1);
};

//
// get_proc_by_index
//
mrb_value RubyInstance::get_proc_by_index(size_t index)
{
  auto ai = mrb_gc_arena_save(mrb);

  auto irep  = mrb_read_irep(mrb, proc_bin.at(index)->p);
  auto rproc = mrb_closure_new(mrb, irep);
  auto proc  = mrb_obj_value(rproc);

  mrb_gc_arena_restore(mrb, ai);

  return proc;
}

}  // namespace mqrb
