#include <java_script.hpp>
#include <vm.hpp>

extern "C"
{
  extern void mqrb_javascript_new(mrb_int id);
  extern void mqrb_javascript_eval(const char* script);
  extern void mqrb_javascript_funcall_by_id(mrb_int id, const char* script);
  extern void mqrb_javascript_inxop_by_id(mrb_int id, const char* script);
  extern void mqrb_javascript_set_value_by_id(mrb_int id, const char* script);
  extern void mqrb_javascript_get_value_by_id(mrb_int id, char* buf, mrb_int* len, size_t max_len);
}

namespace mqrb {
namespace java_script {

  //
  // struct JavaScriptObject
  //
  struct JavaScriptObject
  {
    std::string q;
  };

  //
  // define option
  //
  const size_t WORKING_STRING_BUFFER = 256;

  //
  // DS_JavaScriptObject
  //
  void js_free(mrb_state* mrb, void* p) { delete static_cast<JavaScriptObject*>(p); }
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
          // generate js code
          mrb_int mrb_i = reinterpret_cast<mrb_int>(mrb);
          mrb_int cb_id = mrb_obj_id(value);  //  instance object_id
          std::string q = "function() { cwrap('mqrb_call_proc_by_id', 'number', 'number')(" + std::to_string(mrb_i) + ", " + std::to_string(cb_id) + "); }";
          argv_buf.append(q);
          // register proc
          mrb_gc_protect(mrb, value);
          mqrb::vm::table_callback_proc[cb_id] = value;
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
  }

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
    DATA_TYPE(self) = &DS_JavaScriptObject;
    DATA_PTR(self) = static_cast<void*>(new JavaScriptObject());

