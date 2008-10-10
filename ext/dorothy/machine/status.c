
#include "machine.h"

VALUE status_type( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  if( h_version(zm) > 3 ) {
    return Qnil;
  }

  if( h_status_line_score(zm) ) {
    return sym_score;
  }
  else if( h_status_line_time(zm) ) {
    return sym_time;
  }

  return Qnil;
}

VALUE status_score( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  if( h_version(zm) > 3 ) {
    return Qnil;
  }

  if( ! h_status_line_score(zm) ) {
    return Qnil;
  }

  return INT2NUM(read_word( zm, h_global_table(zm) + 2 ));
}

VALUE status_turns( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  if( h_version(zm) > 3 ) {
    return Qnil;
  }

  if( ! h_status_line_score(zm) ) {
    return Qnil;
  }

  return INT2NUM(read_word( zm, h_global_table(zm) + 4 ));
}

VALUE status_hours( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  if( h_version(zm) > 3 ) {
    return Qnil;
  }

  if( ! h_status_line_time(zm) ) {
    return Qnil;
  }

  return INT2NUM(read_word( zm, h_global_table(zm) + 2 ));
}

VALUE status_minutes( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  if( h_version(zm) > 3 ) {
    return Qnil;
  }

  if( ! h_status_line_time(zm) ) {
    return Qnil;
  }

  return INT2NUM(read_word( zm, h_global_table(zm) + 4 ));
}

VALUE status_location_number( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  if( h_version(zm) > 3 ) {
    return Qnil;
  }

  return UINT2NUM(read_word( zm, h_global_table(zm) ));
}

VALUE status_location_name( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;
  zaddr naddr;
  zword n;

  Data_Get_Struct( machine, zmachine, zm );

  if( h_version(zm) > 3 ) {
    return Qnil;
  }

  n = read_word( zm, h_global_table(zm) );

  naddr = obj_name_addr( zm, n ) + 1; /* skip the size byte */

  return machine_read_string( zm->self, LONG2NUM((long) naddr) );
}

