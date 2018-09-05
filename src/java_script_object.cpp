#ifdef __EMSCRIPTEN__
#include <java_script_object.hpp>
#include <ruby_instance.hpp>

namespace mqrb {
namespace JavaScriptObject {

  //
  // mqrb::JavaScriptObject
  //

  RClass* RC_JavaScriptObject;  //  RClass

  // working buffer
  const size_t WORKING_STRING_BUFFER = 256;
  char buffer_working[WORKING_STRING_BUFFER];

  //
  // DS_JavaScriptObject
  //
  void js_free(mrb_state* mrb, void* p)
  {
    std::cout << "GC" << p << std::endl;
    mrb_free(mrb, p);
  }
  const mrb_data_type DS_JavaScriptObject = {"JavaScriptObject", js_free};

  //
  // make_jsargv
  //
  void make_jsargv(std::string& argv_buf, mrb_state* mrb, mrb_value* argv, mrb_int argc)
  {
    for (mrb_int i = 0; i < argc; i++) {
      auto value = argv[i];
      switch (mrb_type(value)) {
        // string -> enclose '"'
        case MRB_TT_STRING: {
          argv_buf.append("\"");
          argv_buf.append(mrb_str_to_cstr(mrb, value));
          argv_buf.append("\"");
          break;
        }

        // bool number symbol array -> to_s
        case MRB_TT_FALSE:
        case MRB_TT_TRUE:
        case MRB_TT_FIXNUM:
        case MRB_TT_SYMBOL:
        case MRB_TT_FLOAT:
        case MRB_TT_ARRAY:
          argv_buf.append(mrb_str_to_cstr(mrb, mrb_funcall(mrb, value, "to_s", 0)));
          break;

        // HASH = > to_json
        case MRB_TT_HASH: {
          argv_buf.append(mrb_str_to_cstr(mrb, mrb_funcall(mrb, value, "to_json", 0)));
          break;
        }

        // Proc
        case MRB_TT_PROC: {
          // check params
          auto argc = mrb_fixnum(mrb_funcall(mrb, mrb_funcall(mrb, value, "parameters", 0), "size", 0));
          if (argc > 26) {
            mrb_raise(mrb, E_ARGUMENT_ERROR, "To many parameters of Proc or lambda.");
            break;
          }

          // make query of param string
          // => make string "a,b,c,d,e..."
          std::string q_argvs = "";
          for (char i = 0x61; i < (argc + 0x61); i++) {
            q_argvs += std::string{i};
            if (i + 1 != argc + 0x61) {
              q_argvs += ",";
            }
          }

          // dump and register mrb-bytecode
          auto instance = RubyInstance::get_instance_by_mrb_state(mrb);
          auto cb_index = instance->register_proc(value);

          // register param to queue
          std::string q_callback_script = "";
          for (char i = 0x61; i < (argc + 0x61); i++) {
            q_callback_script += "mqrb_jsf_imp_push_queue(" + std::string{i} + ");\n";
          }
          q_callback_script += "cwrap('mqrb_exec_irep_by_callback_index', 'number', ['number', 'number', 'number'])(" + std::to_string(reinterpret_cast<mrb_int>(instance)) + ", " + std::to_string(cb_index) + ", " +
                               std::to_string(argc) + ");";
          std::string q = "(" + q_argvs + ") => {\n " + q_callback_script + "\n }";

          argv_buf.append(q);
          break;
        }
        // error
        default: {
          auto error_message = "Can't converted Ruby value to Javascript value: unsupport type [" + std::to_string(mrb_type(value)) + "]";
          mrb_raise(mrb, E_RUNTIME_ERROR, error_message.c_str());
          std::cerr << error_message << std::endl;
        }
      }
      if (argc > (i + 1)) {
        argv_buf.append(",");
      }
    }
    return;
  }  // namespace JavaScriptObject

  //
  // query_append
  //
  inline void query_append(std::string& cur, std::string& str)
  {
    if (cur.length() != 0) {
      cur += ".";
    }
    cur += str;
    return;
  }

  //
  // mrb_initialize
  //
  mrb_value mrb_initialize(mrb_state* mrb, mrb_value self)
  {
    mqrb_jsf_new(reinterpret_cast<mrb_int>(mrb_obj_ptr(self)));
    return self;
  }

  //
  // mrb_initialize_copy
  //
  mrb_value mrb_initialize_copy(mrb_state* mrb, mrb_value self)
  {
    mrb_value src;
    mrb_get_args(mrb, "o", &src);
    if (mrb_obj_equal(mrb, self, src)) {
      return self;
    }

    if (!mrb_obj_is_instance_of(mrb, src, mrb_obj_class(mrb, self))) {
      mrb_raise(mrb, E_TYPE_ERROR, "wrong argument class");
    }

    mqrb_jsf_new_copy(reinterpret_cast<mrb_int>(mrb_obj_ptr(src)), reinterpret_cast<mrb_int>(mrb_obj_ptr(self)));

    return self;
  }

