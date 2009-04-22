
class Z::Program
  def self.new( file )
    @programs ||= {}

    file = File.expand_path( file )

    return @programs[file] if @programs.key?( file )

    @programs[file] = super
  end

  def _dump( depth=-1 )
    @filename
  end

  def self._load( filename )
    new( filename )
  end

  def dictionary
    @memory.dictionary
  end

end

