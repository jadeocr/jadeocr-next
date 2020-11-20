#!/usr/bin/env ruby

benchmark_dir = File.expand_path(File.dirname(__FILE__))
top_dir = File.expand_path(File.join(benchmark_dir, ".."))
test_dir = File.join(top_dir, "test")
$LOAD_PATH.unshift(File.join(top_dir, "ext", "ruby", ".libs"))
$LOAD_PATH.unshift(File.join(top_dir, "ext", "ruby"))
$LOAD_PATH.unshift(File.join(test_dir))

require "tomoe-spec-utils"

dictionary = TomoeSpecUtils::Path.dictionary
print "converting #{dictionary}..."
est_db = dictionary.sub(/\.xml$/, "")
xml_dict = Tomoe::DictXML.new("filename" => dictionary,
                              "editable" => false)
est_dict = Tomoe::DictEst.new("database" => est_db,
                              "editable" => true)
xml_dict.search(Tomoe::Query.new).each_with_index do |cand, i|
  est_dict.register(cand.char)
  if (i % 1000).zero? and !i.zero?
    print "#{i}."
    $stdout.flush
  end
end
puts "done."

puts "converted characters: #{est_dict.search(Tomoe::Query.new).size}"
