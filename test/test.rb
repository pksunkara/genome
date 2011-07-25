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

  def diff
    self.split("\n").each do |line|
      if line[0]=='>'
        line
      elseif line[0]=='<'
        line
      end
    end
  end
end

class TestSuite
  def initalize(command)
    @command = command
  end

  def run(test_number)
    testdir = "t-" << test_number.to_s.rjust(4,'0')
    `#{@command} #{testdir}/program > #{testdir/output}`
    @diff = `diff #{testdir}/answer #{testdir}/output`
    if @diff.empty?
      puts "PASS".pass
    else
      puts "FAIL".fail
      puts @diff.diff
    end
  end
end
