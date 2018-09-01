MRuby::Build.new do |conf|
  # Gets set by the VS command prompts.
  if ENV["VisualStudioVersion"] || ENV["VSINSTALLDIR"]
    toolchain :visualcpp
  else
    toolchain :gcc
  end

  conf.gembox "mnqrb"
end

MRuby::Build.new("host-debug") do |conf|
  # Gets set by the VS command prompts.
  if ENV["VisualStudioVersion"] || ENV["VSINSTALLDIR"]
    toolchain :visualcpp
  else
    toolchain :gcc
  end

  enable_debug

  # include the default GEMs
  conf.gembox "mnqrb"

  # C compiler settings
  conf.cc.defines = %w(MRB_ENABLE_DEBUG_HOOK)

  # Generate mruby debugger command (require mruby-eval)
  conf.gem :core => "mruby-bin-debugger"

  # bintest
  # conf.enable_bintest
end

MRuby::Build.new("test") do |conf|
  # Gets set by the VS command prompts.
  if ENV["VisualStudioVersion"] || ENV["VSINSTALLDIR"]
    toolchain :visualcpp
  else
    toolchain :gcc
  end

  enable_debug
  conf.enable_bintest
  conf.enable_test

  conf.gembox "mnqrb"
end

MRuby::CrossBuild.new("wasm") do |conf|
  toolchain :clang

  # C compiler settings
  conf.cc do |cc|
    cc.command = "emcc"
    cc.flags = [ENV["CFLAGS"] || %w(-O3)]
    #    cc.defines << %w(MRB_GC_ARENA_SIZE=100)
    #    cc.defines << %w(MRB_GC_FIXED_ARENA)
    cc.defines << %w(MRB_UTF8_STRING)
    cc.include_paths = ["#{root}/include"]
    cc.option_include_path = "-I%s"
    cc.option_define = "-D%s"
    cc.compile_options = "%{flags} -c %{infile} -s WASM=1 -o %{outfile}"
  end

  # Archiver settings
  conf.archiver do |archiver|
    archiver.command = "emcc"
    archiver.archive_options = "%{objs} -s WASM=1 -o %{outfile}"
  end

  # file extensions
  conf.exts do |exts|
    exts.object = ".bc"
    exts.library = ".bc"
  end

  #no executables
  conf.bins = []

  # gembox
  conf.gembox "mnqrb"
end

MRuby::CrossBuild.new("wasm-with-compiler") do |conf|
  toolchain :clang

  # C compiler settings
  conf.cc do |cc|
    cc.command = "emcc"
    cc.flags = [ENV["CFLAGS"] || %w(-O3)]
    #    cc.defines << %w(MRB_GC_ARENA_SIZE=100)
    #    cc.defines << %w(MRB_GC_FIXED_ARENA)
    cc.defines << %w(MRB_UTF8_STRING)
    cc.include_paths = ["#{root}/include"]
    cc.option_include_path = "-I%s"
    cc.option_define = "-D%s"
    cc.compile_options = "%{flags} -c %{infile} -s WASM=1 -o %{outfile}"
  end

  # Archiver settings
  conf.archiver do |archiver|
    archiver.command = "emcc"
    archiver.archive_options = "%{objs} -s WASM=1 -o %{outfile}"
  end

  # file extensions
  conf.exts do |exts|
    exts.object = ".bc"
    exts.library = ".bc"
  end

  #no executables
  conf.bins = []

  # gembox
  conf.gembox "mnqrb"
  conf.gem :core => "mruby-compiler"
end

MRuby::CrossBuild.new("wasm-debug") do |conf|
  toolchain :clang

  enable_debug

  # C compiler settings
  conf.cc do |cc|
    cc.command = "emcc"
    cc.flags = [ENV["CFLAGS"] || %w(-O3)]
    #    cc.defines << %w(MRB_GC_ARENA_SIZE=100)
    #    cc.defines << %w(MRB_GC_FIXED_ARENA)
    cc.defines << %w(MRB_UTF8_STRING)
    cc.include_paths = ["#{root}/include"]
    cc.option_include_path = "-I%s"
    cc.option_define = "-D%s"
    cc.compile_options = "%{flags} -c %{infile} -s WASM=1 -o %{outfile}"
  end

  # Archiver settings
  conf.archiver do |archiver|
    archiver.command = "emcc"
    archiver.archive_options = "%{objs} -s WASM=1 -o %{outfile}"
  end

  # file extensions
  conf.exts do |exts|
    exts.object = ".bc"
    exts.library = ".bc"
  end

  #no executables
  conf.bins = []

  # gembox
  conf.gembox "mnqrb"
end

# MRuby::Build.new("arm") do |conf|
#   toolchain :gcc
#
#   # C compiler settings
#   conf.cc do |cc|
#     cc.command = ENV["CC"] || "/usr/bin/arm-linux-gnueabihf-gcc"
#     cc.flags = [ENV["CFLAGS"] || %w(-O3 -Wall)]
#   end
#
#   # Linker settings
#   conf.linker do |linker|
#     linker.command = ENV["LD"] || "arm-linux-gnueabihf-gcc"
#   end
#
#   # Archiver settings
#   conf.archiver do |archiver|
#     archiver.command = ENV["AR"] || "arm-linux-gnueabihf-ar"
#   end
#
#   #no executables
#   conf.bins = []
#
#   # gembox
#   conf.gembox "mnqrb"
# end
