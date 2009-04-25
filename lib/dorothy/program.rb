
class Z::Program
  attr_reader :memory

  class << self

    def []( file )
      @programs ||= {}

      file = File.expand_path( file )

      return @programs[file] if @programs.key?( file )

      @programs[file] = new( file )
    end

    def programs
      @programs ||= {}
    end
  end

  def _dump( depth=-1 )
    @filename
  end

  def self._load( filename )
    new( filename )
  end
end

