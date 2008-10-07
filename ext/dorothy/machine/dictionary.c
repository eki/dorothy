
#include "machine.h"


VALUE dictionary_load( VALUE self ) {
  VALUE program = rb_iv_get( self, "@program" );
  zaddr dict = (zaddr) NUM2UINT(rb_iv_get( self, "@addr" ));
  zprogram *zp;

  Data_Get_Struct( program, zprogram, zp );

  zbyte entry_length = dict_entry_length( zp, dict );
  zword ne = dict_num_entries( zp, dict );
  zbyte ns = dict_num_word_separators( zp, dict );

  rb_iv_set( self, "@entry_length", UINT2NUM(entry_length) );

  zaddr a;
  zbyte zs[2] = { 5, 0 };
  zbyte *c = &zs[0];

  VALUE ary = rb_ary_new();

  /* Load the entries array */

  int i;

  for( i = 0; i < ne; i++ ) {
    a = dict + 1 + ns + 1 + 2 + entry_length * i;

    rb_ary_push( ary,
      rb_funcall( Entry, id_new, 2,
        program_read_string( program, UINT2NUM(a) ), UINT2NUM(a) ) );
  }

  rb_iv_set( self, "@entries", ary );

  /* Load the word separators array */

  ary = rb_ary_new();

  for( i = 0; i < ns; i++ ) {
    *c = read_byte( zp, dict + 1 + i );
    rb_ary_push( ary, rb_str_new2( c ) );
  }

  rb_iv_set( self, "@word_separators", ary );

  return self;
}

