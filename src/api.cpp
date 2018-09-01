#include <api.h>
#include <ruby_instance.hpp>
#ifdef __EMSCRIPTEN__
#include <java_script.hpp>
#endif

//
// mqrb_create_instance();
//
MQRB_API
void* mqrb_create_instance()
{
  return static_cast<void*>(new mqrb::RubyInstance());
}

//
// mqrb_delete_instance
//
MQRB_API
void mqrb_delete_instance(void* instance)
{
  delete static_cast<mqrb::RubyInstance*>(instance);
}

//
// mqrb_exec_mruby_script
//
#ifdef USE_RUBY_COMPILER
MQRB_API
int mqrb_exec_script(void* instance, const char* script, size_t length)
{
  int ret = 0;
  auto mrb = static_cast<mqrb::RubyInstance*>(instance)->mrb;

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
int mqrb_exec_irep(void* instance, const uint8_t* bin)
{
  int ret = 0;
  auto mrb = static_cast<mqrb::RubyInstance*>(instance)->mrb;

  mrb_load_irep(mrb, bin);
  if (is_exception(mrb)) {
    ret = 1;
  }

  return ret;
}

MQRB_API
int mqrb_exec_irep_by_callback_index(void* instance, int cb_index, int argc)
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
      auto jso = mrb_obj_new(mrb, mqrb::java_script::RC_JavaScriptObject, 0, nullptr);
      mqrb_jsf_shift_global_queue(reinterpret_cast<mrb_int>(mrb_obj_ptr(jso)));
      argv[i] = jso;
    }
    // mrb_yield_argv(mrb, proc, argc, argv); // deprecated　method (https://github.com/mruby/mruby/pull/3701#issuecomment-308422578)
    mrb_funcall_argv(mrb, proc, mrb_intern_lit(mrb, "call"), argc, argv);
  }

  return ret;
}

/*
MQRB_API
int mqrb_call_proc_by_id(mqrb::RubyInstance* instance, const mrb_int cb_id, const mrb_int argc)
{
  int ret = 0;
  auto mrb = instance->mrb;
  mrb_value proc = mrb_obj_value(mrb_proc_new(mrb, reinterpret_cast<RProc*>(cb_id)->body.irep));

  if (!mrb_nil_p(proc)) {
    auto ai = mrb_gc_arena_save(mrb);

    if (argc == 0) {
      mrb_funcall(mrb, proc, "call", 0);
    } else {
      mrb_value argv_values[argc];
      for (int i = 0; i < argc; i++) {
        argv_values[i] = mrb_obj_new(mrb, mqrb::java_script::RC_JavaScriptObject, 0, nullptr);
        mqrb_jsf_shift_global_queue(reinterpret_cast<mrb_int>(mrb_obj_ptr(argv_values[i])));
      }
      // mrb_yield_argv(mrb, proc, argc, argv_values); // deprecated　method (https://github.com/mruby/mruby/pull/3701#issuecomment-308422578)
      mrb_funcall_argv(mrb, proc, mrb_intern_lit(mrb, "call"), argc, argv_values);
    }
    if (is_exception(mrb)) {
      ret = 1;
      mrb->exc = nullptr;
    }

    mrb_gc_arena_restore(mrb, ai);
  }
  return ret;
}
*/