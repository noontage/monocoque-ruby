#
# Monocoque Ruby
#
# ============================================================================================
cmake_minimum_required(VERSION 3.0.0)
project(mqrb VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(MQRB_OUTPUT_NAME "mqrb-core")

# ============================================================================================
# detect arch
if(NOT TARGET_PLATFORM)
  string(REGEX MATCH "emcc"                CCSTR_WASM ${CMAKE_CXX_COMPILER})
  string(REGEX MATCH "arm-linux-gnueabihf" CCSTR_ARM  ${CMAKE_CXX_COMPILER})

  if(CCSTR_WASM)
    set(TARGET_PLATFORM "wasm")
  elseif(CCSTR_ARM)
    set(TARGET_PLATFORM "arm")
  else()
    set(TARGET_PLATFORM "host")
  endif()
endif()

# ============================================================================================
# build options
option(USE_RUBY_COMPILER "Use Build and bundle ruby compiler."            OFF)
option(USE_BUILD_WASM    "[WASM] Build Webassembly."                       ON)
option(USE_EXPORT_ES6    "[WASM] Output ES6."                             OFF)


# ============================================================================================
# set library directory
set(DIR_MRUBY ${PROJECT_SOURCE_DIR}/vendor/mruby)
set(DIR_MRUBY_BUILD ${DIR_MRUBY}/build)

# ============================================================================================
# set library path
set(LIB_MRUBY ${DIR_MRUBY}/build/${TARGET_PLATFORM}/lib/libmruby.a)

# if host
if(${TARGET_PLATFORM} STREQUAL "host")
  add_definitions(-DBUILD_HOST)
endif()

# detect linking library
if(${TARGET_PLATFORM} STREQUAL "wasm")
  if(${CMAKE_BUILD_TYPE} MATCHES Release)
    if(USE_BUILD_WASM)
      if(USE_RUBY_COMPILER)
        message("-- mruby library: wasm-with-compiler")
        add_definitions(-DUSE_RUBY_COMPILER)
        set(LIB_MRUBY ${DIR_MRUBY_BUILD}/wasm-with-compiler/lib/libmruby.bc)
      else()
        message("-- mruby library: wasm")
        set(LIB_MRUBY ${DIR_MRUBY_BUILD}/wasm/lib/libmruby.bc)
      endif() # USE_RUBY_COMPILER
    else()
      if(USE_RUBY_COMPILER)
        message("-- mruby library: asm-with-compiler")
        add_definitions(-DUSE_RUBY_COMPILER)
        set(LIB_MRUBY ${DIR_MRUBY_BUILD}/asm-with-compiler/lib/libmruby.bc)
      else() # USE_RUBY_COMPILER
        message("-- mruby library: asm")
        set(LIB_MRUBY ${DIR_MRUBY_BUILD}/asm/lib/libmruby.bc)
      endif() # USE_RUBY_COMPILER
    endif() # USE_BUILD_WASM
  else()
    if(USE_RUBY_COMPILER)
      message("-- mruby library: wasm-with-compiler-debug")
      add_definitions(-DUSE_RUBY_COMPILER)
      set(LIB_MRUBY ${DIR_MRUBY_BUILD}/wasm-with-compiler-debug/lib/libmruby.bc)
    else()
      message("-- mruby library: wasm-debug")
      set(LIB_MRUBY ${DIR_MRUBY_BUILD}/wasm-debug/lib/libmruby.bc)
    endif() # USE_RUBY_COMPILER
  endif() # Release
endif() #if wasm

# ============================================================================================
# configuration
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited configs" FORCE)
set(CMAKE_SUPPRESS_REGENERATION true)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
  add_definitions(-DMQRB_DEBUG_MODE)
endif()

# ============================================================================================
# include
set(INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/include
  ${DIR_MRUBY}/include
)
include_directories(${INCLUDE_DIR})

# ============================================================================================
# sources
FILE(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
if(${TARGET_PLATFORM} STREQUAL "wasm")
  add_executable(mqrb 
   ${SOURCES}
  )
else()
  add_library(mqrb 
    ${SOURCES}
  )
endif()

# ============================================================================================
# mruby
if(NOT EXISTS ${LIB_MRUBY})
  add_custom_target(mruby
    COMMAND MRUBY_CONFIG=${PROJECT_SOURCE_DIR}/build/build_config.rb ruby minirake
    WORKING_DIRECTORY ${DIR_MRUBY} 
  )
  add_dependencies(mqrb 
    mruby
  )
endif()

# ============================================================================================
# target_lunk
message(${LIB_MRUBY})
target_link_libraries(mqrb 
    ${LIB_MRUBY}
)

# ============================================================================================
# options for wasm
if(${TARGET_PLATFORM} STREQUAL "wasm")
  set(WASM_COMPILE_OPTION "")
  
  # option: debug Assertions
  if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    string(APPEND WASM_COMPILE_OPTION "-s ASSERTIONS=2 ")
    string(APPEND WASM_COMPILE_OPTION "-s SAFE_HEAP=1 ")
  else()
    string(APPEND WASM_COMPILE_OPTION "-s ASSERTIONS=1 ")
  endif()

  # option: output ES6
  if(USE_EXPORT_ES6)
    string(APPEND WASM_COMPILE_OPTION "-s EXPORT_ES6=1 ")
  endif()

  # option: build wasm
  if(USE_BUILD_WASM)
    string(APPEND WASM_COMPILE_OPTION "-s WASM=1 ")
    string(APPEND WASM_COMPILE_OPTION "-s ALLOW_MEMORY_GROWTH=1 ")
  else()
    string(APPEND WASM_COMPILE_OPTION "-s WASM=0 ")
  endif()

  # memory
  string(APPEND WASM_COMPILE_OPTION "-s BINARYEN_MEM_MAX=2147418112 ")
  string(APPEND WASM_COMPILE_OPTION "-s TOTAL_MEMORY=64MB ")

  # option: othrer
  #string(APPEND WASM_COMPILE_OPTION "-s EXPORT_ES6=1 ")  
  string(APPEND WASM_COMPILE_OPTION "-s NO_FILESYSTEM=1 ")
  string(APPEND WASM_COMPILE_OPTION "-s DEMANGLE_SUPPORT=1 ")

  # set output properties
  set_target_properties(mqrb PROPERTIES
      LINK_FLAGS "-s MODULARIZE=1\
      ${WASM_COMPILE_OPTION} \
      --no-entry \
      --pre-js ${PROJECT_SOURCE_DIR}/src-js/mqrb-pre.js \
      --js-library ${PROJECT_SOURCE_DIR}/src-js/mqrb-lib.js \
      -s \"EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap','Pointer_stringify', 'UTF8ToString']\""
      OUTPUT_NAME ${MQRB_OUTPUT_NAME}
    )
  set(CMAKE_EXECUTABLE_SUFFIX ".js")
endif()