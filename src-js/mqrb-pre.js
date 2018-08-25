var mqrb_data_obj = new Object();

function mqrb_impl_javascript_new(id) {
  if (id !== 0) {
    mqrb_data_obj[id] = Function('return this')();
  }
}

function mqrb_impl_javascript_get(id) {
  return mqrb_data_obj[id];
}

function mqrb_impl_javascript_set(id, obj) {
  mqrb_data_obj[id] = obj;
}