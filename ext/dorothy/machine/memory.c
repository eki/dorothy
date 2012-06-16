
#include "machine.h"


/*
 *  Allocate Z::Memory.
 */

VALUE memory_alloc( VALUE klass ) {
  zmemory *m = ALLOC( zmemory );
  return Data_Wrap_Struct( klass, memory_mark, memory_free, m );
}

/*
 *  Initialize the machine.  Loads a story file into memory.
 */

VALUE memory_initialize( VALUE self, VALUE filename ) {
  char *fn = StringValuePtr( filename );
  FILE *fp;
  zmemory *m;

  Data_Get_Struct( self, zmemory, m );

  fp = fopen( fn, "rb" );

  if( fp == NULL ) {
    rb_raise( rb_eRuntimeError, "Unable to open file: %s", fn );
  }

  m->self = self;
  m->parent = 0;
  m->children = 0;

  m->m = m;
  m->m_dynamic = ALLOC_N( zbyte, 64 );

  if( fread( m->m_dynamic, 1, 64, fp ) != 64 ) {
    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading header: %s", fn );
  }

  /* Set the actual program length (don't trust the program header) */

  fseek( fp, 0, SEEK_END );
  m->length = ftell( fp );
  fseek( fp, 0, SEEK_SET );

  /* How long are the dynamic and static parts of memory? */

  m->dynamic_length = 64;  /* Temporary */

  m->dynamic_length = h_static_memory(m) - 1;
  m->static_length = m->length - m->dynamic_length;

  /* Read the entire program into memory */

  REALLOC_N( m->m_dynamic, zbyte, m->dynamic_length );

  if( fread( m->m_dynamic, 1, m->dynamic_length, fp ) != m->dynamic_length ) {
    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading program: %s", fn );
  }

  m->m_static = ALLOC_N( zbyte, m->static_length );

  if( fread( m->m_static, 1, m->static_length, fp ) != m->static_length ) {
    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading program: %s", fn );
  }

  fclose( fp );

  rb_iv_set( self, "@dictionary", 
    rb_funcall( Dictionary, id_new, 2, self, UINT2NUM(h_dictionary(m)) ) );

  rb_iv_set( self, "@header", rb_funcall( Header, id_new, 1, self ) );
  rb_iv_set( self, "@objects", rb_funcall( ObjectTable, id_new, 1, self ) );

  if( h_version(m) <= 3 ) {
    rb_iv_set( self, "@status", rb_funcall( Status, id_new, 1, self ) );
  }

  return self;
}

/*
 *  Free the memory used by a Machine.  
 */

void memory_free( void *p ) {
  zmemory *m = (zmemory *)p;

  if( m->parent != 0 ) {
    m->parent->children--;
  }
  
  free( m->m_dynamic );

  if( m->parent == 0 && m->children <= 0 ) {
    free( m->m_static );
  }

  free( m );
}

/*
 *  Mark references to other zmemory structs (and by extension Z::Memory 
 *  objects).
 */

void memory_mark( void *p ) {
  zmemory *m = (zmemory *)p;

  if( m->parent != 0 ) {
    rb_gc_mark( m->parent->self );
  }

}


