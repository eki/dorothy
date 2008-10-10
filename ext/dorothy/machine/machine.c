
#include <stdio.h>
#include "machine.h"

/*
 *  Allocate memory for a Machine.
 */

VALUE machine_alloc( VALUE klass ) {
  zmachine *zm = ALLOC( zmachine );
  VALUE obj = Data_Wrap_Struct( klass, 0, machine_free, zm );
}

/*
 *  Initialize the machine.  Loads a story file into memory.
 */

VALUE machine_initialize( VALUE self, VALUE program ) {
  VALUE header;
  zmachine *zm;
  zprogram *zp;

  Data_Get_Struct( self, zmachine, zm );

  zm->self = self;

  if( ! RTEST(rb_obj_is_instance_of( program, Program )) ) {
    program = rb_funcall( Program, id_new, 1, program );
  }

  Data_Get_Struct( program, zprogram, zp );

  zm->zp = zp;
  zm->m = ALLOC( zmemory );

  zm->m->m_dynamic = ALLOC_N(zbyte, zp->m->dynamic_length);
  MEMCPY( zm->m->m_dynamic, zp->m->m_dynamic, zbyte, zp->m->dynamic_length);

  zm->m->m_static = zp->m->m_static;

  zm->finished = 0;

  zm->sp = zm->fp = zm->stack + STACK_SIZE;
  zm->frame_count = 0;

  zm->version = zp->version;

  zm->m->length = zp->m->length;
  zm->m->dynamic_length = zp->m->dynamic_length;
  zm->m->static_length = zp->m->static_length;

  /* Now that the program is loaded, finish setting up the Ruby stuff */

  rb_iv_set( self, "@program", program );

  rb_iv_set( self, "@header", rb_funcall( Header, id_new, 1, self ) );

  rb_iv_set( self, "@output", rb_ary_new() );

  rb_iv_set( self, "@keyboard",
    rb_funcall( InputStream, id_new, 1, UINT2NUM(0) ) );

  rb_iv_set( self, "@trace", rb_ary_new() );

  rb_iv_set( self, "@rng", rb_funcall( RandomNumberGenerator, id_new, 0 ) );

  if( h_version(zm) <= 3 ) {
    rb_iv_set( self, "@status", rb_funcall( Status, id_new, 1, self ) );
  }
}

/*
 *  Free the memory used by a Machine.  
 */

void machine_free( void *p ) {
  free( p );
}

/*
 *  Dump the state of this machine to an array that can be reloaded later.
 */

VALUE machine_marshal_dump( VALUE self ) {
  VALUE ary = rb_ary_new();
  VALUE dynamic, stack;
  long i;
  zmachine *zm;

  Data_Get_Struct( self, zmachine, zm );

  rb_ary_push( ary, rb_iv_get( self, "@program" ) );
  rb_ary_push( ary, rb_iv_get( self, "@output" ) );
  rb_ary_push( ary, rb_iv_get( self, "@keyboard" ) );
  rb_ary_push( ary, rb_iv_get( self, "@rng" ) );

  dynamic = rb_ary_new();
  for( i = 0; i < zm->m->dynamic_length; i++ ) {
    rb_ary_push( dynamic, UINT2NUM(zm->m->m_dynamic[i]) );
  }

  rb_ary_push( ary, dynamic );

  stack = rb_ary_new();
  for( i = 0; i < STACK_SIZE; i++ ) {
    rb_ary_push( stack, UINT2NUM(zm->stack[i]) );
  }

  rb_ary_push( ary, stack );

  rb_ary_push( ary, UINT2NUM(PC(zm)) );
  rb_ary_push( ary, UINT2NUM(zm->sp - zm->stack) );
  rb_ary_push( ary, UINT2NUM(zm->fp - zm->stack) );
  rb_ary_push( ary, UINT2NUM(zm->frame_count) );
  rb_ary_push( ary, UINT2NUM(zm->finished) );
}

/*
 *  Load the state of a running machine from the given array.
 */

