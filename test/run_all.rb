# Running all the tests using the test suite

load 'test/test.rb'

suite = TestSuite.new(File.expand_path('genome'), { 'prefix' => 'test/' })

Dir.glob('test/t-*').count.times do |i|
  suite.run(i+1)
end

suite.statsput
