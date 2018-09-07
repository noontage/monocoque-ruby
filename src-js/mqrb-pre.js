function Mqrb() { }

Mqrb.initialize = function () {
  Mqrb._instance = new Object();
  Mqrb._queue = new Array();
}

Mqrb.eval = function (obj, script, call_type) {
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

Mqrb.create = function (id) {
  if (id === 0) {
    throw ('[ERROR] invalid Mqrb instance id.');
  }
  if (!Mqrb._instance) {
    throw ('[ERROR] Uninitialized Mqrb javascript library. please call mqrb_initialize() function.')
  }
  Mqrb._instance[id] = Function('return this')();  // root object
}

Mqrb.get = function (id) {
  return Mqrb._instance[id];
}

Mqrb.set = function (id, obj) {
  Mqrb._instance[id] = obj;
}

Mqrb.print_obj = function (id) {
  console.log(Mqrb._instance[id]);
}

Mqrb.push_queue = function (obj) {
  Mqrb._queue.push(obj);
}

Mqrb.shift_queue = function () {
  return Mqrb._queue.shift();
}