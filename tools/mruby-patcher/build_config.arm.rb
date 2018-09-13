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
