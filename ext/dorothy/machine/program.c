
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
  char *fn = StringValuePtr( filename );
  FILE *fp;
  long i;
  zprogram *zp;

  Data_Get_Struct( self, zprogram, zp );

  fp = fopen( fn, "rb" );

  if( fp == NULL ) {
    rb_raise( rb_eRuntimeError, "Unable to open file: %s", fn );
  }

  zp->program = ALLOC_N( zbyte, 64 );

  if( fread( zp->program, 1, 64, fp ) != 64 ) {
    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading header: %s", fn );
  }

  /* Set the actual program length (don't trust the program header) */

  fseek( fp, 0, SEEK_END );
  zp->length = ftell( fp );
  fseek( fp, 0, SEEK_SET );

  /* Read the entire program into memory */

  REALLOC_N( zp->program, zbyte, zp->length );

  if( fread( zp->program, 1, zp->length, fp ) != zp->length ) {
    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading program: %s", fn );
  }

  fclose( fp );

  zp->checksum = 0;
  for( i = 64; i < zp->length; i++ ) {
    zp->checksum += zp->program[i];
  }

  zp->version = h_version( zp );
  zp->release = h_release( zp );

  for( i = 0; i < 6; i++ ) {
    zp->serial[i] = read_byte( zp, 0x12 + i );
  }
}

/*
 *  Free the memory used by a Machine.  
 */

void program_free( void *p ) {
  free( p );
}

/*
 *  Get the actual program length (may be different from the length reported
 *  by the header).
 */

VALUE program_length( VALUE self ) {
  zprogram *zp;

  Data_Get_Struct( self, zprogram, zp );

  return INT2NUM(zp->length);
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

