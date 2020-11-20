#!/usr/bin/env ruby

$VERBOSE = true

require "test/unit"

test_dir = File.expand_path(File.dirname(__FILE__))
top_dir = File.expand_path(File.join(test_dir, ".."))
$LOAD_PATH.unshift(File.join(top_dir, "bindings", "ruby", ".libs"))
$LOAD_PATH.unshift(File.join(top_dir, "bindings", "ruby"))
$LOAD_PATH.unshift(File.join(test_dir))

require 'tomoe-test-utils'

Dir.glob(File.join(test_dir, "test_*.rb")) do |file|
  require file.gsub(/\.rb/, "")
end

args = [File.dirname($0), ARGV.dup]
if Test::Unit::AutoRunner.respond_to?(:standalone?)
  args.unshift(false)
else
  args.unshift($0)
end
Test::Unit::AutoRunner.run(*args)
