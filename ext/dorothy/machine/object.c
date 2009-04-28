
#include "machine.h"


VALUE object_name( VALUE self ) {
  int n = NUM2UINT(rb_iv_get( self, "@number" ));
  VALUE memory = rb_iv_get( self, "@memory" );

  zaddr naddr;
  zbyte size;

  zmemory *m;
  Data_Get_Struct( memory, zmemory, m );

  naddr = obj_name_addr( m, n ) + 1; /* skip the size byte */
  size  = obj_name_size( m, n );

  if( size > 0 ) {
    return memory_read_string( memory, LONG2NUM((long) naddr) );
  }
  else {
    return rb_str_new2( "" );
  }
}

VALUE object_parent( VALUE self ) {
  int n = NUM2UINT(rb_iv_get( self, "@number" ));
  int o;
  VALUE memory = rb_iv_get( self, "@memory" );

  zmemory *m;
  Data_Get_Struct( memory, zmemory, m );

  o = obj_parent( m, n );

  if( o == 0 ) {
    return Qnil;
  }

  return rb_funcall( Object, id_new, 2, memory, UINT2NUM(o) );
}

VALUE object_child( VALUE self ) {
  int n = NUM2UINT(rb_iv_get( self, "@number" ));
  int o;
  VALUE memory = rb_iv_get( self, "@memory" );

  zmemory *m;
  Data_Get_Struct( memory, zmemory, m );

  o = obj_child( m, n );

  if( o == 0 ) {
    return Qnil;
  }

  return rb_funcall( Object, id_new, 2, memory, UINT2NUM(o) );
}

VALUE object_sibling( VALUE self ) {
  int n = NUM2UINT(rb_iv_get( self, "@number" ));
  int o;
  VALUE memory = rb_iv_get( self, "@memory" );

  zmemory *m;
  Data_Get_Struct( memory, zmemory, m );

  o = obj_sibling( m, n );

  if( o == 0 ) {
    return Qnil;
  }

  return rb_funcall( Object, id_new, 2, memory, UINT2NUM(o) );
}

VALUE object_attributes( VALUE self ) {
  int n = NUM2UINT(rb_iv_get( self, "@number" ));
  int i;
  VALUE memory = rb_iv_get( self, "@memory" );
  VALUE ary = rb_ary_new();

  zmemory *m;
  Data_Get_Struct( memory, zmemory, m );

  for( i = 0; i <= obj_max_attr( m ); i++ ) {
    if( obj_attr( m, n, i ) ) {
      rb_ary_push( ary, INT2NUM(i) );
    }
  }
  
  return ary;
}

