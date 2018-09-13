MRuby::CrossBuild.new("wasm") do |conf|
  toolchain :clang

  enable_cxx_exception

  # C compiler settings
  conf.cc do |cc|
    cc.command = "emcc"
    cc.flags = [ENV["CFLAGS"] || %w(-O3)]
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
  conf.gem :core => "mruby-metaprog"
  conf.gem :core => "mruby-io"
  conf.gem :core => "mruby-pack"
  conf.gem :core => "mruby-sprintf"
  conf.gem :core => "mruby-print"
  conf.gem :core => "mruby-math"
  conf.gem :core => "mruby-time"
  conf.gem :core => "mruby-struct"
  conf.gem :core => "mruby-compar-ext"
  conf.gem :core => "mruby-enum-ext"
  conf.gem :core => "mruby-string-ext"
  conf.gem :core => "mruby-numeric-ext"
  conf.gem :core => "mruby-array-ext"
  conf.gem :core => "mruby-hash-ext"
  conf.gem :core => "mruby-range-ext"
  conf.gem :core => "mruby-proc-ext"
  conf.gem :core => "mruby-symbol-ext"
  conf.gem :core => "mruby-random"
  conf.gem :core => "mruby-object-ext"
  conf.gem :core => "mruby-objectspace"
  conf.gem :core => "mruby-fiber"
  conf.gem :core => "mruby-enumerator"
  conf.gem :core => "mruby-enum-lazy"
  conf.gem :core => "mruby-toplevel-ext"
  conf.gem :core => "mruby-kernel-ext"
  conf.gem :core => "mruby-class-ext"

  # gems of additional
  conf.gem :github => "mattn/mruby-json"
  conf.gem :github => "iij/mruby-regexp-pcre"
end

MRuby::CrossBuild.new("wasm-with-compiler") do |conf|
  toolchain :clang

  # C compiler settings
  conf.cc do |cc|
    cc.command = "emcc"
    cc.flags = [ENV["CFLAGS"] || %w(-O3)]
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
  conf.gem :core => "mruby-metaprog"
  conf.gem :core => "mruby-io"
  conf.gem :core => "mruby-pack"
  conf.gem :core => "mruby-sprintf"
  conf.gem :core => "mruby-print"
  conf.gem :core => "mruby-math"
  conf.gem :core => "mruby-time"
  conf.gem :core => "mruby-struct"
  conf.gem :core => "mruby-compar-ext"
  conf.gem :core => "mruby-enum-ext"
  conf.gem :core => "mruby-string-ext"
  conf.gem :core => "mruby-numeric-ext"
  conf.gem :core => "mruby-array-ext"
  conf.gem :core => "mruby-hash-ext"
  conf.gem :core => "mruby-range-ext"
  conf.gem :core => "mruby-proc-ext"
  conf.gem :core => "mruby-symbol-ext"
  conf.gem :core => "mruby-random"
  conf.gem :core => "mruby-object-ext"
  conf.gem :core => "mruby-objectspace"
  conf.gem :core => "mruby-fiber"
  conf.gem :core => "mruby-enumerator"
  conf.gem :core => "mruby-enum-lazy"
  conf.gem :core => "mruby-toplevel-ext"
  conf.gem :core => "mruby-kernel-ext"
  conf.gem :core => "mruby-class-ext"

  # gems of additional
  conf.gem :github => "mattn/mruby-json"
  conf.gem :github => "iij/mruby-regexp-pcre"
  conf.gem :core => "mruby-compiler"
end

MRuby::CrossBuild.new("wasm-debug") do |conf|
  toolchain :clang

  enable_cxx_exception
  enable_debug

  # C compiler settings
  conf.cc do |cc|
    cc.command = "emcc"
    cc.flags = [ENV["CFLAGS"] || %w(-g3)]
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
  conf.gem :core => "mruby-metaprog"
  conf.gem :core => "mruby-io"
  conf.gem :core => "mruby-pack"
  conf.gem :core => "mruby-sprintf"
  conf.gem :core => "mruby-print"
  conf.gem :core => "mruby-math"
  conf.gem :core => "mruby-time"
  conf.gem :core => "mruby-struct"
  conf.gem :core => "mruby-compar-ext"
  conf.gem :core => "mruby-enum-ext"
  conf.gem :core => "mruby-string-ext"
  conf.gem :core => "mruby-numeric-ext"
  conf.gem :core => "mruby-array-ext"
  conf.gem :core => "mruby-hash-ext"
  conf.gem :core => "mruby-range-ext"
  conf.gem :core => "mruby-proc-ext"
  conf.gem :core => "mruby-symbol-ext"
  conf.gem :core => "mruby-random"
  conf.gem :core => "mruby-object-ext"
  conf.gem :core => "mruby-objectspace"
  conf.gem :core => "mruby-fiber"
  conf.gem :core => "mruby-enumerator"
  conf.gem :core => "mruby-enum-lazy"
  conf.gem :core => "mruby-toplevel-ext"
  conf.gem :core => "mruby-kernel-ext"
  conf.gem :core => "mruby-class-ext"

  # gems of additional
  conf.gem :github => "mattn/mruby-json"
  conf.gem :github => "iij/mruby-regexp-pcre"
end

MRuby::CrossBuild.new("wasm-with-compiler-debug") do |conf|
  toolchain :clang

  enable_cxx_exception
  enable_debug

  # C compiler settings
  conf.cc do |cc|
    cc.command = "emcc"
    cc.flags = [ENV["CFLAGS"] || %w(-O3)]
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
  conf.gem :core => "mruby-metaprog"
  conf.gem :core => "mruby-io"
  conf.gem :core => "mruby-pack"
  conf.gem :core => "mruby-sprintf"
  conf.gem :core => "mruby-print"
  conf.gem :core => "mruby-math"
  conf.gem :core => "mruby-time"
  conf.gem :core => "mruby-struct"
  conf.gem :core => "mruby-compar-ext"
  conf.gem :core => "mruby-enum-ext"
  conf.gem :core => "mruby-string-ext"
  conf.gem :core => "mruby-numeric-ext"
  conf.gem :core => "mruby-array-ext"
  conf.gem :core => "mruby-hash-ext"
  conf.gem :core => "mruby-range-ext"
  conf.gem :core => "mruby-proc-ext"
  conf.gem :core => "mruby-symbol-ext"
  conf.gem :core => "mruby-random"
  conf.gem :core => "mruby-object-ext"
  conf.gem :core => "mruby-objectspace"
  conf.gem :core => "mruby-fiber"
  conf.gem :core => "mruby-enumerator"
  conf.gem :core => "mruby-enum-lazy"
  conf.gem :core => "mruby-toplevel-ext"
  conf.gem :core => "mruby-kernel-ext"
  conf.gem :core => "mruby-class-ext"

  # gems of additional
  conf.gem :github => "mattn/mruby-json"
  conf.gem :github => "iij/mruby-regexp-pcre"
end
