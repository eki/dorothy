require 'test/unit'
require 'yaml'

require 'dorothy'

class TestPrograms < Test::Unit::TestCase

  PROGRAMS_DIR = "#{File.dirname( __FILE__ )}/programs"

  Dir["#{PROGRAMS_DIR}/*.inf"].each do |fn|

    zf = "#{PROGRAMS_DIR}/#{File.basename( fn, '.inf' )}.z5"
    tm = "test_#{File.basename( fn, '.inf' )}"

    output_yaml, reading_output_yaml = "", nil
    open( fn, "r" ) do |f|
      while s = f.gets
        if s =~ /^\! output:/
          reading_output_yaml = true
        elsif reading_output_yaml 
          if s =~ /^\! (.*)/
            output_yaml += $1 + "\n"
          else
            reading_output_yaml = false
          end
        end
      end
    end

    expected_output = YAML::load( output_yaml ) if output_yaml

    define_method( tm ) do
      __test_program( zf, expected_output )
    end

  end


  def __test_program( filename, expected_output )
    m = Machine.new( filename )
    
    m.step until m.finished?

    assert_equal( expected_output.length, m.output.length, 
                  "output doesn't have the expected length" )

    expected_output.length.times do |i|
      assert_equal( expected_output[i], m.output[i],
                    "output (#{i}) doesn't match expected output" )
    end
  end

end

