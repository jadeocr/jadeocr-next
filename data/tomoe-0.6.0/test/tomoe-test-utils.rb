require 'tempfile'
require 'fileutils'
require 'yaml'
require 'uconv'

$KCODE = "u"

module TomoeTestUtils
  def self.included(base)
    base.class_eval do
      include Base
      include Path
      include Config
      include Dictionary
      include Unicode
      include Assertions
    end
  end

  module Base
    def setup
      super
    end

    def teardown
      super
    end
  end

  module Path
    module_function
    def base_dir
      File.expand_path(File.dirname(__FILE__))
    end

    def tmp_dir
      File.join(base_dir, "tmp")
    end

    def top_dir
      File.expand_path(File.join(base_dir, ".."))
    end

    def data_dir
      File.join(top_dir, "data")
    end

    def test_data_dir
      File.join(base_dir, "data")
    end

    def module_dir
      File.join(top_dir, "module")
    end

    def recognizer_dir
      File.join(module_dir, "recognizer", ".libs")
    end

    def dict_dir
      File.join(module_dir, "dict", ".libs")
    end

    def db_dir
      File.join(top_dir, "db")
    end

    def db_config_file
      File.join(db_dir, "config.yml")
    end

    def test_data_files
      Dir.glob(File.join(test_data_dir, "*.data"))
    end

    def dictionary
      File.join(data_dir, "kanjidic2.xml")
    end
  end

  module Config
    extend Path

    module_function
    def db_config_for_active_record(type=nil)
      YAML.load(File.read(db_config_file))[type || ENV["TOMOE_ENV"] || "test"]
    end

    def db_config(type=nil)
      config = db_config_for_active_record(type)
      config.delete("adapter")
      config.delete("encoding")
      config["user"] = config.delete("username") if config["username"]
      config
    end

    def setup
      super
      FileUtils.mkdir_p(tmp_dir)
      @config_file = make_config_file
    end

    def teardown
      super
      FileUtils.rm_rf(tmp_dir)
    end

    def dict_module_type
      ENV["TOMOE_DICT_MODULE"] || "xml"
    end

    def make_config_file(dict_type=nil)
      dict_type ||= dict_module_type
      name ||= "tomoe-#{dict_type}"
      config_file = Tempfile.new(name)
      config_file.open
      config_file.puts(<<-EOC)
[config]
use-system-dictionaries = false
EOC

      config_maker = "make_config_file_for_#{dict_type}"
      unless respond_to?(config_maker, true)
        raise "unknown dictionary type: #{dict_type}"
      end
      config_file.puts(send(config_maker))

      config_file.close
      config_file
    end

    def make_config_file_for_unihan
      <<-EOC
[unihan-dictionary]
type = unihan
EOC
    end

    def est_db
      File.join(tmp_dir, File.basename(dictionary).sub(/\.xml$/, ''))
    end

    def ensure_dict_est
      unless File.exists?(est_db)
        tmp_est_db = "#{est_db}.tmp"
        FileUtils.rm_rf(tmp_est_db)
        xml_dict = Tomoe::DictXML.new("filename" => dictionary,
                                      "editable" => false)
        est_dict = Tomoe::DictEst.new("database" => tmp_est_db,
                                      "editable" => true)
        xml_dict.search(Tomoe::Query.new).each_with_index do |cand, i|
          est_dict.register(cand.char)
        end
        est_dict.flush
        FileUtils.cp_r(tmp_est_db, est_db)
      end
    end

    def ensure_dict_mysql
      sql_purge("test")
      xml_dict = Tomoe::DictXML.new("filename" => dictionary,
                                    "editable" => false)
      mysql_dict = Tomoe::DictMySQL.new(db_config("test"))
      xml_dict.search(Tomoe::Query.new).each_with_index do |cand, i|
        mysql_dict.register(cand.char)
      end
    end

    def make_config_file_for_est
      dict_basename = File.basename(dictionary).sub(/\.xml$/, '')
      <<-EOC
