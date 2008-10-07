
#include "machine.h"

/*
 * z_add, 16bit addition.
 *
 *      zargs[0] = first value
 *      zargs[1] = second value
 *
 */

void z_add( zmachine *zm ) {   
  p_store( zm, (zword) ((short) zm->zargs[0] + (short) zm->zargs[1]) );
}

/*
 * z_and, bitwise AND operation.
 *
 *      zargs[0] = first value
 *      zargs[1] = second value
 *
 */

void z_and( zmachine *zm ) {
  p_store( zm, zm->zargs[0] & zm->zargs[1] );
}

/*
 * z_art_shift, arithmetic SHIFT operation.
 *
 *      zargs[0] = value
 *      zargs[1] = #positions to shift left (positive) or right
 *
 */

void z_art_shift( zmachine *zm ) {
  if( (short) zm->zargs[1] > 0 ) {
    p_store( zm, (zword) ((short) zm->zargs[0] << (short) zm->zargs[1]) );
  }
  else {
    p_store( zm, (zword) ((short) zm->zargs[0] >> - (short) zm->zargs[1]) );
  }
}

/*
 * z_div, signed 16bit division.
 *
 *      zargs[0] = first value
 *      zargs[1] = second value
 *
 */

void z_div( zmachine *zm ) {
  if( zm->zargs[1] == 0 ) {
    runtime_error( "divide by zero" );
  }

  p_store( zm, (zword) ((short) zm->zargs[0] / (short) zm->zargs[1]) );
}

/*
 * z_je, branch if the first value equals any of the following.
 *
 *      zargs[0] = first value
 *      zargs[1] = second value (optional)
 *      ...
 *      zargs[3] = fourth value (optional)
 *
 */

void z_je( zmachine *zm ) {
  p_branch( zm, (zm->zargc > 1 && (zm->zargs[0] == zm->zargs[1])) ||
                (zm->zargc > 2 && (zm->zargs[0] == zm->zargs[2])) ||
                (zm->zargc > 3 && (zm->zargs[0] == zm->zargs[3])) );
}

/*
 * z_jg, branch if the first value is greater than the second.
 *
 *      zargs[0] = first value
 *      zargs[1] = second value
 *
 */

void z_jg( zmachine *zm ) {
  p_branch( zm, (short) zm->zargs[0] > (short) zm->zargs[1] );
}

/*
 * z_jl, branch if the first value is less than the second.
 *
 *      zargs[0] = first value
 *      zargs[1] = second value
 *
 */

void z_jl( zmachine *zm ) {
  p_branch( zm, (short) zm->zargs[0] < (short) zm->zargs[1] );
}

/*
 * z_jz, branch if value is zero.
 *
 *      zargs[0] = value
 *
 */

void z_jz( zmachine *zm ) {
  p_branch( zm, (short) zm->zargs[0] == 0 );
}


/*
 * z_log_shift, logical SHIFT operation.
 *
 *      zargs[0] = value
 *      zargs[1] = #positions to shift left (positive) or right (negative)
 *
 */

void z_log_shift( zmachine *zm ) {
  if( (short) zm->zargs[1] > 0 ) {
    p_store( zm, (zword) (zm->zargs[0] << (short) zm->zargs[1]) );
  }
  else {
    p_store( zm, (zword) (zm->zargs[0] >> - (short) zm->zargs[1]) );
  }
}

/*
 * z_mod, remainder after signed 16bit division.
 *
 *      zargs[0] = first value
 *      zargs[1] = second value
 *
 */

void z_mod( zmachine *zm ) {
  if( zm->zargs[1] == 0 ) {
    runtime_error( "divide by zero" );
  }

  p_store( zm, (zword) ((short) zm->zargs[0] % (short) zm->zargs[1]) );
}

/*
 * z_mul, 16bit multiplication.
 *
 *      zargs[0] = first value
 *      zargs[1] = second value
 *
 */

void z_mul( zmachine *zm ) {
  p_store( zm, (zword) ((short) zm->zargs[0] * (short) zm->zargs[1]) );
}

/*
 * z_not, bitwise NOT operation.
 *
 *      zargs[0] = value
 *
 */

void z_not( zmachine *zm ) {
  p_store( zm, (zword) ~zm->zargs[0] );
}

/*
 * z_or, bitwise OR operation.
 *
 *      zargs[0] = first value
 *      zargs[1] = second value
 *
 */

void z_or( zmachine *zm ) {
  p_store( zm, (zword) (zm->zargs[0] | zm->zargs[1]) );
}

/*
 * z_random, store a random number or set the random number seed.
 *
 *      zargs[0] = range (positive) or seed value (negative)
 *
 *  TODO:  RandomNumberGenerator can be re-seeded by this opcode.  The RNG
 *         code should be updated to serialize / replay correctly when this
 *         happens.
 */

void z_random( zmachine *zm ) {
  zword n = 0;
  short range = zm->zargs[0];
  VALUE rng = rb_iv_get( zm->self, "@rng" );

  if( range < 0 ) {
    rb_funcall( rng, id_srand, 1, INT2NUM(-range) );
  }
  else if( range == 0 ) {
    rb_funcall( rng, id_srand, 0 );
  }
  else {
    n = NUM2UINT(rb_funcall( rng, id_rand, 1, INT2NUM(range) ));
  }

  p_store( zm, n );
}

/*
 * z_sub, 16bit substraction.
 *
 *      zargs[0] = first value
 *      zargs[1] = second value
 *
 */

void z_sub( zmachine *zm ) {
  p_store( zm, (zword) ((short) zm->zargs[0] - (short) zm->zargs[1]) );
}

/*
 * z_test, branch if all the flags of a bit mask are set in a value.
 *
 *      zargs[0] = value to be examined
 *      zargs[1] = bit mask
 *
 */

void z_test( zmachine *zm ) {
  p_branch( zm, (zm->zargs[0] & zm->zargs[1]) == zm->zargs[1] );
}

