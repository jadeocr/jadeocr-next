require 'tomoe-test-utils'

class TestContext < Test::Unit::TestCase
  include TomoeTestUtils

  def setup
    super
    @user_dict_file = Tempfile.new("tomoe-user-dict")
    @user_dict_file.open
    @user_dict_file.puts(<<-EOD)
<?xml version ="1.0" encoding="UTF-8"?>
<!DOCTYPE dictionary SYSTEM "#{File.join(data_dir, 'tomoe-dict.dtd')}">
<dictionary name="User dictionary">
</dictionary>
EOD
    @user_dict_file.close

    @user_dict_config_file = Tempfile.new("tomoe-context")
    @user_dict_config_file.open
    @user_dict_config_file.puts(<<-EOC)
[config]
use-system-dictionaries = false
user-dictionary = user

[user-dictionary]
type = xml
file = #{@user_dict_file.path}
EOC
    @user_dict_config_file.close
  end

  TomoeTestUtils::Path.test_data_files.each do |file|
    base = File.basename(file)
    define_method("test_search_by_strokes_for_#{base}") do
      expected, writing = TomoeTestUtils::TestData.parse(file)
      query = Tomoe::Query.new
      query.writing = writing
      cands = context.search(query)
      actual = cands.collect {|cand| cand.char.utf8}

      assert_equal([base, expected], [base, actual])
    end
  end

  def test_search_by_reading
    query = Tomoe::Query.new
    query.add_reading(Tomoe::Reading.new(Tomoe::Reading::JA_KUN, "せい"))
    cands = context.search(query)
    assert_equal(["汐", "背", "脊", "醎", "鈼", "鮬", "鹹"].sort,
                 cands.collect {|cand| cand.char.utf8}.sort)
  end

  def test_user_dictionary
    context = Tomoe::Context.new()
    context.load_config(@user_dict_config_file.path)

    assert_equal([], context.search(Tomoe::Query.new))

    char = Tomoe::Char.new
    char.utf8 = "あ"
    assert_true(context.register(char))
    assert_equal([char.utf8],
                 context.search(Tomoe::Query.new).collect do |cand|
                   cand.char.utf8
                 end)

    assert_true(context.unregister(char.utf8))
    assert_equal([], context.search(Tomoe::Query.new))
  end

  def test_auto_assign_available_utf8_encoded_code_point
    context = Tomoe::Context.new()
    context.load_config(@user_dict_config_file.path)

    n_strokes = 8
    query = Tomoe::Query.new
    query.min_n_strokes = n_strokes
    query.max_n_strokes = n_strokes

    assert_equal([], context.search(query))

    char = Tomoe::Char.new
    char.n_strokes = n_strokes
    assert_true(context.register(char))
    assert_equal(ucs4_to_utf8(Tomoe::Char::PRIVATE_USE_AREA_START),
                 char.utf8)

    assert_equal([char.utf8],
                 context.search(query).collect do |cand|
                   cand.char.utf8
                 end)


    char2 = Tomoe::Char.new
    char2.n_strokes = n_strokes
    assert_true(context.register(char2))
    assert_equal(ucs4_to_utf8(Tomoe::Char::PRIVATE_USE_AREA_START + 1),
                 char2.utf8)

    assert_equal([char.utf8, char2.utf8].sort,
                 context.search(query).collect do |cand|
                   cand.char.utf8
                 end.sort)
  end

  def test_get_character_by_utf8
    context = Tomoe::Context.new()
    context.load_config(@user_dict_config_file.path)

    assert_nil(context[ucs4_to_utf8(Tomoe::Char::PRIVATE_USE_AREA_START)])

    char = Tomoe::Char.new
    char.n_strokes = 8
    assert_true(context.register(char))
    assert_equal(ucs4_to_utf8(Tomoe::Char::PRIVATE_USE_AREA_START),
                 char.utf8)

    retrieved_char = context[ucs4_to_utf8(Tomoe::Char::PRIVATE_USE_AREA_START)]
    assert_equal(char.n_strokes, retrieved_char.n_strokes)
  end

  private
  @@context = nil
  def context
    return @@context if @@context
    config = {
      "filename" => File.join(data_dir, "handwriting.xml"),
      "editable" => false,
    }
    dict = Tomoe::DictXML.new(config)
    recognizer = Tomoe::RecognizerSimple.new("dictionary" => dict)
    @@context = Tomoe::Context.new("recognizer" => recognizer)
    ensure_dict = "ensure_dict_#{dict_module_type}"
    __send__(ensure_dict) if respond_to?(ensure_dict, true)
    @@context.load_config(@config_file.path)
    @@context
  end
end
