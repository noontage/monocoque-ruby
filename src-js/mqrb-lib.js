if (typeof mergeInto !== undefined) {
    mergeInto(LibraryManager.library, {
        mqrb_javascript_eval: function (str) {
            var script = Pointer_stringify(str);
            eval(script);
        },
        mqrb_javascript_new: function (id) {
            mqrb_impl_javascript_new(id);
        },
        mqrb_javascript_new_copy: function (id, id_src) {
            mqrb_impl_javascript_new(id);
            mqrb_impl_javascript_set(id, mqrb_impl_javascript_get(id_src))
        },
        mqrb_javascript_funcall_by_id: function (id, str) {
            var script = Pointer_stringify(str);
            var obj = mqrb_impl_javascript_get(id);
            var ret = eval("obj." + script);
            mqrb_impl_javascript_set(id, ret);
        },
        mqrb_javascript_inxop_by_id: function (id, str) {
            var script = Pointer_stringify(str);
            var obj = mqrb_impl_javascript_get(id);
            var ret = eval("obj[" + script + "]");
            mqrb_impl_javascript_set(id, ret);
        },
        mqrb_javascript_set_value_by_id: function (id, str) {
            var script = Pointer_stringify(str);
            var obj = mqrb_impl_javascript_get(id);
            var ret = eval("obj=" + script);
            mqrb_impl_javascript_set(id, ret);
        },
        mqrb_javascript_get_value_by_id: function (id, buf, len, max_len) {
            var value = mqrb_impl_javascript_get(id);
            try {
                var data = JSON.stringify(value);
                if (data.length * 4 + 1 < max_len) {
                    stringToUTF8(data, buf, max_len);
                    setValue(len, lengthBytesUTF8(data), 'i32');
                }
            } catch (e) {
                setValue(buf, 0, 'i8');
                setValue(len, 0, 'i32');
            }
        },
        //mqrb_javascript_set_callback_table: function (cb_id) {
        //    setTimeout(function () { mqrb_call_proc_by_id(cb_id) }, 1000);
        //}
    });
}

