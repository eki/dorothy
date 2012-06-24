
class Z::Machine

  attr_reader :memory, :program, :screen, :trace, :keyboard, :status

  def run
    loop do
      break unless step
    end
  end

  def status?
    ! status.nil?
  end

  def scored?
    status? && status.type == :score
  end

  def timed?
    status? && status.type == :time
  end

  def output
    if (screen && screen.window && screen.window.output)
      screen.window.output
    else
      []
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
      @str.length > 0
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
      c = @str.slice!( 0, 1 )
      c.respond_to?( :ord ) ? c.ord : c[0]
    end

    def _dump( depth=-1 )
      Marshal.dump( [id, @str] )
    end

    def self._load( str )
      id, str = Marshal.load( str )
      stream = new( id )
      stream << str
      stream
    end
  end

end