  //
  // mrb_js_exec_eval
  //
  mrb_value mrb_js_exec_eval(mrb_state* mrb, mrb_value self)
  {
    char* script;
    mrb_int s_len;
    mrb_get_args(mrb, "s", &script, &s_len);

    mqrb_jsf_exec_eval(script);
    return self;
  }

  //
  // mrb_js_print_obj
  //
  mrb_value mrb_js_print_obj(mrb_state* mrb, mrb_value self)
  {
    mqrb_jsf_print_obj(reinterpret_cast<mrb_int>(mrb_obj_ptr(self)));
    return self;
  }

  //
  // mrb_js_to_rb
  //
  mrb_value mrb_js_to_rb(mrb_state* mrb, mrb_value self)
  {
    auto id = reinterpret_cast<mrb_int>(mrb_obj_ptr(self));

    mrb_int len;
    mqrb_jsf_get_value(id, buffer_working, &len, WORKING_STRING_BUFFER);

    auto ai = mrb_gc_arena_save(mrb);
    mrb_value ret = mrb_str_new(mrb, buffer_working, len);
    if (mrb_str_strlen(mrb, RSTRING(ret)) == 0) {
      return mrb_nil_value();
    }
    mrb_gc_arena_restore(mrb, ai);

    mrb_value mrb_mod_json_value = mrb_obj_value(mrb_module_get(mrb, "JSON"));

    return mrb_funcall(mrb, mrb_mod_json_value, "parse", 1, ret);
  }

  //
  // mrb_js_op_ary
  //
  mrb_value mrb_js_op_ary(mrb_state* mrb, mrb_value self)
  {
    mrb_value idx;
    mrb_get_args(mrb, "o", &idx);

    auto id = reinterpret_cast<mrb_int>(mrb_obj_ptr(self));
    std::string script;
    std::string name = std::string(mrb_str_to_cstr(mrb, mrb_funcall(mrb, idx, "to_s", 0)));

    mqrb_jsf_op_ary(id, name.c_str());

    return self;
  }

  //
  // mrb_js_op_eq
  //
  mrb_value mrb_js_op_eq(mrb_state* mrb, mrb_value self)
  {
    mrb_value idx;
    mrb_get_args(mrb, "o", &idx);

    auto id = reinterpret_cast<mrb_int>(mrb_obj_ptr(self));
    std::string script;
    std::string name = std::string(mrb_str_to_cstr(mrb, mrb_funcall(mrb, idx, "to_s", 0)));

    mqrb_jsf_set_value(id, name.c_str());

    return self;
  }

  //
  // method_missing
  //
  mrb_value method_missing(mrb_state* mrb, mrb_value self)
  {
    mrb_sym rb_method;
    mrb_value* argv;
    mrb_int argc;
    mrb_get_args(mrb, "n*!", &rb_method, &argv, &argc);

    auto ai = mrb_gc_arena_save(mrb);

    std::string buf_argvstr;
    auto method = mrb_sym2name(mrb, rb_method);
    auto copy = mrb_obj_dup(mrb, self);
    auto copy_id = reinterpret_cast<mrb_int>(mrb_obj_ptr(copy));

    if (argc == 0) {
      mqrb_jsf_funcall(copy_id, method);
    } else {  // if function
      make_jsargv(buf_argvstr, mrb, argv, argc);
      std::string script = std::string(method) + "(" + buf_argvstr + ")";
      mqrb_jsf_funcall(copy_id, script.c_str());
    }

    mrb_gc_arena_restore(mrb, ai);

    return copy;
  }

  //
  // initialize
  //
  int initialize(mrb_state* mrb)
  {
    RC_JavaScriptObject = mrb_define_class(mrb, "JavaScriptObject", mrb->object_class);

    // initialize
    mrb_define_method(mrb, RC_JavaScriptObject, "initialize", mrb_initialize, MRB_ARGS_NONE());
    mrb_define_method(mrb, RC_JavaScriptObject, "initialize_copy", mrb_initialize_copy, MRB_ARGS_REQ(1));

    // exec_eval
    mrb_define_method(mrb, RC_JavaScriptObject, "_eval", mrb_js_exec_eval, MRB_ARGS_REQ(1));

    // instance method
    mrb_define_method(mrb, RC_JavaScriptObject, "_p", mrb_js_print_obj, MRB_ARGS_NONE());  //  print JS object to console.log
    mrb_define_method(mrb, RC_JavaScriptObject, "_val", mrb_js_to_rb, MRB_ARGS_NONE());    //  get ruby value from JS object

    // operator
    mrb_define_method(mrb, RC_JavaScriptObject, "[]", mrb_js_op_ary, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, RC_JavaScriptObject, "=", mrb_js_op_eq, MRB_ARGS_REQ(1));

    // dynamic execute javascript
    mrb_define_method(mrb, RC_JavaScriptObject, "method_missing", method_missing, MRB_ARGS_REQ(2));

    // for GC
    MRB_SET_INSTANCE_TT(RC_JavaScriptObject, MRB_TT_DATA);

    return 0;
  }

}  // namespace JavaScriptObject
}  // namespace mqrb
#endif