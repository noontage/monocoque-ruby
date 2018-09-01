if (typeof mergeInto !== undefined) {
    mergeInto(LibraryManager.library, {
        //
        // mqrb_jsf_eval
        //
        mqrb_jsf_eval: function (str) {
            var script = Pointer_stringify(str);
            eval(script);
        },
        //
        // mqrb_jsf_new
        //
        mqrb_jsf_new: function (id) {
            mqrb_jsf_imp_new(id);
        },
        //
        // mqrb_jsf_new_copy
        //
        mqrb_jsf_new_copy: function (id, id_src) {
            mqrb_jsf_imp_new(id);
            mqrb_jsf_imp_set(id, mqrb_jsf_imp_get(id_src))
        },
        //
        // mqrb_jsf_put_obj
        //
        mqrb_jsf_put_obj: function (id) {
            mqrb_jsf_imp_put_obj(id);
        },
        //
        // mqrb_jsf_funcall_by_id
        //
        mqrb_jsf_funcall_by_id: function (id, str) {
            var script = Pointer_stringify(str);
            var obj = mqrb_jsf_imp_get(id);
            var ret = eval("obj." + script);
            mqrb_jsf_imp_set(id, ret);
        },
        //
        // mqrb_jsf_inxop_by_id
        //
        mqrb_jsf_inxop_by_id: function (id, str) {
            var script = Pointer_stringify(str);
            var obj = mqrb_jsf_imp_get(id);
            var ret = eval("obj[" + script + "]");
            mqrb_jsf_imp_set(id, ret);
        },
        //
        // mqrb_jsf_set_value_by_id
        //
        mqrb_jsf_set_value_by_id: function (id, str) {
            var script = Pointer_stringify(str);
            var obj = mqrb_jsf_imp_get(id);
            var ret = eval("obj=" + script);
            mqrb_jsf_imp_set(id, ret);
        },
        //
        // mqrb_jsf_get_value_by_id
        //
        mqrb_jsf_get_value_by_id: function (id, buf, len, max_len) {
            var value = mqrb_jsf_imp_get(id);
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
    });
}

