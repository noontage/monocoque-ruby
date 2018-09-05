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
option(USE_RUBY_COMPILER "Use Ruby Compiler" OFF)

# ============================================================================================
# set library directory
set(DIR_MRUBY ${PROJECT_SOURCE_DIR}/vendor/mruby)

# ============================================================================================
# set library path
set(LIB_MRUBY ${DIR_MRUBY}/build/${TARGET_PLATFORM}/lib/libmruby.a)
if(${TARGET_PLATFORM} STREQUAL "wasm")
  if(USE_RUBY_COMPILER)
    set(LIB_MRUBY ${DIR_MRUBY}/build/wasm-with-compiler/lib/libmruby.bc)
    add_definitions(-DUSE_RUBY_COMPILER)
  #elseif(${CMAKE_BUILD_TYPE} MATCHES Debug)
  #set(LIB_MRUBY ${DIR_MRUBY}/build/wasm-debug/lib/libmruby.bc)
  else()
    set(LIB_MRUBY ${DIR_MRUBY}/build/wasm/lib/libmruby.bc)
  endif()
endif()

# ============================================================================================
# configuration
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited configs" FORCE)
set(CMAKE_SUPPRESS_REGENERATION true)

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
add_executable(mqrb 
  ${SOURCES}
)

# ============================================================================================
# mruby
if(NOT EXISTS ${LIB_MRUBY})
  add_custom_target(mruby
    ruby minirake
    WORKING_DIRECTORY ${DIR_MRUBY} 
  )
  add_dependencies(mqrb 
    mruby
  )
endif()

# ============================================================================================
# target_lunk
target_link_libraries(mqrb 
    ${LIB_MRUBY}
)

# ============================================================================================
# options for wasm
if(${TARGET_PLATFORM} STREQUAL "wasm")
  set_target_properties(mqrb PROPERTIES
    LINK_FLAGS "-s WASM=1 -s MODULARIZE=1\
     --pre-js ${PROJECT_SOURCE_DIR}/src-js/mqrb-pre.js \
     --js-library ${PROJECT_SOURCE_DIR}/src-js/mqrb-lib.js \
     -s \"ASSERTIONS=2\" \
     -s \"DEMANGLE_SUPPORT=1\" \
     -s \"EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap','Pointer_stringify', 'UTF8ToString', 'writeArrayToMemory']\""
    OUTPUT_NAME ${MQRB_OUTPUT_NAME}
  )
  set(CMAKE_EXECUTABLE_SUFFIX ".js")
endif()
