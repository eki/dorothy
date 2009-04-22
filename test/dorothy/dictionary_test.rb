require 'test/unit'

require 'dorothy'

class TestDictionary < Test::Unit::TestCase

  F  = "#{File.dirname( __FILE__ )}/programs/read.z5"
  ZP = Z::Program.new( F )

  def test_addr
    assert_equal( 1317, ZP.dictionary.addr )   # bad to hardcode!
  end

  def test_entry_length
    assert_equal( 5, ZP.version )
    assert_equal( 9, ZP.dictionary.entry_length )
  end

  def test_word_separators
    assert_equal( ['.', ',', '"'], ZP.dictionary.word_separators )
  end

  def test_entries
    assert_equal( ["mailbox", "open"], ZP.dictionary.entries.map { |e| e.word })
  end

  def test_find
    assert( ZP.dictionary.find( "mailbox" )   != 0 ) 
    assert( ZP.dictionary.find( "open" )      != 0 )

    assert( ZP.dictionary.find( "mailboxes" ) != 0 )
    assert( ZP.dictionary.find( "opening" )   != 0 )

    assert( ZP.dictionary.find( "mail" ) == 0 ) 
    assert( ZP.dictionary.find( "pen" )  == 0)
    assert( ZP.dictionary.find( "ope" )  == 0)
    assert( ZP.dictionary.find( "o" )    == 0)
  end

  def test_entry_lookup
    assert_equal "mailbox", ZP.dictionary[ZP.dictionary.find( "mailbox" )].word
    assert_equal "open",    ZP.dictionary[ZP.dictionary.find( "open"    )].word
  end

  def test_parse
    rs = ZP.dictionary.parse( "open the mailbox" )

    assert_equal( [4, 3, 7], rs.map { |r| r[1] } )    # word length
    assert_equal( [0, 5, 9], rs.map { |r| r[2] } )    # starting positions

    assert( rs[0][0] != 0 )   # in dictionary
    assert( rs[1][0] == 0 )   # not in dictionary
    assert( rs[2][0] != 0 )

    assert_equal( [1333, 0, 1324], rs.map { |r| r[0] } )  # hardcoded addr!!
  end

  def test_cached
    assert_equal( ZP.dictionary.object_id, 
                  Z::Program.new( F).dictionary.object_id )
  end

end

