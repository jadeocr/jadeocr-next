#!/usr/bin/env ruby

base = File.expand_path(File.dirname(__FILE__))
require File.join(base, "init")

migrations = File.join(base, "migrate")
version = ARGV.shift
ActiveRecord::Migrator.migrate(migrations, version ? version.to_i : nil)