VALUE machine_marshal_load( VALUE self, VALUE ary ) {
  VALUE program;
  VALUE dynamic, stack;
  long i;
  zmachine *zm;
  zprogram *zp;

  Data_Get_Struct( self, zmachine, zm );

  program = rb_ary_entry( ary, 0 );

  Data_Get_Struct( program, zprogram, zp );

  rb_iv_set( self, "@program", program );
  rb_iv_set( self, "@output", rb_ary_entry( ary, 1 ) );
  rb_iv_set( self, "@keyboard", rb_ary_entry( ary, 2 ) );
  rb_iv_set( self, "@rng", rb_ary_entry( ary, 3 ) );

  zm->zp = zp;
  zm->m = ALLOC( zmemory );

  zm->m->m_dynamic = ALLOC_N(zbyte, zp->m->dynamic_length);
  zm->m->m_static = zp->m->m_static;

  zm->version = zp->version;

  zm->m->length = zp->m->length;
  zm->m->dynamic_length = zp->m->dynamic_length;
  zm->m->static_length = zp->m->static_length;

  dynamic = rb_ary_entry( ary, 4 );

  for( i = 0; i < zm->m->dynamic_length; i++ ) {
    zm->m->m_dynamic[i] = NUM2UINT(rb_ary_entry( dynamic, i ));
  } 

  stack = rb_ary_entry( ary, 5 );

  for( i = 0; i < STACK_SIZE; i++ ) {
    zm->stack[i] = NUM2UINT(rb_ary_entry( stack, i ));
  }

  zm->pcp = zm->m->m_static + NUM2UINT(rb_ary_entry( ary, 6 )) - 
            zm->m->dynamic_length;
  zm->sp = zm->stack + NUM2UINT(rb_ary_entry( ary, 7 ));
  zm->fp = zm->stack + NUM2UINT(rb_ary_entry( ary, 8 ));
  zm->frame_count = NUM2UINT(rb_ary_entry( ary, 9 ));
  zm->finished = NUM2UINT(rb_ary_entry( ary, 10 ));

  rb_iv_set( self, "@trace", rb_ary_new() );
  rb_iv_set( self, "@header", rb_funcall( Header, id_new, 1, self ) );

  if( h_version(zm) <= 3 ) {
    rb_iv_set( self, "@status", rb_funcall( Status, id_new, 1, self ) );
  }

  zm->self = self;

  return self;
}

/*
 *  Execute the next instruction (if possible) in the program loaded into the
 *  machine.  Returns false if an instruction was *not* executed (for example,
 *  the program has finished execution or the instruction is a read but no
 *  input is available.
 */

VALUE machine_step( VALUE self ) {
  zmachine *zm;

  Data_Get_Struct( self, zmachine, zm );

  return p_step( zm ) ? Qtrue : Qfalse;
}

/*
 *  Has the program finished running?
 */

VALUE machine_finished( VALUE self ) {
  zmachine *zm;

  Data_Get_Struct( self, zmachine, zm );

  return zm->finished >= 9999 ? Qtrue : Qfalse;
}

VALUE machine_read_byte( VALUE self, VALUE addr ) {
  zmachine *zm;
  zaddr a = (zaddr) NUM2INT(addr);

  Data_Get_Struct( self, zmachine, zm );

  return INT2NUM(read_byte( zm, a ));
}

VALUE machine_read_word( VALUE self, VALUE addr ) {
  zmachine *zm;
  zaddr a = (zaddr) NUM2INT(addr);

  Data_Get_Struct( self, zmachine, zm );

  return INT2NUM(read_word( zm, a ));
}

