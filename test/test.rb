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
      else
        puts "\t\t#{line}"
      end
    end
  end
end

class TestSuite
  def initialize(command, prefix = '')
    @command = command
    @stats = {'pass' => 0, 'fail' => 0, 'time' => 0}
    @prefix = prefix
  end

  def run(testcase)
    start_time = Time.now.to_i
    testdir = (testcase.is_a?(String) ? testcase : "t-#{testcase.to_s.rjust(3,'0')}")

    if Dir.exist?("#{@prefix}#{testdir}") && File.exist?("#{@prefix}#{testdir}/program") && File.exist?("#{@prefix}#{testdir}/answer")
      `#{@command} #{@prefix}#{testdir}/program > #{@prefix}#{testdir}/output`
      diff = `diff #{@prefix}#{testdir}/answer #{@prefix}#{testdir}/output`
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

    end_time = Time.now.to_i
    @stats['time'] += (end_time - start_time)
  end

  def statsput
    puts "\nFinished in #{@stats['time']} seconds.\n#{@stats['pass'] + @stats['fail']} tests. " << "#{@stats['pass']} passes".pass << ", " << "#{@stats['fail']} failures".fail << "."
  end
end
