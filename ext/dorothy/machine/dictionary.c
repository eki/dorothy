
#include "machine.h"


VALUE dictionary_load( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zaddr dict = (zaddr) NUM2UINT(rb_iv_get( self, "@addr" ));
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  zbyte entry_length = dict_entry_length( zm, dict );
  zword ne = dict_num_entries( zm, dict );
  zbyte ns = dict_num_word_separators( zm, dict );

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
        machine_read_string( machine, UINT2NUM(a) ), UINT2NUM(a) ) );
  }

  rb_iv_set( self, "@entries", ary );

  /* Load the word separators array */

  ary = rb_ary_new();

  for( i = 0; i < ns; i++ ) {
    *c = read_byte( zm, dict + 1 + i );
    rb_ary_push( ary, rb_str_new2( c ) );
  }

  rb_iv_set( self, "@word_separators", ary );

  return self;
}

