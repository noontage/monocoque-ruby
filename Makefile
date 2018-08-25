BUILD_DIR := build

# ===== all =====
.PHONY: all
all:
	mkdir -p $(BUILD_DIR)
	./tools/mruby-patcher/patch.sh
	cd $(BUILD_DIR); cmake ../
	cd $(BUILD_DIR); make #VERBOSE=1

# ===== wasm =====
.PHONY: wasm
wasm:
	mkdir -p $(BUILD_DIR)
	./tools/mruby-patcher/patch.sh
	cd $(BUILD_DIR); cmake ../ ../ -DCMAKE_TOOLCHAIN_FILE=../wasm-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
	cd $(BUILD_DIR); make

# ===== wasm-debug =====
.PHONY: wasmdb
wasmdb:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR); cmake ../ ../ -DCMAKE_TOOLCHAIN_FILE=../wasm-toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
	cd $(BUILD_DIR); make

# ===== demo =====
.PHONY: demo
demo:
	make wasm
	vendor/mruby/bin/mrbc demo/src/demo.rb
	cp -f $(BUILD_DIR)/mnqrb-core.wasm demo/webassembly
	cp -f $(BUILD_DIR)/mnqrb-core.js demo/webassembly
	cp -f src-js/mnqrb-lib.js demo/webassembly
	mv -f demo/src/demo.mrb demo/webassembly

# ===== clean =====
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# ===== clean-deep =====
.PHONY: clean-deep
clean-deep:
	make clean
	cd vendor/mruby; rake deep_clean