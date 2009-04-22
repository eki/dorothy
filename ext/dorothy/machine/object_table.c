
#include "machine.h"


VALUE object_table_find( VALUE self, VALUE number ) {
  int n = NUM2UINT(number);
  VALUE memory = rb_iv_get( self, "@memory" );

  if( n == 0 ) {
    return Qnil;
  }

  return rb_funcall( Object, id_new, 2, memory, number );
}

VALUE object_table_each( VALUE self ) {
  int i = 1;
  zaddr min_prop_addr;
  VALUE memory = rb_iv_get( self, "@memory" );

  zmemory *m;
  Data_Get_Struct( memory, zmemory, m );

  min_prop_addr = m->dynamic_length;

  while( i < obj_max_objects(m) && obj_addr( m, i ) < min_prop_addr ) {
    if( obj_name_addr( m, i ) < min_prop_addr ) {
      min_prop_addr = obj_name_addr( m, i );
    }

    rb_yield( rb_funcall( Object, id_new, 2, memory, INT2NUM(i) ) );

    i++;
  }

  return self;
}

