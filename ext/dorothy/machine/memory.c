
#include "machine.h"


/*
 *  Allocate memory for a Machine.
 */

VALUE memory_alloc( VALUE klass ) {
  zmemory *zm = ALLOC( zmemory );
  VALUE obj = Data_Wrap_Struct( klass, 0, memory_free, zm );
}

/*
 *  Initialize the machine.  Loads a story file into memory.
 */

VALUE memory_initialize( VALUE self, VALUE filename ) {
  char *fn = StringValuePtr( filename );
  FILE *fp;
  long i;
  zmemory *zm;

  Data_Get_Struct( self, zmemory, zm );

  fp = fopen( fn, "rb" );

  if( fp == NULL ) {
    rb_raise( rb_eRuntimeError, "Unable to open file: %s", fn );
  }

  zm = ALLOC( zmemory );

  zm->self = self;

  zm->m = zm;
  zm->m_dynamic = ALLOC_N( zbyte, 64 );

  if( fread( zm->m_dynamic, 1, 64, fp ) != 64 ) {
    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading header: %s", fn );
  }

  /* Set the actual program length (don't trust the program header) */

  fseek( fp, 0, SEEK_END );
  zm->length = ftell( fp );
  fseek( fp, 0, SEEK_SET );

  /* How long are the dynamic and static parts of memory? */

  zm->dynamic_length = 64;  /* Temporary */

  zm->dynamic_length = h_static_memory(zm) - 1;
  zm->static_length = zm->length - zm->dynamic_length;

  /* Read the entire program into memory */

  REALLOC_N( zm->m_dynamic, zbyte, zm->dynamic_length );

  if( fread( zm->m_dynamic, 1, zm->dynamic_length, fp ) != 
      zm->dynamic_length ) {

    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading program: %s", fn );
  }

  zm->m_static = ALLOC_N( zbyte, zm->static_length );

  if( fread( zm->m_static, 1, zm->static_length, fp ) != zm->static_length ) {
    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading program: %s", fn );
  }

  fclose( fp );
}

/*
 *  Free the memory used by a Machine.  
 */

void memory_free( void *p ) {
  free( p );
}


VALUE memory_initialize_copy( VALUE self, VALUE original ) {
  zmemory *zm;
  zmemory *zmo;

  Data_Get_Struct( self, zmemory, zm );
  Data_Get_Struct( original, zmemory, zmo );

  zm->m = zm;
  zm->m_dynamic = ALLOC_N(zbyte, zmo->dynamic_length);
  MEMCPY( zm->m_dynamic, zmo->m_dynamic, zbyte, zmo->dynamic_length ); 

  zm->m_static = zmo->m_static;

  zm->length = zmo->length;
  zm->dynamic_length = zm->dynamic_length;
  zm->static_length = zm->static_length;

  return self;
}

/*
 *  This is a wrapper on write_byte.  It's for use by z_ops and provides
 *  error checking.  TODO:  improve error checking.
 */

void storeb( zmachine *zm, zaddr addr, zbyte value ) {
  if( addr >= h_static_memory(zm) ) {
    runtime_error( "store out of range" );
  }

  write_byte( zm, addr, value );
}

/*
 *  This is a wrapper on write_word.  It's for use by z_ops and provides
 *  error checking.  TODO:  improve error checking.
 */

void storew( zmachine *zm, zaddr addr, zword value ) {
  if( addr >= h_static_memory(zm) ) {
    runtime_error( "store out of range" );
  }

  write_word( zm, addr, value );
}

