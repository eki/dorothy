
#include "machine.h"

/*
 *  Write the parse table (at address pt) for the given text.  Use the given
 *  dictionary for parsing.  If the skip flag is set, don't write parse
 *  table entries for words that are not found in the dictionary.
 */

void write_parse_table( zmachine *zm, 
                        VALUE text, zaddr pt, zaddr dict, bool skip ) {

  VALUE dictionary;
  VALUE parsed;
  zbyte n;
  int i;

  if( dict == h_dictionary(zm) ) {
    dictionary = rb_funcall( zm->self, id_dictionary, 0 );
  }
  else {
    dictionary = rb_funcall( Dictionary, id_new, 2, zm->self, UINT2NUM(dict) );
  }


  n = read_byte( zm, pt );
  parsed = rb_funcall( dictionary, id_parse, 1, text );

  if( n > RARRAY(parsed)->len ) {
    n = RARRAY(parsed)->len;
  }

  write_byte( zm, pt + 1, n );

  for( i = 0; i < n; i++ ) {
    VALUE entry = rb_ary_entry( parsed, i );
    zaddr a = NUM2UINT(rb_ary_entry( entry, 0 ));
    zbyte num = NUM2UINT(rb_ary_entry( entry, 1 ));
    zbyte pos = NUM2UINT(rb_ary_entry( entry, 2 ));

    if( a == 0 || ! skip ) {
      write_word( zm, pt + 2 + i * 4, a );
      write_byte( zm, pt + 2 + i * 4 + 2, num );
      write_byte( zm, pt + 2 + i * 4 + 3, pos + text_buffer_offset(zm) );
    }
  }
}

void z_encode_text( zmachine *zm ) {
  runtime_error( "z_encode_text is not yet implemented!" );
}

/*
 * z_read, read a line of input and (in V5+) store the terminating key.
 *
 *      zargs[0] = address of text buffer
 *      zargs[1] = address of token buffer
 *      zargs[2] = timeout in tenths of a second (optional)
 *      zargs[3] = packed address of routine to be called on timeout
 *
 */

void z_read( zmachine *zm ) {
  zbyte max = read_byte( zm, zm->zargs[0] );
  zbyte size = read_byte( zm, zm->zargs[0] + 1 );

  VALUE line;

  int i;

  if( h_version(zm) < 5 ) {
    max -= 1;
    size = 0;
  }

  line = rb_funcall( keyboard(zm), id_read_line, 1, UINT2NUM(max - size) );

  for( i = 0; i < RSTRING(line)->len; i++ ) {
    zchar c = *(RSTRING(line)->ptr + i);

    write_byte( zm, zm->zargs[0] + text_buffer_offset(zm) + size + i, 
      translate_to_zscii( zm, c ) );
  }

  if( h_version(zm) < 5 ) {
    write_byte( zm, zm->zargs[0] + text_buffer_offset(zm) + size + i, 0 );
  }

  if( h_version(zm) > 4 ) {
    write_byte( zm, zm->zargs[0] + 1, i );
  }

  /* write the parse table */

  if( zm->zargs[1] != 0 ) {
    write_parse_table( zm, line, zm->zargs[1], h_dictionary(zm), 0 );
  }

  /* echo the input back to the output */

  print_cstr( zm, "> " );
  print_rstr( zm, line );
  print_cstr( zm, "\n" );

  /* store the terminating character (if required) */

  if( h_version(zm) > 4 ) {
    p_store( zm, translate_to_zscii( zm, 10 ) );
  }
}

/*
 * z_read_char, read and store a key.
 *
 *      zargs[0] = input device (must be 1)
 *      zargs[1] = timeout in tenths of a second (optional)
 *      zargs[2] = packed address of routine to be called on timeout
 *
 */

void z_read_char( zmachine *zm ) {
  zchar c = NUM2UINT(rb_funcall( keyboard(zm), id_read_char, 0 ));
  p_store( zm, translate_to_zscii( zm, c ) );
}

/*
 * z_tokenise, make a lexical analysis of a ZSCII string.
 *
 *      zargs[0] = address of string to analyze
 *      zargs[1] = address of token buffer
 *      zargs[2] = address of dictionary (optional)
 *      zargs[3] = set when unknown words cause empty slots (optional)
 *
 */

void z_tokenise( zmachine *zm ) {

  zaddr dict = h_dictionary(zm);
  bool skip = 0;

  VALUE text = machine_read_string_array( zm->self, 
    UINT2NUM(zm->zargs[0] + text_buffer_offset(zm)),
    UINT2NUM(read_byte( zm, zm->zargs[0] )) );

  rb_funcall( zm->self, rb_intern( "puts" ), 1, text );

  if( zm->zargc >= 3 ) {
    dict = zm->zargs[2];
  }

  if( zm->zargc == 4 ) {
    skip = zm->zargs[3];
  }

  write_parse_table( zm, text, zm->zargs[1], dict, skip );
}