VALUE machine_read_string_array( VALUE self, VALUE addr, VALUE length ) {
  zmachine *zm;
  zaddr a = (zaddr) NUM2INT(addr);

  Data_Get_Struct( self, zmachine, zm );

  zbyte zs[2] = { 5, 0 };
  zbyte *c = &zs[0];
  VALUE str = rb_str_new2( "" );
  int len = NUM2INT(length);
  int i;

  for( i = 0; i < len; i++ ) {
    *c = translate_from_zscii( zm->m, read_byte( zm, a + i ) );

    rb_str_append( str, rb_str_new2( c ) );
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
  memory_wrapper mw;
  memory_wrapper *zm = &mw;
  zm->m = m;

  zaddr unicode = h_unicode_table( zm );

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

      zbyte N = read_byte( zm, unicode );

      if (c - 0x9b < N) {

        zaddr addr = unicode + 1 + 2 * (c - 0x9b);
        zword uc = read_word( zm, addr );

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

zbyte translate_to_zscii( zmachine *zm, zchar c ) {
  zaddr unicode = h_unicode_table( zm );
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
      zbyte N = read_byte( zm, unicode );

      for( i = 0x9b; i < 0x9b + N; i++ ) {
        if( c == read_word( zm, unicode + 1 + 2 * (i - 0x9b) ) ) {
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
  memory_wrapper mw;
  memory_wrapper *zm = &mw;
  zm->m = m;

  int version = h_version( zm );
  zaddr alphabet = h_alphabet_table( zm );

  if( alphabet != 0 ) {      /* game uses its own alphabet */
      zbyte c = read_byte( zm, alphabet + set * 26 + index );
      return translate_from_zscii( zm->m, c );

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


VALUE machine_read_string( VALUE self, VALUE a ) {
  zmachine *zm;
  Data_Get_Struct( self, zmachine, zm );

  long addr = NUM2LONG(a);
  bool consume = false;
  int limit;

  if( addr > 0 ) {
    trace( zm, "read_string called with addr (%d)\n", addr );
  }
  else if( addr == -1 ) {
    addr = PC(zm);
    consume = true;
    trace( zm, "read_string called with PC (%d)\n", addr );
  }

  int version = h_version( zm );
  zaddr abbreviations = h_abbreviations_table( zm );

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
    w = read_word( zm, addr );
    addr += 2;

    if( consume ) {
      zm->pcp += 2;
    }

    trace( zm, "  raw word: %x\n", w );

    for( i = 10; i >= 0; i -= 5 ) {
      *c = (w >> i) & 0x1f;

      trace( zm, "  raw c: %2d    ", *c );

      switch( status ) {
        case 0:
          if( shift_state == 2 && *c == 6 ) {
            status = 2;

            trace( zm, "  status 2\n" );
          }
          else if( version == V1 && *c == 1 ) {
            *c = '\n';
            rb_str_append( str, rb_str_new2( c ) );

            trace( zm, "  (newline) read c: %s\n", c ); 
          }
          else if( version >= V2 && shift_state == 2 && *c == 7 ) {
            *c = '\n';
            rb_str_append( str, rb_str_new2( c ) );

            trace( zm, "  (newline) read c: %s\n", c ); 
          }
          else if( *c >= 6 ) {
            *c = alphabet( zm->m, shift_state, *c - 6 );
            rb_str_append( str, rb_str_new2( c ) );

            trace( zm, "  (alpha) read c: %s\n", c ); 
          }
          else if( *c == 0 ) {
            *c = ' ';
            rb_str_append( str, rb_str_new2( c ) );

            trace( zm, "  (space) read c: %s\n", c ); 
          }
          else if( (version >= V2 && *c == 1) ||
                   (version >= V3 && *c <= 3) ) {

            status = 1;

            trace( zm, "status 1\n" );
          }
          else {
             shift_state = (shift_lock + (*c & 1) + 1) % 3;

             trace( zm, "  shift_state %d\n", shift_state );

             if( version <= V2 && *c >= 4 ) {
               shift_lock = shift_state;
             }

             break;
          }

          shift_state = shift_lock;
          break;

        case 1:

          a_addr = abbreviations + (32 * (last_c - 1) + *c) * 2;
          a_addr = read_word( zm, a_addr ) << 1;  /* word address! */

          trace( zm, "recursing to look up abbreviation (%d)\n", a_addr );

          a_str = machine_read_string( self, INT2NUM((long) a_addr) );
          rb_str_append( str, a_str );

          status = 0;
          break;

        case 2:

          trace( zm, "  status 3\n" );
          status = 3;
          break;

        case 3:

          *c = translate_from_zscii( zm->m, (zbyte) ((last_c << 5) | *c) );
          rb_str_append( str, rb_str_new2( c ) );

          trace( zm, "  (10bit) read c: %s\n", c ); 

          status = 0;
          break;
      }

      last_c = *c;


    }
  }
  while( !(w & 0x8000) );

  trace( zm, "returing str\n" );

  return str;
}

