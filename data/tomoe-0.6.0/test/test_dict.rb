require 'tomoe-test-utils'

dict_module_type = TomoeTestUtils::Config.dict_module_type
read_only_dictionaries = %w(unihan)
if read_only_dictionaries.include?(dict_module_type)
  puts "#{dict_module_type} is read-only dictionary. skip tests."
else
  class TestDict < Test::Unit::TestCase
    include TomoeTestUtils

    def setup
      super
      setup_dict_file
      @original = Tomoe::DictXML.new("filename" => @dict_file.path)
    end

    def test_load
      make_temporary_dict(@original) do |dict|
        a = dict[@utf8]
        assert_not_nil(a)
        assert_equal(@utf8, a.utf8)
        if dict_module_type == "mysql"
          puts "MySQL backend doesn't support writing"
        else
          assert_equal(@strokes, a.writing.strokes)
        end
      end
    end

    def test_register_and_unregister
      make_temporary_dict(@original) do |dict|
        prev = dict.search(Tomoe::Query.new).collect do |cand|
          cand.char.utf8
        end

        char = Tomoe::Char.new
        char.utf8 = "か"
        assert_true(dict.register(char))
        assert_equal(char, dict["か"])
        assert_equal(["か", *prev].sort,
                     dict.search(Tomoe::Query.new).collect do |cand|
                       cand.char.utf8
                     end.sort)
        assert_true(dict.unregister("か"))
        assert_nil(dict["か"])

        assert_equal(prev,
                     dict.search(Tomoe::Query.new).collect do |cand|
                       cand.char.utf8
                     end)
      end
    end

    def assert_override
      make_temporary_dict(@original) do |dict|
        prev = dict.search(Tomoe::Query.new).collect do |cand|
          cand.char.utf8
        end

        utf8 = "か"
        first_n_strokes = 8
        second_n_strokes = 10

        first_char = Tomoe::Char.new
        first_char.utf8 = utf8
        first_char.n_strokes = first_n_strokes
        assert_true(dict.register(first_char))
        assert_equal(first_n_strokes, dict[utf8].n_strokes)
        assert_equal([utf8, *prev].sort,
                     dict.search(Tomoe::Query.new).collect do |cand|
                       cand.char.utf8
                     end.sort)

        second_char = Tomoe::Char.new
        second_char.utf8 = utf8
        second_char.n_strokes = second_n_strokes
        assert_true(dict.register(second_char))
        assert_not_equal(first_n_strokes, dict[utf8].n_strokes)
        assert_equal(second_n_strokes, dict[utf8].n_strokes)
        assert_equal([utf8, *prev].sort,
                     dict.search(Tomoe::Query.new).collect do |cand|
                       cand.char.utf8
                     end.sort)
      end
    end

    def test_pua_character_register_and_unregister
      make_temporary_dict(@original) do |dict|
        pua_start = Tomoe::Char::PRIVATE_USE_AREA_START

        assert_equal(pua_start, utf8_to_ucs4(dict.available_private_utf8))

        char1 = Tomoe::Char.new
        assert_true(dict.register(char1))
        assert_equal(ucs4_to_utf8(pua_start), char1.utf8)
        assert_equal(pua_start + 1, utf8_to_ucs4(dict.available_private_utf8))

        char2 = Tomoe::Char.new
        assert_true(dict.register(char2))
        assert_equal(ucs4_to_utf8(pua_start + 1), char2.utf8)
        assert_equal(pua_start + 2, utf8_to_ucs4(dict.available_private_utf8))

        assert_equal(char1, dict[ucs4_to_utf8(pua_start)])
        assert_equal(char2, dict[ucs4_to_utf8(pua_start + 1)])
        assert_true(dict.unregister(char1.utf8))
        assert_nil(dict[ucs4_to_utf8(pua_start)])
        assert_equal(char2, dict[ucs4_to_utf8(pua_start + 1)])

        char3 = Tomoe::Char.new
        assert_true(dict.register(char3))
        assert_equal(ucs4_to_utf8(pua_start + 2), char3.utf8)
        assert_equal(pua_start + 3, utf8_to_ucs4(dict.available_private_utf8))
      end
    end

    def test_pua_is_full
      make_temporary_dict(@original) do |dict|
        pua_start = Tomoe::Char::PRIVATE_USE_AREA_START
        pua_end = Tomoe::Char::PRIVATE_USE_AREA_END

        assert_equal(pua_start, utf8_to_ucs4(dict.available_private_utf8))
        pua_start.upto(pua_end - 1) do |i|
          char = Tomoe::Char.new
          char.utf8 = ucs4_to_utf8(i)
          assert_true(dict.register(char))
        end
        assert_equal(pua_end, utf8_to_ucs4(dict.available_private_utf8))

        char = Tomoe::Char.new
        char.utf8 = ucs4_to_utf8(pua_end)
        assert_true(dict.register(char))
        assert_nil(dict.available_private_utf8)

        char = Tomoe::Char.new
        assert_false(dict.register(char))
      end
    end

    def test_meta_data
      make_temporary_dict(@original) do |dict|
        char = Tomoe::Char.new
        char.utf8 = "か"
        assert_false(char.has_meta_data?)

        char["meta1"] = "value1"
        char["meta2"] = "value2"

        assert_true(dict.register(char))

        registered_char = dict["か"]
        assert_true(char.has_meta_data?)
        assert_equal("value1", registered_char["meta1"])
        assert_equal("value2", registered_char["meta2"])
      end
    end

    def test_reading_search
      make_temporary_dict(@original) do |dict|
        char = Tomoe::Char.new
        char.utf8 = "池"
        char.add_reading(Tomoe::Reading.new(Tomoe::Reading::JA_KUN, "いけ"))

        assert_true(dict.register(char))

        query = Tomoe::Query.new
        query.add_reading(Tomoe::Reading.new(Tomoe::Reading::JA_KUN, "いけ"))
        assert_equal(["池"],
                     dict.search(query).collect do |cand|
                       cand.char.utf8
                     end)
      end
    end

    def test_n_strokes_search
      make_temporary_dict(@original) do |dict|
        char = Tomoe::Char.new
        char.utf8 = "池"
        char.n_strokes = 6

        assert_true(dict.register(char))

        query = Tomoe::Query.new
        query.min_n_strokes = 6
        query.max_n_strokes = 6
        assert_equal(["池"],
                     dict.search(query).collect do |cand|
                       cand.char.utf8
                     end)

        query = Tomoe::Query.new
        query.min_n_strokes = 6
        query.max_n_strokes = 7
        assert_equal(["池"],
                     dict.search(query).collect do |cand|
                       cand.char.utf8
                     end)

        query = Tomoe::Query.new
        query.min_n_strokes = 5
        query.max_n_strokes = 6
        assert_equal(["池"],
                     dict.search(query).collect do |cand|
                       cand.char.utf8
                     end)

        query = Tomoe::Query.new
        query.min_n_strokes = 6
        assert_equal(["池"],
                     dict.search(query).collect do |cand|
                       cand.char.utf8
                     end)

        query = Tomoe::Query.new
        query.max_n_strokes = 6
        assert_equal(["あ", "池"].sort,
                     dict.search(query).collect do |cand|
                       cand.char.utf8
                     end.sort)
      end
    end

    def test_radical_search
      make_temporary_dict(@original) do |dict|
        sanzui = ucs4_to_utf8(27701) # さんずい
        char = Tomoe::Char.new
        char.utf8 = "池"
        char.add_radical(sanzui)

        assert_true(dict.register(char))

        query = Tomoe::Query.new
        query.add_radical(sanzui)
        assert_equal(["池"],
                     dict.search(query).collect do |cand|
                       cand.char.utf8
                     end)
      end
    end

    def test_variant_search
      make_temporary_dict(@original) do |dict|
        hashigo_daka = ucs4_to_utf8(39641) # はしご高
        char = Tomoe::Char.new
        char.utf8 = "高"
        char.variant = hashigo_daka

        assert_true(dict.register(char))

        query = Tomoe::Query.new
        query.variant = hashigo_daka
        assert_equal(["高"],
                     dict.search(query).collect do |cand|
                       cand.char.utf8
                     end)
      end
    end

    def test_utf8_search
      make_temporary_dict(@original) do |dict|
        char = Tomoe::Char.new
        char.utf8 = "か"

        assert_true(dict.register(char))

        query = Tomoe::Query.new
        query.utf8 = "か"
        assert_equal(["か"],
                     dict.search(query).collect do |cand|
                       cand.char.utf8
                     end)
      end
    end

    private
    def setup_strokes
      @strokes = [
                  [
                   [18, 19],
                   [83, 22]
                  ],
                  [
                   [49, 3],
                   [48, 67],
                   [60, 84],
                  ],
                  [
                   [74, 34],
                   [49, 76],
                   [27, 80],
                   [17, 68],
                   [28, 49],
                   [60, 46],
                   [80, 57],
                   [82, 74],
                   [76, 83],
                  ],
                 ]
    end

    def setup_dict_file
      @dict_file = Tempfile.new("tomoe-dict-xml")
      @utf8 = "あ"

      setup_strokes
      strokes_xml = "    <strokes>\n"
      @strokes.each do |stroke|
        strokes_xml << "      <stroke>\n"
        stroke.each do |x, y|
          strokes_xml << "        <point x=\"#{x}\" y=\"#{y}\"/>\n"
        end
        strokes_xml << "      </stroke>\n"
      end
      strokes_xml << "    </strokes>"

      @character_xml = <<-EOC
  <character>
    <utf8>#{@utf8}</utf8>
    <number-of-strokes>#{@strokes.size}</number-of-strokes>
#{strokes_xml}
  </character>
EOC

      @dict_content = <<-EOX
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE dictionary SYSTEM "tomoe-dict.dtd">
<dictionary>
#{@character_xml}
</dictionary>
EOX
      set_content(@dict_file, @dict_content)
    end

    def setup_est_draft_file
      @est_draft_file = Tempfile.new("tomoe-dict-est-draft")
      @est_draft_content = <<-EOC
@uri=font:#{@utf8}
utf8=#{@utf8}
n_strokes=#{@strokes.size}

#{@character_xml.collect {|line| "\t#{line}"}}
EOC
      set_content(@est_draft_file, @est_draft_content)
    end

    def set_content(file, content)
      file.open
      file.truncate(0)
      file.rewind
      file.print(content)
      file.close

      file.open
      assert_equal(content, file.read)
      file.close
    end

    def truncate_content(file)
      file.set_content("")
    end

    def content(file)
      file.open
      file.read
    ensure
      file.close
    end
  end
end
