
#include "machine.h"

VALUE status_type( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  if( h_version(m) > 3 ) {
    return Qnil;
  }

  if( h_status_line_score(m) ) {
    return sym_score;
  }
  else if( h_status_line_time(m) ) {
    return sym_time;
  }

  return Qnil;
}

VALUE status_score( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  if( h_version(m) > 3 ) {
    return Qnil;
  }

  if( ! h_status_line_score(m) ) {
    return Qnil;
  }

  return INT2NUM(read_word( m, h_global_table(m) + 2 ));
}

VALUE status_turns( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  if( h_version(m) > 3 ) {
    return Qnil;
  }

  if( ! h_status_line_score(m) ) {
    return Qnil;
  }

  return INT2NUM(read_word( m, h_global_table(m) + 4 ));
}

VALUE status_hours( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  if( h_version(m) > 3 ) {
    return Qnil;
  }

  if( ! h_status_line_time(m) ) {
    return Qnil;
  }

  return INT2NUM(read_word( m, h_global_table(m) + 2 ));
}

VALUE status_minutes( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  if( h_version(m) > 3 ) {
    return Qnil;
  }

  if( ! h_status_line_time(m) ) {
    return Qnil;
  }

  return INT2NUM(read_word( m, h_global_table(m) + 4 ));
}

VALUE status_location( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  VALUE number;
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  if( h_version(m) > 3 ) {
    return Qnil;
  }

  number = UINT2NUM(read_word( m, h_global_table(m) ));

  return object_table_find( self, number );
}

