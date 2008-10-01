
class Dictionary
  attr_reader :machine, :addr, :entries, :entry_length, :word_separators

  private :load

  def initialize( machine, addr )
    @machine, @addr = machine, addr
    load
  end

  # Parse the given string against this dictionary.  The result is an array
  # of triples:
  #
  #   [[address of matching entry, 
  #     number of letters in word, 
  #     position in the string], ... ]
  #
  # The parse data is used by the z-machine to construct the parse tables
  # used by z_tokenise and z_read instructions.

  def parse( str )
    ws = word_separators
    tokens = str.scan( /[^#{ws}\s]+|[#{ws}]|\s+/ )
    
    a, cs = [], 0
   
    tokens.each do |t|
      a << [find( t ), t.length, cs] unless t =~ /^\s+$/
      cs += t.length 
    end 

    a
  end

  # Returns the address of the longest matching dictionary entry.  For example,
  # if the dictionary contains both "z" and "zork", a search for "zork" would
  # return the "zork" entry.  A search for "zor" would return the "z" entry.

  def find( w )
    entry = entries.select { |e| e =~ w }.max { |e1,e2| e1 <=> e2 }
    entry ? entry.addr : 0
  end

  # Lookup an entry by address.

  def []( addr )
    entries.find { |e| e.addr == addr }
  end

  class Entry
    include Comparable

    attr_reader :word, :addr

    def initialize( word, addr )
      @word, @addr = word, addr
    end

    def to_s
      @word
    end

    def inspect
      to_s
    end

    def =~( s )
      s =~ /^#{Regexp.escape( word )}/
    end

    def <=>( e )
      to_s <=> e.to_s
    end
  end

end

