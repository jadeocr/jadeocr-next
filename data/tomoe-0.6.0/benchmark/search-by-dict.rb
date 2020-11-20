require "benchmark"

benchmark_dir = File.expand_path(File.dirname(__FILE__))
top_dir = File.expand_path(File.join(benchmark_dir, ".."))
test_dir = File.join(top_dir, "test")
$LOAD_PATH.unshift(File.join(top_dir, "bindings", "ruby", ".libs"))
$LOAD_PATH.unshift(File.join(top_dir, "bindings", "ruby"))
$LOAD_PATH.unshift(File.join(test_dir))

require "tomoe-test-utils"

n = 10
dict_type = ARGV.shift
tmp_dir = TomoeTestUtils::Path.tmp_dir
Benchmark.bmbm do |x|
  dictionary = TomoeSpecUtils::Path.dictionary
  case dict_type
  when "mysql"
    config = TomoeSpecUtils::Config.db_config("benchmark")
    dict = Tomoe::DictMySQL.new(config)
  when "est"
    database = dictionary.sub(/\.xml$/, '')
    dict = Tomoe::DictEst.new("name" => File.basename(dictionary),
                              "database" => database,
                              "editable" => false)
  when "svn"
    repos = File.join(tmp_dir, "svn.repos", File.basename(dictionary))
    FileUtils.mkdir_p(repos)
    repos_url = "file://#{repos}"
    wc = File.join(tmp_dir, "svn.wc", File.basename(dictionary))
    dict_file = File.join(wc, "dict.xml")

    `svnadmin create #{repos.dump}`
    `svn co #{repos_url.dump} #{wc.dump}`
    FileUtils.cp(dictionary, dict_file)
    `svn add #{dict_file.dump}`
    `svn ci -m '' #{wc.dump}`

    xml_dict = Tomoe::DictXML.new("filename" => dict_file, "editable" => true)
    dict = Tomoe::DictSvn.new("dictionary" => xml_dict, "working_copy" => wc)
  else
    dict = Tomoe::DictXML.new("filename" => dictionary, "editable" => false)
  end

  x.report("#{File.basename(dictionary)}: all") do
    query = Tomoe::Query.new
    expected = 13039
    actual = dict.search(query).size
    if actual != expected
      puts "expected: #{expected}"
      puts " but was: #{actual}"
    end
    n.times {dict.search(query)}
  end

  x.report("#{File.basename(dictionary)}: >= 5") do
    query = Tomoe::Query.new
    query.min_n_strokes = 10
    expected = 9889
    actual = dict.search(query).size
    if actual != expected
      puts "expected: #{expected}"
      puts " but was: #{actual}"
    end
    n.times {dict.search(query)}
  end

  x.report("#{File.basename(dictionary)}: readings") do
    query = Tomoe::Query.new
    query.add_reading(Tomoe::Reading.new(Tomoe::READING_JA_KUN, "せい"))
    expected = ["汐", "背", "脊"].sort
    actual =  dict.search(query).collect do |cand|
      cand.char.utf8
    end.sort
    if actual != expected
      $KCODE = "u"
      puts "expected: #{expected.inspect}"
      puts " but was: #{actual.inspect}"
    end
    n.times {dict.search(query)}
  end
end

FileUtils.rm_rf(tmp_dir)
