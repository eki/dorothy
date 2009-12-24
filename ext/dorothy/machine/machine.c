
#include <stdio.h>
#include "machine.h"

/*
 *  Allocate memory for a Machine.
 */

VALUE machine_alloc( VALUE klass ) {
  zmachine *zm = ALLOC( zmachine );
  return Data_Wrap_Struct( klass, 0, machine_free, zm );
}

/*
 *  Initialize the machine.  Loads a story file into memory.
 */

VALUE machine_initialize( VALUE self, VALUE program ) {
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

  rb_iv_set( self, "@screen", rb_funcall( Screen, id_new, 1, self ) );

  rb_iv_set( self, "@keyboard",
    rb_funcall( InputStream, id_new, 1, UINT2NUM(0) ) );

  rb_iv_set( self, "@trace", rb_ary_new() );

  rb_iv_set( self, "@rng", rb_funcall( RandomNumberGenerator, id_new, 0 ) );

  return self;
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

  return ary;
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

/*
 *  What is the PC?
 */

VALUE machine_pc( VALUE self ) {
  zmachine *zm;

  Data_Get_Struct( self, zmachine, zm );

  return UINT2NUM( PC(zm) );
}

