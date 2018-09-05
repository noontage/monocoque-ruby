var mqrb_data_instance = new Object();
var mqrb_data_queue = new Array();
var mqrb_data_bytecode = new Array();

function mqrb_jsf_imp_eval(obj, script, call_type) {
  if (!call_type) {
    call_type = 0;
  }
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
    mqrb_data_instance[id] = Function('return this')();  // root object
  }
}

function mqrb_jsf_imp_get(id) {
  return mqrb_data_instance[id];
}

function mqrb_jsf_imp_set(id, obj) {
  mqrb_data_instance[id] = obj;
}

function mqrb_jsf_imp_print_obj(id) {
  console.log(mqrb_data_instance[id]);
}

function mqrb_jsf_imp_push_queue(obj) {
  mqrb_data_queue.push(obj);
}

function mqrb_jsf_imp_shift_queue() {
  return mqrb_data_queue.shift();
}

/*
function Mqrb() {
  this.initialize.apply(this, arguments);
}

Mqrb.prototype = Object.create(Object.prototype);
Mqrb.prototype.constructor = Mqrb;

Mqrb.prototype.initialize = function () {
  this._obj = Function('return this')();
  this._mruby_bytecodes = [];
}

Mqrb.prototype.register_bytecode = function (bytecode) {
  return this._mruby_bytecodes.push(bytecode);
}
*/