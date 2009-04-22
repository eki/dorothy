
#include <stdio.h>
#include "machine.h"

/*
 *  Allocate memory for a Machine.
 */

VALUE program_alloc( VALUE klass ) {
  zprogram *zp = ALLOC( zprogram );
  VALUE obj = Data_Wrap_Struct( klass, 0, program_free, zp );
}

/*
 *  Initialize the machine.  Loads a story file into memory.
 */

VALUE program_initialize( VALUE self, VALUE filename ) {
  int i;
  zprogram *zp;
  zmemory *zm;

  VALUE memory = rb_funcall( Memory, id_new, 1, filename );

  Data_Get_Struct( self, zprogram, zp );
  Data_Get_Struct( memory, zmemory, zm );

  zp->m = zm;  

  zp->checksum = 0;
  for( i = 64; i < zp->m->length; i++ ) {
    zp->checksum += read_byte( zp, i );
  }

  zp->version = h_version( zp );
  zp->release = h_release( zp );

  for( i = 0; i < 6; i++ ) {
    zp->serial[i] = read_byte( zp, 0x12 + i );
  }

  rb_iv_set( self, "@memory", memory );

  rb_iv_set( self, "@filename", filename );
}

/*
 *  Free the memory used by a Machine.  
 */

void program_free( void *p ) {
  free( p );
}

/*
 *  Get the actual program checksum (may be different from the checksum 
 *  reported by the header).
 */

VALUE program_checksum( VALUE self ) {
  zprogram *zp;

  Data_Get_Struct( self, zprogram, zp );

  return INT2NUM(zp->checksum);
}

/*
 *  Get the program's z-machine version.
 */

VALUE program_version( VALUE self ) {
  zprogram *zp;

  Data_Get_Struct( self, zprogram, zp );

  return UINT2NUM(zp->version);
}

/*
 *  Get the program's release number.
 */

VALUE program_release( VALUE self ) {
  zprogram *zp;

  Data_Get_Struct( self, zprogram, zp );

  return UINT2NUM(zp->release);
}

/*
 *  Get the program's serial number.
 */

VALUE program_serial( VALUE self ) {
  zprogram *zp;

  Data_Get_Struct( self, zprogram, zp );

  VALUE serial;
  int i;
  char s[7] = { 0, 0, 0, 0, 0, 0, 0 };

  for( i = 0; i < 6; i++ ) {
    s[i] = zp->serial[i];
  }

  return rb_str_new2( s );
}

