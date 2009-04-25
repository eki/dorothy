
#include "machine.h"

/*
 * z_new_line, print a new line.
 *
 *      no zargs used
 *
 */

void z_new_line( zmachine *zm ) {
  print_cstr( zm, "\n" );
}

/*
 * z_print, print a string embedded in the instruction stream.
 *
 *      no zargs used
 *
 */

void z_print( zmachine *zm ) {
  print_rstr( zm, memory_read_string( zm->m->self, LONG2NUM(PC(zm)) ) );
  p_skip_embedded_string( zm );
}

/*
 * z_print_addr, print a string from the lower 64KB.
 *
 *      zargs[0] = address of string to print
 *
 */

void z_print_addr( zmachine *zm ) {
  print_rstr( zm, 
    memory_read_string( zm->m->self, LONG2NUM((long) zm->zargs[0]) ) );
}

/*
 * z_print_char print a single ZSCII character.
 *
 *      zargs[0] = ZSCII character to be printed
 *
 */

void z_print_char( zmachine *zm ) {
  zbyte zs[2] = { translate_from_zscii( zm->m, zm->zargs[0] ), 0 };
  print_cstr( zm, zs );
}

void z_print_form( zmachine *zm ) {

}

/*
 * z_print_num, print a signed number.
 *
 *      zargs[0] = number to print
 *
 */

void z_print_num( zmachine *zm ) {
  print_rstr( zm, INT2NUM((short) zm->zargs[0]) );
}

/*
 * z_print_obj, print an object description.
 *
 *      zargs[0] = number of object to be printed
 *
 */

void z_print_obj( zmachine *zm ) {
  zaddr naddr = obj_name_addr( zm, zm->zargs[0] ) + 1; /* skip the size byte */
  zbyte size  = obj_name_size( zm, zm->zargs[0] );

  VALUE str;

  if( size > 0 ) {
    str = memory_read_string( zm->m->self, LONG2NUM((long) naddr) );
  }
  else {
    str = rb_str_new2( "" );
  }

  print_rstr( zm, str );
}

/*
 * z_print_paddr, print the string at the given packed address.
 *
 *      zargs[0] = packed address of string to be printed
 *
 */

void z_print_paddr( zmachine *zm ) {
  long addr = zm->zargs[0];

  if( h_version(zm) <= 3 ) {
    addr *= 2;
  }
  else if( h_version(zm) <= 5 ) {
    addr *= 4;
  }
  else if( h_version(zm) <= 7 ) {
    addr = addr * 2 + h_strings(zm) * 8;
  }
  else {
    addr *= 8;
  }

  print_rstr( zm, memory_read_string( zm->m->self, LONG2NUM(addr) ) );
}

/*
 * z_print_ret, print the string at PC, print newline then return true.
 *
 *      no zargs used
 *
 */

void z_print_ret( zmachine *zm ) {
  print_rstr( zm, memory_read_string( zm->m->self, LONG2NUM(PC(zm)) ) );
  print_cstr( zm, "\n" );
  p_skip_embedded_string( zm );
  p_ret( zm, 1 );
}

void z_print_table( zmachine *zm ) {

}

void z_print_unicode( zmachine *zm ) {

}

