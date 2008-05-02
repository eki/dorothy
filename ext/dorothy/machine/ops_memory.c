
#include "machine.h"

/*
 * z_copy_table, copy a table or fill it with zeroes.
 *
 *      zargs[0] = address of table
 *      zargs[1] = destination address or 0 for fill
 *      zargs[2] = size of table
 *
 * Note: Copying is safe even when source and destination overlap; but
 *       if zargs[1] is negative the table _must_ be copied forwards.
 *
 */

void z_copy_table( zmachine *zm ) {
  zword addr;
  zword size = zm->zargs[2];
  zbyte value;
  int i;

  if( zm->zargs[1] == 0 ) {
    for( i = 0; i < size; i++ ) {
      storeb( zm, (zword) (zm->zargs[0] + i), 0 );
    }
  }
  else if( (short) size < 0 || zm->zargs[0] > zm->zargs[1] ) {
    size = ((short) size < 0) ? - (short) size : size;

    for( i = 0; i < size; i++ ) {
      value = read_byte( zm, zm->zargs[0] + i );
      storeb( zm, (zword) (zm->zargs[1] + i), value );
    }
  }
  else {
    for( i = size - 1; i >= 0; i-- ) {
      value = read_byte( zm, zm->zargs[0] + i );
      storeb( zm, (zword) (zm->zargs[1] + i), value );
    }
  }
}

/*
 * z_loadb, store a value from a table of bytes.
 *
 *      zargs[0] = address of table
 *      zargs[1] = index of table entry to store
 *
 */

void z_loadb( zmachine *zm ) {
  p_store( zm, read_byte( zm, zm->zargs[0] + zm->zargs[1] ) );
}

/*
 * z_loadw, store a value from a table of words.
 *
 *      zargs[0] = address of table
 *      zargs[1] = index of table entry to store
 *
 */

void z_loadw( zmachine *zm ) {
  p_store( zm, read_word( zm, zm->zargs[0] + 2 * zm->zargs[1] ) );
}

/*
 * z_scan_table, find and store the address of a target within a table.
 *
 *      zargs[0] = target value to be searched for
 *      zargs[1] = address of table
 *      zargs[2] = number of table entries to check value against
 *      zargs[3] = type of table (optional, defaults to 0x82)
 *
 * Note: The table is a word array if bit 7 of zargs[3] is set; otherwise
 *       it's a byte array. The lower bits hold the address step.
 *
 */

void z_scan_table( zmachine *zm ) {
  zword addr = zm->zargs[1];
  zword match = 0;
  int i;

  /* Supply default arguments */

  if( zm->zargc < 4 ) {
    zm->zargs[3] = 0x82;
  }

  /* Scan byte or word array */

  for( i = 0; i < zm->zargs[2] && !match; i++ ) {
    if( (zm->zargs[3] & 0x80 && read_word( zm, addr ) == zm->zargs[0]) ||
                               (read_byte( zm, addr ) == zm->zargs[0]) ) {
      match = addr;
    }

    addr += zm->zargs[3] & 0x7f;
  }

  p_store( zm, match );
  p_branch( zm, match );
}

/*
 * z_storeb, write a byte into a table of bytes.
 *
 *      zargs[0] = address of table
 *      zargs[1] = index of table entry
 *      zargs[2] = value to be written
 *
 */

void z_storeb( zmachine *zm ) {
  storeb( zm, (zword) (zm->zargs[0] + zm->zargs[1]), zm->zargs[2] );
}

/*
 * z_storew, write a word into a table of words.
 *
 *      zargs[0] = address of table
 *      zargs[1] = index of table entry
 *      zargs[2] = value to be written
 *
 */

void z_storew( zmachine *zm ) {
  storew( zm, (zword) (zm->zargs[0] + 2 * zm->zargs[1]), zm->zargs[2] );
}

/*
 * z_verify, check the story file integrity.
 *
 *      no zargs used
 *
 */

void z_verify( zmachine *zm ) {
  p_branch( zm, zm->program_checksum == h_program_checksum(zm) );
}

