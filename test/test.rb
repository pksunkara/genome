# Ruby test suite for testing genome

class String
  def code(code)
    "\033[#{code.to_s}m#{self}\033[0m"
  end

  def pass
    self.code(32)
  end

  def fail
    self.code(31)
  end

  def diffput
    puts "\tDiff:"
    self.split("\n").each do |line|
      if line[0]=='>'
        puts "\t\t+ #{line[2..-1]}".pass
      elsif line[0]=='<'
        puts "\t\t- #{line[2..-1]}".fail
      elsif line[0]!='-'
        puts "\t\t#{line}"
      end
    end
  end
end

class TestSuite
  def initialize(command, options = {})
    @command = command
    @stats = {'pass' => 0, 'fail' => 0, 'time' => 0}
    @options = {
      'prefix' => '',
      'rjust' => 3,
      'program' => 'program',
      'input' => 'input',
      'answer' => 'answer',
      'test_prefix' => 't-'
    }
    @options = @options.merge(options)
    puts @options
    puts ''
  end

  def run(testcase)
    start_time = Time.now
    testdir = (testcase.is_a?(String) ? testcase : @options['test_prefix'] + testcase.to_s.rjust(@options['rjust'],'0'))
    testcase = @options['prefix'] + testdir

    if Dir.exist?(testcase) && File.exist?("#{testcase}/#{@options['program']}")
      @input = File.exist?("#{testcase}/#{@options['input']}") ? " < #{testcase}/#{@options['input']}" : ""
      `#{@command} #{testcase}/#{@options['program']}#{@input} > #{testcase}/output`
      diff = `diff -N #{testcase}/#{@options['answer']} #{testcase}/output`
    else
      diff = "> NO TESTCASE"
    end

    print "#{testdir} "
    if diff.empty?
      puts "[PASS]".pass
      @stats['pass'] += 1
    else
      puts "[FAIL]".fail
      @stats['fail'] += 1
      diff.diffput
    end

    end_time = Time.now
    @stats['time'] += (end_time - start_time)
  end

  def statsput
    puts ""
    puts "Finished in #{@stats['time'].round(6)} seconds."
    print "#{@stats['pass'] + @stats['fail']} tests. "
    print "#{@stats['pass']} passes".pass << ", "
    puts "#{@stats['fail']} failures".fail << "."
  end
end
