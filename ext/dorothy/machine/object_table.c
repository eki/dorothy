
#include "machine.h"


VALUE object_table_find( VALUE self, VALUE number ) {
  int n = NUM2UINT(number);
  VALUE memory = rb_iv_get( self, "@memory" );

  if( n == 0 ) {
    return Qnil;
  }

  return rb_funcall( Object, id_new, 2, memory, number );
}

