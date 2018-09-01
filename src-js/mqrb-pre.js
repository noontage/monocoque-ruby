var mqrb_data_obj = new Object();

function mqrb_jsf_imp_eval(obj, script, call_type = 0) {
  if (!obj) {
    obj = Function('return this')();
  }
  try {
    switch (call_type) {
      case 0:
        return eval("obj." + script);
      case 1:
        return eval("obj[" + script + "]");
      case 2:
        return eval("obj=" + script);
    }
  } catch (e) {
    console.warn("JavaScriptObjectError:");
    console.error(e);
    console.error(script);
  }
}

function mqrb_jsf_imp_new(id) {
  if (id !== 0) {
    mqrb_data_obj[id] = Function('return this')();  // root object
  }
}

function mqrb_jsf_imp_get(id) {
  return mqrb_data_obj[id];
}

function mqrb_jsf_imp_set(id, obj) {
  mqrb_data_obj[id] = obj;
}

function mqrb_jsf_imp_put_obj(id) {
  console.log(mqrb_data_obj[id]);
}