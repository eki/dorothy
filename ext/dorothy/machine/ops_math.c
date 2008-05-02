
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

void z_dec( zmachine *zm ) {

}

void z_dec_chk( zmachine *zm ) {

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

void z_inc( zmachine *zm ) {

}

void z_inc_chk( zmachine *zm ) {

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

void z_random( zmachine *zm ) {

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

void z_test( zmachine *zm ) {

}

