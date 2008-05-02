
#include "machine.h"

/*
void dict_get_entry( zmachine *zm, zword i, zchar *entry ) {
  zaddr d = h_dictionary( zm );
  zbyte ns = dict_num_word_separators( zm );
  zword ne = dict_num_entries( zmachine *zm );
}
*/

VALUE dictionary_num_word_separators( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return INT2NUM(dict_num_word_separators( zm ));
}

VALUE dictionary_entry_length( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return INT2NUM(dict_entry_length( zm ));
}

VALUE dictionary_num_entries( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return INT2NUM(dict_num_entries( zm ));
}

VALUE dictionary_entries( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  zbyte entry_length = dict_entry_length( zm );
  zword ne = dict_num_entries( zm );
  zaddr d = h_dictionary( zm );
  zbyte ns = dict_num_word_separators( zm );
  zaddr a;

  VALUE ary = rb_ary_new();

  int i;

  for( i = 0; i < ne; i++ ) {
    a = d + 1 + ns + 1 + 2 + entry_length * i;
    printf( "about to call read_string with addr (%d)\n", a );
    rb_ary_push( ary, machine_read_string( machine, UINT2NUM(a) ) );
  }

  return ary;
}



