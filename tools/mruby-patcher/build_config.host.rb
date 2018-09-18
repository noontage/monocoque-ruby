#
# = set_mqrb_build_conf
#
def set_mqrb_build_conf(conf, gems = [])
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

  gems.each do |g|
    conf.gem g
  end
end

MRuby::Build.new do |conf|
  # Gets set by the VS command prompts.
  if ENV["VisualStudioVersion"] || ENV["VSINSTALLDIR"]
    toolchain :visualcpp
  else
    toolchain :gcc
  end

  set_mqrb_build_conf(conf)
end

MRuby::Build.new("host-debug") do |conf|
  # Gets set by the VS command prompts.
  if ENV["VisualStudioVersion"] || ENV["VSINSTALLDIR"]
    toolchain :visualcpp
  else
    toolchain :gcc
  end

  enable_debug

  # C compiler settings
  conf.cc.defines = %w(MRB_ENABLE_DEBUG_HOOK)

  set_mqrb_build_conf(conf, [:core => "mruby-bin-debugger"])
end
