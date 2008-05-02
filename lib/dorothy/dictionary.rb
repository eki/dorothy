
class Dictionary
  attr_reader :machine, :addr

  def initialize( machine )
    @machine, @addr = machine, machine.header.dictionary
  end

  def word_separators
    (1..num_word_separators).map do |i|
      machine.read_byte( addr + i ).chr
    end
  end

end

