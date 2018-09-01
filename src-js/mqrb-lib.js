if (typeof mergeInto !== undefined) {
    mergeInto(LibraryManager.library, {
        //
        // mqrb_jsf_exec_eval
        //
        mqrb_jsf_exec_eval: function (str) {
            var script = Pointer_stringify(str);
            mqrb_jsf_imp_eval(null, script);
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
        mqrb_jsf_new_copy: function (src, dest) {
            mqrb_jsf_imp_new(dest);
            mqrb_jsf_imp_set(dest, mqrb_jsf_imp_get(src))
        },
        //
        // mqrb_jsf_shift_global_queue
        //
        mqrb_jsf_shift_global_queue: function (id) {
            mqrb_jsf_imp_set(id, mqrb_jsf_imp_shift_queue());
        },
        //
        // mqrb_jsf_register_bytecode
        //
        mqrb_jsf_register_bytecode: function (ptr, size) {
            var bytecode = new Uint8Array(size);
            bytecode.set(module.HEAP8.subarray(ptr, ptr + size));
            console.log(bytecode);
            return mqrb_jsf_imp_register_bytecode(bytecode);
        },
        //
        // mqrb_jsf_print_obj
        //
        mqrb_jsf_print_obj: function (id) {
            mqrb_jsf_imp_print_obj(id);
        },
        //
        // mqrb_jsf_funcall
        //
        mqrb_jsf_funcall: function (id, str) {
            var script = Pointer_stringify(str);
            var obj = mqrb_jsf_imp_get(id);
            var ret = mqrb_jsf_imp_eval(obj, script, 0);
            mqrb_jsf_imp_set(id, ret);
        },
        //
        // mqrb_jsf_op_ary
        //
        mqrb_jsf_op_ary: function (id, str) {
            var script = Pointer_stringify(str);
            var obj = mqrb_jsf_imp_get(id);
            var ret = mqrb_jsf_imp_eval(obj, script, 1);
            mqrb_jsf_imp_set(id, ret);
        },
        //
        // mqrb_jsf_set_value
        //
        mqrb_jsf_set_value: function (id, str) {
            var script = Pointer_stringify(str);
            var obj = mqrb_jsf_imp_get(id);
            var ret = mqrb_jsf_imp_eval(obj, script, 2);
            mqrb_jsf_imp_set(id, ret);
        },
        //
        // mqrb_jsf_get_value
        //
        mqrb_jsf_get_value: function (id, buf, len, max_len) {
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

