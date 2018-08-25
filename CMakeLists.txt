#
# F5Ruby
#
# ============================================================================================
cmake_minimum_required(VERSION 3.0.0)
project(mnqrb VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(MNQRB_OUTPUT_NAME "mnqrb-core")

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
#option(BUILD_HOST   "Build for Building host architecture"     ON)
#option(BUILD_ARM    "Build for Linux on ARM CPU architecture"  OFF)
#option(BUILD_WASM   "Build for Webassembly"                    ON)

# ============================================================================================
# set library directory
set(DIR_MRUBY ${PROJECT_SOURCE_DIR}/vendor/mruby)

# ============================================================================================
# set library path
set(LIB_MRUBY ${DIR_MRUBY}/build/${TARGET_PLATFORM}/lib/libmruby.a)
if(${TARGET_PLATFORM} STREQUAL "wasm")
  set(LIB_MRUBY ${DIR_MRUBY}/build/${TARGET_PLATFORM}/lib/libmruby.bc)
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
add_executable(mnqrb 
  ${SOURCES}
)

# ============================================================================================
# mruby
if(NOT EXISTS ${LIB_MRUBY})
  add_custom_target(mruby
    ruby minirake
    WORKING_DIRECTORY ${DIR_MRUBY} 
  )
  add_dependencies(mnqrb 
    mruby
  )
endif()

# ============================================================================================
# target_lunk
target_link_libraries(mnqrb 
    ${LIB_MRUBY}
)

# ============================================================================================
# options for wasm
if(${TARGET_PLATFORM} STREQUAL "wasm")
  set_target_properties(mnqrb PROPERTIES
    LINK_FLAGS "-s WASM=1 -s MODULARIZE=1 -s ASSERTIONS=1 --js-library  ${PROJECT_SOURCE_DIR}/src-js/mnqrb-lib.js -s \"EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']\""
    OUTPUT_NAME ${MNQRB_OUTPUT_NAME}
  )
  set(CMAKE_EXECUTABLE_SUFFIX ".js")
endif()
