require 'tomoe-test-utils'

class TestChar < Test::Unit::TestCase
  include TomoeTestUtils

  def test_dump_without_data
    char = Tomoe::Char.new
    assert_equal("", char.to_xml)
  end

  def test_dump_with_utf8
    char = Tomoe::Char.new
    char.utf8 = "あ"
    xml = "  <character>\n"
    xml << "    <utf8>あ</utf8>\n"
    xml << "  </character>\n"
    assert_equal(xml, char.to_xml)
  end

  def test_dump_with_variant
    hashigo_daka = ucs4_to_utf8(39641) # はしご高
    char = Tomoe::Char.new
    char.utf8 = "高"
    char.variant = hashigo_daka
    xml = "  <character>\n"
    xml << "    <utf8>高</utf8>\n"
    xml << "    <variant>#{hashigo_daka}</variant>\n"
    xml << "  </character>\n"
    assert_equal(xml, char.to_xml)
  end

  def test_dump_with_radicals
    sanzui = ucs4_to_utf8(27701) # さんずい
    char = Tomoe::Char.new
    char.utf8 = "池"
    char.add_radical(sanzui)
    xml = "  <character>\n"
    xml << "    <utf8>池</utf8>\n"
    xml << "    <radicals>\n"
    xml << "      <radical>#{sanzui}</radical>\n"
    xml << "    </radicals>\n"
    xml << "  </character>\n"
    assert_equal(xml, char.to_xml)
  end

  def test_load_from_dumped_xml
    char = Tomoe::Char.new
    char.utf8 = "あ"
    new_char = Tomoe::Char.new(char.to_xml)
    assert_equal(char.utf8, new_char.utf8)
  end

  def test_load_from_dumped_xml_with_variant
    hashigo_daka = ucs4_to_utf8(39641) # はしご高
    char = Tomoe::Char.new
    char.utf8 = "高"
    char.variant = hashigo_daka

    new_char = Tomoe::Char.new(char.to_xml)
    assert_equal(char.utf8, new_char.utf8)
    assert_equal(hashigo_daka, new_char.variant)
  end

  def test_load_from_dumped_xml_with_radicals
    sanzui = ucs4_to_utf8(27701) # さんずい
    char = Tomoe::Char.new
    char.utf8 = "池"
    char.add_radical(sanzui)

    new_char = Tomoe::Char.new(char.to_xml)
    assert_equal(char.utf8, new_char.utf8)
    assert_equal([sanzui], new_char.radicals)
  end

  def test_load_from_dumped_xml_with_n_strokes
    utf8 = "a"
    n_strokes = 5

    char = Tomoe::Char.new
    char.utf8 = utf8
    assert_equal(-1, char.n_strokes)
    char.n_strokes = n_strokes
    assert_equal(n_strokes, char.n_strokes)

    new_char = Tomoe::Char.new(char.to_xml)
    assert_equal(utf8, new_char.utf8)
    assert_equal(n_strokes, new_char.n_strokes)
  end

  def test_n_strokes_accessor
    char = Tomoe::Char.new
    assert_equal(-1, char.n_strokes)

    char.n_strokes = 5
    assert_equal(5, char.n_strokes)
  end

  def test_variant_accessor
    variant = "異"

    char = Tomoe::Char.new
    assert_nil(char.variant)

    char.variant = variant
    assert_equal(variant, char.variant)
  end

  def test_writing_accessor
    strokes = [[[0, 0], [10, 10]]]
    writing = Tomoe::Writing.new
    writing.move_to(*strokes[0][0])
    writing.line_to(*strokes[0][1])
    assert_equal(strokes, writing.strokes)

    char = Tomoe::Char.new
    assert_nil(char.writing)

    char.writing = writing
    assert_equal(strokes, char.writing.strokes)
  end

  def test_reading_accessor
    reading_a = Tomoe::Reading.new(Tomoe::Reading::JA_ON, "あ")

    reading_kai = Tomoe::Reading.new(Tomoe::Reading::JA_KUN, "カイ")

    char = Tomoe::Char.new
    assert_equal([], char.readings)

    char.add_reading(reading_a)
    assert_equal([reading_a.reading].sort,
                 char.readings.collect do |reading|
                   reading.reading
                 end.sort)

    char.add_reading(reading_kai)
    assert_equal([reading_a.reading, reading_kai.reading].sort,
                 char.readings.collect do |reading|
                   reading.reading
                 end.sort)
  end

  def test_radical_accessor
    radical1 = "木"
    radical2 = "水"

    char = Tomoe::Char.new
    assert_equal([], char.radicals)

    char.add_radical(radical1)
    assert_equal([radical1], char.radicals.sort)

    char.add_radical(radical2)
    assert_equal([radical1, radical2].sort, char.radicals.sort)
  end

  def test_comparable
    char1 = Tomoe::Char.new
    char1.utf8 = "あ"
    char2 = Tomoe::Char.new
    char2.utf8 = "あ"
    assert_equal(char1, char2)
  end
end
