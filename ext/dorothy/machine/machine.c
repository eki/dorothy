
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
  VALUE memory;

  zmachine *zm;
  zprogram *zp;
  zmemory  *m;

  Data_Get_Struct( self, zmachine, zm );

  zm->self = self;

  if( ! RTEST(rb_obj_is_instance_of( program, Program )) ) {
    program = rb_funcall( Program, id_new, 1, program );
  }

  memory = rb_funcall( rb_iv_get( program, "@memory" ), id_dup, 0 );

  Data_Get_Struct( program, zprogram, zp );
  Data_Get_Struct( memory, zmemory, m );

  zm->m = m;
  zm->zp = zp;


  zm->finished = 0;

  zm->sp = zm->fp = zm->stack + STACK_SIZE;
  zm->frame_count = 0;

  zm->version = zp->version;

  /* Now that the program is loaded, finish setting up the Ruby stuff */

  rb_iv_set( self, "@program", program );
  rb_iv_set( self, "@memory", memory );

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
  VALUE program, memory;
  VALUE dynamic, stack;
  long i;
  zmachine *zm;
  zprogram *zp;
  zmemory *m;

  Data_Get_Struct( self, zmachine, zm );

  program = rb_ary_entry( ary, 0 );

  Data_Get_Struct( program, zprogram, zp );

  rb_iv_set( self, "@program", program );
  rb_iv_set( self, "@output", rb_ary_entry( ary, 1 ) );
  rb_iv_set( self, "@keyboard", rb_ary_entry( ary, 2 ) );
  rb_iv_set( self, "@rng", rb_ary_entry( ary, 3 ) );

  zm->zp = zp;
  zm->version = zp->version;

  memory = rb_funcall( rb_iv_get( program, "@memory" ), id_dup, 0 );

  Data_Get_Struct( memory, zmemory, m );

  rb_iv_set( self, "@memory", memory );

  zm->m = m;

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

