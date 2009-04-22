
#include "machine.h"


VALUE object_name( VALUE self ) {
  int n = NUM2UINT(rb_iv_get( self, "@number" ));
  VALUE memory = rb_iv_get( self, "@memory" );

  zmemory *m;
  Data_Get_Struct( memory, zmemory, m );

  zaddr naddr = obj_name_addr( m, n ) + 1; /* skip the size byte */

  return memory_read_string( memory, LONG2NUM((long) naddr) );
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

