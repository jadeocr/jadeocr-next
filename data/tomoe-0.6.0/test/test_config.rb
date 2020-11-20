require 'tomoe-test-utils'

class TestConfig < Test::Unit::TestCase
  include TomoeTestUtils

  def test_get_language
    file = config_file(<<-EOC)
[config]
language = ja
EOC
    config = Tomoe::Config.new(file.path)
    assert_equal(["ja"], config.languages)
  end

  def test_get_languages
    file = config_file(<<-EOC)
[config]
languages = ja
EOC
    config = Tomoe::Config.new(file.path)
    assert_equal(["ja"], config.languages)

    file = config_file(<<-EOC)
[config]
languages = ja;en;fr
EOC
    config = Tomoe::Config.new(file.path)
    assert_equal(['ja', 'en', 'fr'], config.languages)
  end

  def test_language_and_languages
    file = config_file(<<-EOC)
[config]
language = ja
languages = en;ja;fr
EOC
    config = Tomoe::Config.new(file.path)
    assert_equal(["ja"], config.languages)
  end

  private
  def config_file(contents)
    config_file = Tempfile.new("tomoe-context")
    config_file.open
    config_file.puts(contents)
    config_file.close
    config_file
  end
end
