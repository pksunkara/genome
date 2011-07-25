# Ruby test suite for testing genome

class String
  def pass
    "\033#{self}"
  end

  def fail
    "\033#{self}"
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
