
class Z::Program
  attr_reader :dictionary

  def self.new( file )
    @programs ||= {}

    file = File.expand_path( file )

    return @programs[file] if @programs.key?( file )

    @programs[file] = super
  end

end

