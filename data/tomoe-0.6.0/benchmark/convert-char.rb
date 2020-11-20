require "benchmark"

benchmark_dir = File.expand_path(File.dirname(__FILE__))
top_dir = File.expand_path(File.join(benchmark_dir, ".."))
test_dir = File.join(top_dir, "test")
$LOAD_PATH.unshift(File.join(top_dir, "bindings", "ruby", ".libs"))
$LOAD_PATH.unshift(File.join(top_dir, "bindings", "ruby"))
$LOAD_PATH.unshift(File.join(test_dir))

require "tomoe-test-utils"

xml = <<-XML
  <character>
    <utf8>あ</utf8>
    <strokes>
      <stroke>
        <point x="180" y="193"/>
        <point x="830" y="226"/>
      </stroke>
      <stroke>
        <point x="490" y="33"/>
        <point x="483" y="670"/>
        <point x="606" y="840"/>
      </stroke>
      <stroke>
        <point x="746" y="343"/>
        <point x="496" y="766"/>
        <point x="273" y="800"/>
        <point x="176" y="680"/>
        <point x="286" y="496"/>
        <point x="606" y="463"/>
        <point x="800" y="573"/>
        <point x="826" y="746"/>
        <point x="760" y="833"/>
      </stroke>
    </strokes>
  </character>
XML

n = 5000
Benchmark.bmbm do |x|
  x.report("XML -> TomoeChar (##{n})") {n.times {Tomoe::Char.new(xml)}}
  GC.start
  char = Tomoe::Char.new(xml)
  x.report("TomoeChar -> XML (##{n})") {n.times {char.to_xml}}
end
