
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

  zp->m = ALLOC( zmemory );

  zp->m->m_dynamic = ALLOC_N( zbyte, 64 );

  if( fread( zp->m->m_dynamic, 1, 64, fp ) != 64 ) {
    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading header: %s", fn );
  }

  /* Set the actual program length (don't trust the program header) */

  fseek( fp, 0, SEEK_END );
  zp->m->length = ftell( fp );
  fseek( fp, 0, SEEK_SET );

  /* How long are the dynamic and static parts of memory? */

  zp->m->dynamic_length = 64;  /* Temporary */

  zp->m->dynamic_length = h_static_memory(zp) - 1;
  zp->m->static_length = zp->m->length - zp->m->dynamic_length;

  /* Read the entire program into memory */

  REALLOC_N( zp->m->m_dynamic, zbyte, zp->m->dynamic_length );

  if( fread( zp->m->m_dynamic, 1, zp->m->dynamic_length, fp ) != 
      zp->m->dynamic_length ) {

    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading program: %s", fn );
  }

  zp->m->m_static = ALLOC_N( zbyte, zp->m->static_length );

  if( fread( zp->m->m_static, 1, zp->m->static_length, fp ) != 
      zp->m->static_length ) {

    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading program: %s", fn );
  }

  fclose( fp );

  zp->checksum = 0;
  for( i = 64; i < zp->m->length; i++ ) {
    zp->checksum += read_byte( zp, i );
  }

  zp->version = h_version( zp );
  zp->release = h_release( zp );

  for( i = 0; i < 6; i++ ) {
    zp->serial[i] = read_byte( zp, 0x12 + i );
  }

  rb_iv_set( self, "@dictionary", 
    rb_funcall( Dictionary, id_new, 2, self, UINT2NUM(h_dictionary(zp)) ) );
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

  return INT2NUM(zp->m->length);
}

/*
 *  Get the length of dynamic memory.
 */

VALUE program_dynamic_length( VALUE self ) {
  zprogram *zp;

  Data_Get_Struct( self, zprogram, zp );

  return INT2NUM(zp->m->dynamic_length);
}

/*
 *  Get the length of static memory.
 */

VALUE program_static_length( VALUE self ) {
  zprogram *zp;

  Data_Get_Struct( self, zprogram, zp );

  return INT2NUM(zp->m->static_length);
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

/*
 * Read a string from this program.
 *
 */

VALUE program_read_string( VALUE self, VALUE a ) {
  zprogram *zp;
  Data_Get_Struct( self, zprogram, zp );

  long addr = NUM2LONG(a);
  int limit;

  int version = h_version( zp );
  zaddr abbreviations = h_abbreviations_table( zp );

  zword w;
  zbyte zs[2] = { 5, 0 };
  zbyte *c = &zs[0];
  zbyte last_c = 0;

  zaddr a_addr;
  VALUE a_str;

  int shift_state = 0;
  int shift_lock = 0;
  int status = 0;

  VALUE str = rb_str_new2( "" );

  do {
    int i;
    w = read_word( zp, addr );
    addr += 2;

    for( i = 10; i >= 0; i -= 5 ) {
      *c = (w >> i) & 0x1f;

      switch( status ) {
        case 0:
          if( shift_state == 2 && *c == 6 ) {
            status = 2;
          }
          else if( version == V1 && *c == 1 ) {
            *c = '\n';
            rb_str_append( str, rb_str_new2( c ) );
          }
          else if( version >= V2 && shift_state == 2 && *c == 7 ) {
            *c = '\n';
            rb_str_append( str, rb_str_new2( c ) );
          }
          else if( *c >= 6 ) {
            *c = alphabet( zp->m, shift_state, *c - 6 );
            rb_str_append( str, rb_str_new2( c ) );
          }
          else if( *c == 0 ) {
            *c = ' ';
            rb_str_append( str, rb_str_new2( c ) );
          }
          else if( (version >= V2 && *c == 1) ||
                   (version >= V3 && *c <= 3) ) {

            status = 1;
          }
          else {
             shift_state = (shift_lock + (*c & 1) + 1) % 3;

             if( version <= V2 && *c >= 4 ) {
               shift_lock = shift_state;
             }

             break;
          }

          shift_state = shift_lock;
          break;

        case 1:

          a_addr = abbreviations + (32 * (last_c - 1) + *c) * 2;
          a_addr = read_word( zp, a_addr ) << 1;  /* word address! */

          a_str = machine_read_string( self, INT2NUM((long) a_addr) );
          rb_str_append( str, a_str );

          status = 0;
          break;

        case 2:

          status = 3;
          break;

        case 3:

          *c = translate_from_zscii( zp->m, (zbyte) ((last_c << 5) | *c) );
          rb_str_append( str, rb_str_new2( c ) );

          status = 0;
          break;
      }

      last_c = *c;


    }
  }
  while( !(w & 0x8000) );

  return str;
}