VALUE memory_initialize_copy( VALUE self, VALUE original ) {
  zmemory *m;
  zmemory *mo;

  Data_Get_Struct( self, zmemory, m );
  Data_Get_Struct( original, zmemory, mo );

  m->self = self;
  m->parent = mo;
  m->children = 0;
  m->parent->children++;

  m->m = m;
  m->m_dynamic = ALLOC_N(zbyte, mo->dynamic_length);
  MEMCPY( m->m_dynamic, mo->m_dynamic, zbyte, mo->dynamic_length ); 

  m->m_static = mo->m_static;

  m->length = mo->length;
  m->dynamic_length = mo->dynamic_length;
  m->static_length = mo->static_length;

  rb_iv_set( self, "@header", rb_funcall( Header, id_new, 1, self ) );
  rb_iv_set( self, "@objects", rb_funcall( ObjectTable, id_new, 1, self ) );

  if( h_version(m) <= 3 ) {
    rb_iv_set( self, "@status", rb_funcall( Status, id_new, 1, self ) );
  }

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

/*
 *  Get the actual program length (may be different from the length reported
 *  by the header).
 */

VALUE memory_length( VALUE self ) {
  zmemory *m;

  Data_Get_Struct( self, zmemory, m );

  return UINT2NUM(m->length);
}

/*
 *  Get the length of dynamic memory.
 */

VALUE memory_dynamic_length( VALUE self ) {
  zmemory *m;

  Data_Get_Struct( self, zmemory, m );

  return UINT2NUM(m->dynamic_length);
}

/*
 *  Get the length of static memory.
 */

VALUE memory_static_length( VALUE self ) {
  zmemory *m;

  Data_Get_Struct( self, zmemory, m );

  return UINT2NUM(m->static_length);
}

VALUE memory_read_byte( VALUE self, VALUE addr ) {
  zmemory *m;
  zaddr a = (zaddr) NUM2INT(addr);

  Data_Get_Struct( self, zmemory, m );

  return INT2NUM(read_byte( m, a ));
}

VALUE memory_read_word( VALUE self, VALUE addr ) {
  zmemory *m;
  zaddr a = (zaddr) NUM2INT(addr);

  Data_Get_Struct( self, zmemory, m );

  return INT2NUM(read_word( m, a ));
}

VALUE memory_read_string_array( VALUE self, VALUE addr, VALUE length ) {
  zmemory *m;
  zaddr a = (zaddr) NUM2INT(addr);

  Data_Get_Struct( self, zmemory, m );

  zbyte zs[2] = { 5, 0 };
  zbyte *c = &zs[0];
  VALUE str = rb_str_new2( "" );
  int len = NUM2INT(length);
  int i;

  for( i = 0; i < len; i++ ) {
    *c = translate_from_zscii( m, read_byte( m, a + i ) );

    rb_str_append( str, rb_str_new2( (char *)c ) );
  }

  return str;
}

static zchar zscii_to_latin1[] = {
    0xe4, 0xf6, 0xfc, 0xc4, 0xd6, 0xdc, 0xdf, 0xbb,
    0xab, 0xeb, 0xef, 0xff, 0xcb, 0xcf, 0xe1, 0xe9,
    0xed, 0xf3, 0xfa, 0xfd, 0xc1, 0xc9, 0xcd, 0xd3,
    0xda, 0xdd, 0xe0, 0xe8, 0xec, 0xf2, 0xf9, 0xc0,
    0xc8, 0xcc, 0xd2, 0xd9, 0xe2, 0xea, 0xee, 0xf4,
    0xfb, 0xc2, 0xca, 0xce, 0xd4, 0xdb, 0xe5, 0xc5,
    0xf8, 0xd8, 0xe3, 0xf1, 0xf5, 0xc3, 0xd1, 0xd5,
    0xe6, 0xc6, 0xe7, 0xc7, 0xfe, 0xf0, 0xde, 0xd0,
    0xa3, 0x00, 0x00, 0xa1, 0xbf
};

zchar translate_from_zscii( zmemory *m, zbyte c ) {   
  zaddr unicode = h_unicode_table( m );

  if( c == 0xfc ) {
    return ZC_MENU_CLICK;
  }

  if( c == 0xfd ) {
    return ZC_DOUBLE_CLICK;
  }

  if( c == 0xfe ) {
    return ZC_SINGLE_CLICK;
  }

  if( c >= 0x9b ) {

    if( unicode != 0 ) {    /* game has its own Unicode table */

      zbyte N = read_byte( m, unicode );

      if (c - 0x9b < N) {

        zaddr addr = unicode + 1 + 2 * (c - 0x9b);
        zword uc = read_word( m, addr );

        return (uc < 0x100) ? (zchar) uc : '?';
      }
      else {
        return '?';
      }
    }
    else {                        /* game uses standard set */

      if( c <= 0xdf ) {

        if( c == 0xdc || c == 0xdd ) {   /* Oe and oe ligatures */
          return '?';                    /* are not ISO-Latin 1 */
        }

        return zscii_to_latin1[c - 0x9b];
      } 
      else {
        return '?';
      }
    }
  }

  return c;
}

zbyte translate_to_zscii( zmemory *m, zchar c ) {
  zaddr unicode = h_unicode_table( m );
  int i;

  if( c == ZC_SINGLE_CLICK ) {
    return 0xfe;
  }

  if( c == ZC_DOUBLE_CLICK ) {
    return 0xfd;
  }

  if( c == ZC_MENU_CLICK ) {
    return 0xfc;
  }

  if( c >= ZC_LATIN1_MIN ) {

    if( unicode != 0 ) {    /* game has its own Unicode table */
      zbyte N = read_byte( m, unicode );

      for( i = 0x9b; i < 0x9b + N; i++ ) {
        if( c == read_word( m, unicode + 1 + 2 * (i - 0x9b) ) ) {
          return (zbyte) i;
        }
      }

      return '?';
    } 
    else {                        /* game uses standard set */

      for( i = 0x9b; i <= 0xdf; i++ ) {
        if( c == zscii_to_latin1[i - 0x9b] ) {
          return (zbyte) i;
        }
      }

      return '?';
    }
  }

  if( c == 0 ) {    /* Safety thing from David Kinder */  
    c = '?';        /* regarding his Unicode patches */
  }                 /* Sept 15, 2002 */

  return c;
}

zchar alphabet( zmemory *m, int set, int index ) {   
  int version = h_version( m );
  zaddr alphabet = h_alphabet_table( m );

  if( alphabet != 0 ) {      /* game uses its own alphabet */
      zbyte c = read_byte( m, alphabet + set * 26 + index );
      return translate_from_zscii( m, c );

  } 
  else {                    /* game uses default alphabet */
    if( set == 0 ) {
      return 'a' + index;
    }
    else if( set == 1 ) {
      return 'A' + index;
    }
    else if( version == V1 ) {
      return " 0123456789.,!?_#'\"/\\<-:()"[index];
    }
    else {
      return " ^0123456789.,!?_#'\"/\\-:()"[index];
    }
  }
}

/*
 * Read a string from this program.
 *
 */

VALUE memory_read_string( VALUE self, VALUE a ) {
  zmemory *m;
  Data_Get_Struct( self, zmemory, m );

  long addr = NUM2LONG(a);

  int version = h_version( m );
  zaddr abbreviations = h_abbreviations_table( m );

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
    w = read_word( m, addr );
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
            rb_str_append( str, rb_str_new2( (char *)c ) );
          }
          else if( version >= V2 && shift_state == 2 && *c == 7 ) {
            *c = '\n';
            rb_str_append( str, rb_str_new2( (char *)c ) );
          }
          else if( *c >= 6 ) {
            *c = alphabet( m, shift_state, *c - 6 );
            rb_str_append( str, rb_str_new2( (char *)c ) );
          }
          else if( *c == 0 ) {
            *c = ' ';
            rb_str_append( str, rb_str_new2( (char *)c ) );
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
          a_addr = read_word( m, a_addr ) << 1;  /* word address! */

          a_str = memory_read_string( self, INT2NUM((long) a_addr) );
          rb_str_append( str, a_str );

          status = 0;
          break;

        case 2:

          status = 3;
          break;

        case 3:

          *c = translate_from_zscii( m, (zbyte) ((last_c << 5) | *c) );
          rb_str_append( str, rb_str_new2( (char *)c ) );

          status = 0;
          break;
      }

      last_c = *c;


    }
  }
  while( !(w & 0x8000) );

  return str;
}

