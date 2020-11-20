#!/usr/bin/env ruby

ENV["TOMOE_ENV"] ||= "benchmark"

base = File.expand_path(File.dirname(__FILE__))
require File.join(base, "init")

ActiveRecord::Base.logger.level = Logger::INFO

def utf8_to_ucs4(utf8)
  if utf8
    TomoeTestUtils::Unicode.utf8_to_ucs4(utf8)
  else
    nil
  end
end

$LOAD_PATH.unshift(File.join(base, "lib"))

migrate = File.join(base, "migrate.rb")
print "purging DB..."
$stdout.flush
`#{migrate} 0`
`#{migrate}`
puts "done."

filename = TomoeTestUtils::Path.dictionary
dict = Tomoe::DictXML.new("filename" => filename,
                          "editable" => false)
config = TomoeTestUtils::Config.db_config.merge("editable" => true)
mysql_dict = Tomoe::DictMySQL.new(config)

cands = dict.search(Tomoe::Query.new)
puts "dict size: #{cands.size}"

print "converting..."
$stdout.flush
$KCODE = "u"
cands.each_with_index do |cand, i|
  mysql_dict.register(cand.char)
  if (i % 1000).zero? and !i.zero?
    print "#{i}."
    $stdout.flush
  end
end
puts "done."

puts "converted characters: #{Char.count}"
