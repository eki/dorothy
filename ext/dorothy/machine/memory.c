
#include "machine.h"

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