/*
 * Dump this object's data to an array.
 */
VALUE memory_dump_data( VALUE self ) {
    VALUE ary = rb_ary_new();
    VALUE dynamic_mem = rb_ary_new();
    VALUE static_mem = rb_ary_new();
    long i;
    
    zmemory *m;
    Data_Get_Struct( self, zmemory, m );

    rb_ary_push( ary, UINT2NUM(m->length) );
    rb_ary_push( ary, UINT2NUM(m->static_length) );
    rb_ary_push( ary, UINT2NUM(m->dynamic_length) );

    for( i = 0; i < m->static_length; i++ ) {
      rb_ary_push( static_mem, UINT2NUM(m->m_static[i]) );
    }

    rb_ary_push( ary, static_mem );
    
    for( i = 0; i < m->dynamic_length; i++ ) {
      rb_ary_push( dynamic_mem, UINT2NUM(m->m_dynamic[i]) );
    }

    rb_ary_push( ary, dynamic_mem );
    
    return ary;
}

/*
 * Reconstitute this object from an array constructed by memory_dump_data.
 */
VALUE memory_load_data( VALUE self, VALUE ary ) {
    zmemory *m;
    long i;
    Data_Get_Struct( self, zmemory, m );
    
    m->self = self;
    m->parent = 0;
    m->children = 0;
    m->m = m;
    
    m->length = NUM2UINT( rb_ary_entry( ary, 0 ) );
    m->static_length = NUM2UINT( rb_ary_entry( ary, 1 ) );
    m->dynamic_length = NUM2UINT( rb_ary_entry( ary, 2 ) );
    
    VALUE static_mem = rb_ary_entry( ary, 3 );
    VALUE dynamic_mem = rb_ary_entry( ary, 4 );

    m->m_static = ALLOC_N( zbyte, m->static_length );
    for( i = 0; i < m->static_length; i++ ) {
        m->m_static[i] = NUM2UINT( rb_ary_entry( static_mem, i ) );
    }
    
    m->m_dynamic = ALLOC_N( zbyte, m->dynamic_length );
    for( i = 0; i < m->dynamic_length; i++ ) {
        m->m_dynamic[i] = NUM2UINT( rb_ary_entry( dynamic_mem, i ) );
    }
    
    return self;
}
