require 'tomoe-test-utils'

class TestWriting < Test::Unit::TestCase
  include TomoeTestUtils

  def setup
    super
    @writing = Tomoe::Writing.new
    @strokes = [
                [[51, 29], [177, 41]],
                [[99, 65], [219, 77]],
                [[27, 131], [261, 131]],
                [[129, 17], [57, 203]],
                [[111, 71], [219, 173]],
                [[81, 161], [93, 281]],
                [[99, 167], [207, 167], [189, 245]],
                [[99, 227], [189, 227]],
                [[111, 257], [189, 245]],
              ]
    @strokes.each do |first_point, *rest_points|
      @writing.move_to(*first_point)
      rest_points.each do |x, y|
        @writing.line_to(x, y)
      end
    end
  end

  def test_each
    assert_equal(@strokes, @writing.strokes)
    assert_equal(@strokes, @writing.collect{|stroke| stroke})
  end

  def test_dump_xml
    xml = ""
    xml << "    <strokes>\n"
    @strokes.each do |stroke|
      xml << "      <stroke>\n"
      stroke.each do |x, y|
        xml << "        <point x=\"#{x}\" y=\"#{y}\"/>\n"
      end
      xml << "      </stroke>\n"
    end
    xml << "    </strokes>\n"
    assert_equal(xml, @writing.to_xml)
  end
end