    mqrb_javascript_new(reinterpret_cast<mrb_int>(DATA_PTR(self)));
    return self;
  }

  //
  // mrb_js_eval
  //
  mrb_value mrb_js_eval(mrb_state* mrb, mrb_value self)
  {
    char* script;
    mrb_int s_len;
    mrb_get_args(mrb, "s", &script, &s_len);

    mqrb_javascript_eval(script);
    return self;
  }

  //
  // mrb_js_q_function
  //
  mrb_value mrb_js_q_function(mrb_state* mrb, mrb_value self)
  {
    std::string argv_str_buf, script = "";

    char* name;
    mrb_int s_len;
    mrb_value* argv;
    mrb_int argc;
    mrb_get_args(mrb, "s*!", &name, &s_len, &argv, &argc);

    make_jsargv(argv_str_buf, mrb, argv, argc);
    script = std::string(name) + "(" + argv_str_buf + ")";

    auto jso = static_cast<JavaScriptObject*>(DATA_PTR(self));
    query_append(jso->q, script);

    return self;
  }

  //
  // mrb_js_q_object
  //
  mrb_value mrb_js_q_object(mrb_state* mrb, mrb_value self)
  {
    char* name;
    mrb_int s_len;
    mrb_get_args(mrb, "s", &name, &s_len);

    auto jso = static_cast<JavaScriptObject*>(DATA_PTR(self));
    auto name_s = std::string(name);
    query_append(jso->q, name_s);

    return self;
  }

  //
  // mrb_js_exec
  //
  mrb_value mrb_js_exec(mrb_state* mrb, mrb_value self)
  {
    auto jso = static_cast<JavaScriptObject*>(DATA_PTR(self));
    mqrb_javascript_eval(jso->q.c_str());
    jso->q.clear();

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

    auto name = std::string(mrb_sym2name(mrb, rb_method));
    auto jso = static_cast<JavaScriptObject*>(DATA_PTR(self));
    auto id = reinterpret_cast<mrb_int>(jso);
    std::string argv_str_buf, script;

    if (argc == 0) {  // if object
      mqrb_javascript_funcall_by_id(id, name.c_str());
    } else {  // if function
      make_jsargv(argv_str_buf, mrb, argv, argc);
      script = name + "(" + argv_str_buf + ")";
      mqrb_javascript_funcall_by_id(id, script.c_str());
    }

    return self;
  }

  //
  // mrb_js_var
  //
  mrb_value mrb_js_var(mrb_state* mrb, mrb_value self)
  {
    auto jso = static_cast<JavaScriptObject*>(DATA_PTR(self));
    auto id = reinterpret_cast<mrb_int>(jso);

    char buf[WORKING_STRING_BUFFER];
    mrb_int len;
    mqrb_javascript_get_value_by_id(id, buf, &len, WORKING_STRING_BUFFER);

    auto ai = mrb_gc_arena_save(mrb);
    mrb_value ret = mrb_str_new(mrb, buf, len);
    if (mrb_str_strlen(mrb, RSTRING(ret)) == 0) {
      return mrb_nil_value();
    }
    mrb_gc_arena_restore(mrb, ai);

    mrb_value mrb_mod_json_value = mrb_obj_value(mrb_module_get(mrb, "JSON"));

    return mrb_funcall(mrb, mrb_mod_json_value, "parse", 1, ret);
  }

  //
  // mrb_js_aryop
  //
  mrb_value mrb_js_aryop(mrb_state* mrb, mrb_value self)
  {
    mrb_value idx;
    mrb_get_args(mrb, "o", &idx);

    auto jso = static_cast<JavaScriptObject*>(DATA_PTR(self));
    auto id = reinterpret_cast<mrb_int>(jso);
    std::string script;
    std::string name = std::string(mrb_str_to_cstr(mrb, mrb_funcall(mrb, idx, "to_s", 0)));

    mqrb_javascript_inxop_by_id(id, name.c_str());

    return self;
  }

  //
  // mrb_js_set_value
  //
  mrb_value mrb_js_set_value(mrb_state* mrb, mrb_value self)
  {
    mrb_value idx;
    mrb_get_args(mrb, "o", &idx);

    auto jso = static_cast<JavaScriptObject*>(DATA_PTR(self));
    auto id = reinterpret_cast<mrb_int>(jso);
    std::string script;
    std::string name = std::string(mrb_str_to_cstr(mrb, mrb_funcall(mrb, idx, "to_s", 0)));

    mqrb_javascript_set_value_by_id(id, name.c_str());

    return self;
  }

  //
  // initialize
  //
  int initialize(mrb_state* mrb)
  {
    auto JavaScriptObject = mrb_define_class(mrb, "JavaScriptObject", mrb->object_class);

    mrb_define_method(mrb, JavaScriptObject, "initialize", mrb_initialize, MRB_ARGS_NONE());
    // TODO: initialize_copy

    // eval
    mrb_define_method(mrb, JavaScriptObject, "eval", mrb_js_eval, MRB_ARGS_REQ(1));

    // make quary of javascript
    mrb_define_method(mrb, JavaScriptObject, "js_q_function", mrb_js_q_function, MRB_ARGS_REQ(1) | MRB_ARGS_ANY());
    mrb_define_method(mrb, JavaScriptObject, "f", mrb_js_q_function, MRB_ARGS_REQ(1) | MRB_ARGS_ANY());  //  alias: mrb_js_q_object
    mrb_define_method(mrb, JavaScriptObject, "js_q_object", mrb_js_q_object, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, JavaScriptObject, "o", mrb_js_q_object, MRB_ARGS_REQ(1));  // alias: mrb_js_q_object
    mrb_define_method(mrb, JavaScriptObject, "exec", mrb_js_exec, MRB_ARGS_NONE());

    // get value of javascript
    mrb_define_method(mrb, JavaScriptObject, "var", mrb_js_var, MRB_ARGS_NONE());

    // operator
    mrb_define_method(mrb, JavaScriptObject, "[]", mrb_js_aryop, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, JavaScriptObject, "=", mrb_js_set_value, MRB_ARGS_REQ(1));

    // dynamic execute javascript
    mrb_define_method(mrb, JavaScriptObject, "method_missing", method_missing, MRB_ARGS_REQ(2));

    // for GC
    MRB_SET_INSTANCE_TT(JavaScriptObject, MRB_TT_DATA);

    return 0;
  }

}  // namespace java_script
}  // namespace mqrb