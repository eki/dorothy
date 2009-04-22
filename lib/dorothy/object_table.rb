
class Z::ObjectTable
  include Enumerable

  attr_reader :memory

  def initialize( memory )
    @memory = memory
  end
end