[#{dict_basename}-dictionary]
type = est
name = #{dict_basename}
database = #{est_db}
EOC
    end

    def make_config_file_for_xml
      <<-EOC
[#{File.basename(dictionary)}-dictionary]
type = xml
file = #{dictionary}
EOC
    end

    def make_config_file_for_mysql
      config = <<-EOC
[mysql-dictionary]
type = mysql
EOC
      db_config.each do |key, value|
        config << "#{key} = #{value}\n"
      end
      config
    end
  end

  module Dictionary
    module_function
    def make_dict(dict_type=nil, config=nil)
      dict_type ||= dict_module_type
      dict_maker = "make_dict_#{dict_type}"
      unless respond_to?(dict_maker, true)
        raise "unknown dictionary type: #{dict_type}"
      end
      send(dict_maker, config)
    end

    def make_temporary_dict(original, dict_type=nil, config=nil, &block)
      dict_type ||= dict_module_type
      temporary_dict_maker = "make_temporary_dict_#{dict_type}"
      unless respond_to?(temporary_dict_maker, true)
        raise "unknown dictionary type: #{dict_type}"
      end
      send(temporary_dict_maker, config) do |dict|
        original.search(Tomoe::Query.new).each do |cand|
          dict.register(cand.char)
        end
        block.call(dict)
      end
    end

    def make_dict_unihan(config=nil)
      check_dict_module_availability("Unihan")
      Tomoe::DictUnihan.new(config || {})
    end

    def make_dict_xml(config=nil)
      check_dict_module_availability("XML")
      config ||= {}
      config = config.dup
      config["filename"] ||= dictionary
      Tomoe::DictXML.new(config)
    end

    def make_temporary_dict_xml(config=nil)
      check_dict_module_availability("XML")
      dict = nil
      begin
        tmp_dict_dir = File.join(tmp_dir, "dict")
        FileUtils.mkdir_p(tmp_dict_dir)
        dict_file = File.join(tmp_dict_dir, "dict.xml")
        dict = Tomoe::DictXML.new("filename" => dict_file, "editable" => true)
        yield dict
      ensure
        dict.flush if dict
        FileUtils.rm_rf(tmp_dict_dir)
      end
    end

    def make_dict_est(config=nil)
      check_dict_module_availability("Est")
      config ||= {}
      config = config.dup
      config["database"] ||= dictionary.sub(/\.xml/, '')
      config["editable"] = true unless config.has_key?("editable")
      Tomoe::DictEst.new(config)
    end

    def make_temporary_dict_est(config=nil)
      check_dict_module_availability("Est")
      begin
        tmp_dict_dir = File.join(tmp_dir, "est")
        yield Tomoe::DictEst.new("database" => tmp_dict_dir, "editable" => true)
      ensure
        FileUtils.rm_rf(tmp_dict_dir)
      end
    end

    def make_dict_mysql(config=nil)
      check_dict_module_availability("MySQL")
      config ||= db_config
      config = config.dup
      Tomoe::DictMySQL.new(config)
    end

    def make_temporary_dict_mysql(config=nil)
      check_dict_module_availability("MySQL")
      sql_purge("temp")
      yield Tomoe::DictMySQL.new(db_config("temp"))
    end

    def check_dict_module_availability(type)
      begin
        Tomoe.const_get("Dict#{type}")
      rescue NameError
        raise "Tomoe doesn't support the dictionary type: #{type}"
      end
    end

    def sql_migrate(type=nil, version=nil)
      migrate = File.join(db_dir, "migrate.rb")
      tomoe_env = ENV["TOMOE_ENV"]
      ENV["TOMOE_ENV"] = type if type
      unless `#{migrate} #{version}`
        message = "failed to migrate"
        message << " to #{version}" if version
        raise message
      end
    ensure
      ENV["TOMOE_ENV"] = tomoe_env
    end

    def sql_purge(type=nil)
      sql_migrate(type, 0)
      sql_migrate(type)
    end
  end

  module TestData
    module_function
    def parse(file)
      expected = nil
      writing = Tomoe::Writing.new
      File.open(file) do |f|
        expected = f.gets.split
        f.each do |line|
          next if /\A\s*\z/ =~ line
          begin
            first_point, *rest_points = line.split(/,/)
            numbered_first_point = numbers_to_point(first_point)
            writing.move_to(*numbered_first_point)
            rest_points.each do |point|
              writing.line_to(*numbers_to_point(point))
            end
          rescue ArgumentError
            raise "invalid format in #{file} at #{f.lineno}: #{line}"
          end
        end
      end
      [expected, writing]
    end

    def numbers_to_point(str)
      point = str.split.collect {|x| Integer(x)}
      raise ArgumentError if point.size != 2
      point
    end
  end

  module Unicode
    module_function
    def ucs4_to_utf8(ucs4)
      Uconv.u4tou8([ucs4].pack("I*"))
    end

    def utf8_to_ucs4(utf8)
      Uconv.u8tou4(utf8).unpack("I*")[0]
    end
  end

  module Assertions
    def assert_true(actual, *args)
      assert_equal(true, actual, *args)
    end

    def assert_false(actual, *args)
      assert_equal(false, actual, *args)
    end
  end
end

require 'tomoe'

Tomoe::Dict.default_module_dir = TomoeTestUtils::Path.dict_dir
Tomoe::Recognizer.default_module_dir = TomoeTestUtils::Path.recognizer_dir
