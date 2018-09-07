if (typeof mergeInto !== undefined) {
    mergeInto(LibraryManager.library, {
        //
        // mqrb_jsf_initialize
        //
        mqrb_jsf_initialize: function () {
            Mqrb.initialize();
        },
        //
        // mqrb_jsf_exec_eval
        //
        mqrb_jsf_exec_eval: function (str) {
            var script = Pointer_stringify(str);
            Mqrb.eval(null, script);
        },
        //
        // mqrb_jsf_new
        //
        mqrb_jsf_new: function (id) {
            Mqrb.create(id);
        },
        //
        // mqrb_jsf_new_copy
        //
        mqrb_jsf_new_copy: function (src, dest) {
            Mqrb.create(dest);
            Mqrb.set(dest, Mqrb.get(src))
        },
        //
        // mqrb_jsf_print_obj
        //
        mqrb_jsf_print_obj: function (id) {
            Mqrb.print_obj(id);
        },
        //
        // mqrb_jsf_shift_global_queue
        //
        mqrb_jsf_shift_global_queue: function (id) {
            Mqrb.set(id, Mqrb.shift_queue());
        },
        //
        // mqrb_jsf_funcall
        //
        mqrb_jsf_funcall: function (id, str) {
            var script = Pointer_stringify(str);
            var obj = Mqrb.get(id);
            Mqrb.set(id, Mqrb.eval(obj, script, 0));
        },
        //
        // mqrb_jsf_op_ary
        //
        mqrb_jsf_op_ary: function (id, str) {
            var script = Pointer_stringify(str);
            var obj = Mqrb.get(id);
            Mqrb.set(id, Mqrb.eval(obj, script, 1));
        },
        //
        // mqrb_jsf_set_value
        //
        mqrb_jsf_set_value: function (id, str) {
            var script = Pointer_stringify(str);
            var obj = Mqrb.get(id);
            Mqrb.set(id, Mqrb.eval(obj, script, 2));
        },
        //
        // mqrb_jsf_get_value
        //
        mqrb_jsf_get_value: function (id, buf, len, max_len) {
            var value = Mqrb.get(id);
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

