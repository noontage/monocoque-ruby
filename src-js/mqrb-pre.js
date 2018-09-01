var mqrb_data_obj = new Object();

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