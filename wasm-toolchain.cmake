#
# F5Ruby
# for Webassembly
#
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_C_COMPILER   emcc)
set(CMAKE_CXX_COMPILER em++)
set(CMAKE_AR           emar)
set(CMAKE_RANLIB       emranlib)

set(TARGET_PLATFORM "wasm")