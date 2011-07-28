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
      'answer' => 'answer'
    }
    @options = @options.merge(options)
    puts @options
    puts ''
  end

  def run(testcase)
    start_time = Time.now
    testdir = (testcase.is_a?(String) ? testcase : "t-#{testcase.to_s.rjust(@options['rjust'],'0')}")

    if Dir.exist?("#{@options['prefix']}#{testdir}") && File.exist?("#{@options['prefix']}#{testdir}/#{@options['program']}")
      @input = File.exist?("#{@options['prefix']}#{testdir}/#{@options['input']}") ? " < #{@options['prefix']}#{testdir}/#{@options['input']}" : ""
      `#{@command} #{@options['prefix']}#{testdir}/#{@options['program']}#{@input} > #{@options['prefix']}#{testdir}/output`
      diff = `diff --unidirectional-new-file #{@options['prefix']}#{testdir}/#{@options['answer']} #{@options['prefix']}#{testdir}/output`
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
    puts "\nFinished in #{@stats['time'].round(6)} seconds.\n#{@stats['pass'] + @stats['fail']} tests. " << "#{@stats['pass']} passes".pass << ", " << "#{@stats['fail']} failures".fail << "."
  end
end
