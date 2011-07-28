# Running a single test case using the test suite

load 'test/test.rb'

suite = TestSuite.new(File.expand_path('genome'), {'prefix' => 'test/'})

suite.run(ARGV[0].to_i)
suite.statsput
