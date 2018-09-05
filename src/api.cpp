#include <api.h>
#include <ruby_instance.hpp>
#ifdef __EMSCRIPTEN__
#include <java_script_object.hpp>
#endif

//
// mqrb_initialize();
//
MQRB_API
int mqrb_initialize()
{
#ifdef __EMSCRIPTEN__
  mqrb_jsf_initialize();
#endif
  return 0;
}

//
// mqrb_create_instance();
//
MQRB_API
mqrb::RubyInstance* mqrb_create_instance()
{
  return static_cast<mqrb::RubyInstance*>(new mqrb::RubyInstance());
}

//
// mqrb_delete_instance
//
MQRB_API
void mqrb_delete_instance(mqrb::RubyInstance* instance)
{
  delete static_cast<mqrb::RubyInstance*>(instance);
}

//
// mqrb_exec_mruby_script
//
#ifdef USE_RUBY_COMPILER
MQRB_API
int mqrb_exec_script(mqrb::RubyInstance* instance, const char* script, size_t length)
{
  int ret = 0;
  auto mrb = instance->mrb;

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
int mqrb_exec_irep(mqrb::RubyInstance* instance, const uint8_t* bin)
{
  int ret = 0;
  auto mrb = static_cast<mqrb::RubyInstance*>(instance)->mrb;

  mrb_load_irep(mrb, bin);
  if (is_exception(mrb)) {
    ret = 1;
  }

  return ret;
}

#ifdef __EMSCRIPTEN__
MQRB_API
int mqrb_exec_irep_by_callback_index(mqrb::RubyInstance* instance, int cb_index, int argc)
{
  int ret = 0;
  auto rb_instance = static_cast<mqrb::RubyInstance*>(instance);
  auto mrb = rb_instance->mrb;
  auto proc = rb_instance->get_proc_by_index(cb_index);

  if (argc == 0) {
    mqrb_exec_irep(rb_instance, rb_instance->proc_bin[cb_index]->p);
  } else {
    mrb_value argv[argc];
    for (int i = 0; i < argc; i++) {
      auto jso = mrb_obj_new(mrb, mqrb::JavaScriptObject::RC_JavaScriptObject, 0, nullptr);
      mqrb_jsf_shift_global_queue(reinterpret_cast<mrb_int>(mrb_obj_ptr(jso)));
      argv[i] = jso;
    }
    // mrb_yield_argv(mrb, proc, argc, argv); // deprecated　method (https://github.com/mruby/mruby/pull/3701#issuecomment-308422578)
    mrb_funcall_argv(mrb, proc, mrb_intern_lit(mrb, "call"), argc, argv);
  }

  return ret;
}
#endif