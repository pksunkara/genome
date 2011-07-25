# Running all the tests using the test suite

load 'test/test.rb'

suite = TestSuite.new(File.expand_path('genome'))

Dir.glob('test/t-*').each do |testdir|
  suite.run(testdir)
end

suite.statsput
