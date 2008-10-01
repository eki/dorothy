
class Machine

  attr_reader :header, :dictionary, :output, :trace, :keyboard

  def run
    loop do
      break unless step
    end
  end

  class InputStream
    attr_reader :id

    def initialize( id )
      @id, @str = id, ""
    end

    def <<( s )
      @str << s
    end

    def line_available?
      @str.index "\n"
    end

    def char_available?
      @str.length > 1
    end

    def read_line( max=nil )
      i = line_available?

      return ""  if i.nil?

      if max.nil? || i <= max
        s = @str.slice!( 0, i )
        t = @str.slice!( 0, 1 )
        s
      else
        @str.slice!( 0, max )
      end
    end

    def read_char
      @str.slice!( 0, 1 )[0]
    end
  end

end

