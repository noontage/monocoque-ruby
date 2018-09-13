BUILD_DIR := build

# ===== all =====
.PHONY: all
all:
	mkdir -p $(BUILD_DIR)
	./tools/mruby-patcher/patch.sh
	cd $(BUILD_DIR); cmake ../
	cd $(BUILD_DIR); make #VERBOSE=1

# ===== mruby =====
.PHONY: mruby
mruby:
	cd vendor/mruby/; ./minirake

# ===== wasm =====
.PHONY: wasm
wasm:
	mkdir -p $(BUILD_DIR)
	./tools/mruby-patcher/patch.sh
	cd $(BUILD_DIR); cmake ../ ../ -DCMAKE_TOOLCHAIN_FILE=../wasm-toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
	cd $(BUILD_DIR); make

# ===== wasm-production =====
.PHONY: wasm-production
wasm-production:
	mkdir -p $(BUILD_DIR)
	./tools/mruby-patcher/patch.sh
	cd $(BUILD_DIR); cmake ../ ../ -DCMAKE_TOOLCHAIN_FILE=../wasm-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
	cd $(BUILD_DIR); make

# ===== wasm-with-compiler =====
.PHONY: wasm-with-compiler
wasm-with-compiler:
	mkdir -p $(BUILD_DIR)
	./tools/mruby-patcher/patch.sh
	cd $(BUILD_DIR); cmake ../ ../ -DCMAKE_TOOLCHAIN_FILE=../wasm-toolchain.cmake -DUSE_RUBY_COMPILER=ON  -DCMAKE_BUILD_TYPE=Debug
	cd $(BUILD_DIR); make

# ===== wasm-with-compiler-production =====
.PHONY: wasm-with-compiler-production
wasm-with-compiler-production:
	mkdir -p $(BUILD_DIR)
	./tools/mruby-patcher/patch.sh
	cd $(BUILD_DIR); cmake ../ ../ -DCMAKE_TOOLCHAIN_FILE=../wasm-toolchain.cmake -DUSE_RUBY_COMPILER=ON  -DCMAKE_BUILD_TYPE=Release
	cd $(BUILD_DIR); make

# ===== demo =====
.PHONY: demo
demo:
	vendor/mruby/bin/mrbc demo/src/demo.rb
	make wasm-with-compiler-production
	cp -f $(BUILD_DIR)/mqrb-core.wasm demo/webassembly
	cp -f $(BUILD_DIR)/mqrb-core.js demo/webassembly
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
	cd vendor/mruby; rm -rf build
