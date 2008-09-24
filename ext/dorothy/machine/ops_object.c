
#include "machine.h"

zaddr obj_next_prop_addr( zmachine *zm, zaddr prop ) {
  zbyte offset = read_byte( zm, prop );
  prop++;

  if( h_version(zm) < 4 ) {
    offset >>= 5;
  }
  else if( !(offset & 0x80) ) {
    offset >>= 6;
  }
  else {
    offset = read_byte( zm, prop ) & 0x3f;
    offset = offset == 0 ? 64 : offset;
  }

  return prop + offset + 1;
}

void obj_set_parent( zmachine *zm, zword n, zword pn ) {
  if( h_version(zm) < 4 ) {
    write_byte( zm, obj_addr(zm,n) + obj_parent_offset(zm), pn );
  }
  else {
    write_word( zm, obj_addr(zm,n) + obj_parent_offset(zm), pn );
  }
}

void obj_set_sibling( zmachine *zm, zword n, zword sn ) {
  if( h_version(zm) < 4 ) {
    write_byte( zm, obj_addr(zm,n) + obj_parent_offset(zm), sn );
  }
  else {
    write_word( zm, obj_addr(zm,n) + obj_parent_offset(zm), sn );
  }
}

void obj_set_child( zmachine *zm, zword n, zword cn ) {
  if( h_version(zm) < 4 ) {
    write_byte( zm, obj_addr(zm,n) + obj_parent_offset(zm), cn );
  }
  else {
    write_word( zm, obj_addr(zm,n) + obj_parent_offset(zm), cn );
  }
}

void obj_unlink( zmachine *zm, zword n ) {
  zword parent = obj_parent( zm, n );
  zword younger_sibling = obj_child( zm, parent );
  zword older_sibling = obj_sibling( zm, n );

  if( n == 0 ) {
    runtime_error( "Attempt to unlink object 0" );
    return;
  }

  if( parent == 0 ) {
    return;
  }

  obj_set_parent( zm, n, 0 );
  obj_set_sibling( zm, n, 0 );

  if( younger_sibling == n ) {
    obj_set_child( zm, parent, older_sibling );
  }
  else {
    zword last;
    do {
      last = younger_sibling;
      younger_sibling = obj_sibling( zm, younger_sibling );
    }
    while( younger_sibling != n );

    obj_set_sibling( zm, last, older_sibling );
  }

}

void z_clear_attr( zmachine *zm ) {

}

/*
 * z_get_child, store the child of an object.
 *
 *      zargs[0] = object
 *
 */

void z_get_child( zmachine *zm ) {
  p_store( zm, obj_child( zm, zm->zargs[0] ) );
}

void z_get_next_prop( zmachine *zm ) {

}

/*
 * z_get_parent, store the parent of an object.
 *
 *      zargs[0] = object
 *
 */

void z_get_parent( zmachine *zm ) {
  p_store( zm, obj_parent( zm, zm->zargs[0] ) );
}

void z_get_prop( zmachine *zm ) {

}

void z_get_prop_addr( zmachine *zm ) {

}

void z_get_prop_len( zmachine *zm ) {

}

/*
 * z_get_sibling, store the sibling of an object.
 *
 *      zargs[0] = object
 *
 */

void z_get_sibling( zmachine *zm ) {
  p_store( zm, obj_sibling( zm, zm->zargs[0] ) );
}

void z_insert_obj( zmachine *zm ) {

}

void z_jin( zmachine *zm ) {

}

void z_put_prop( zmachine *zm ) {

}

/*
 * z_remove_obj, unlink an object from its parent and siblings.
 *
 *      zargs[0] = object
 *
 */

void z_remove_obj( zmachine *zm ) {
  obj_unlink( zm, zm->zargs[0] );
}

void z_set_attr( zmachine *zm ) {

}

void z_test_attr( zmachine *zm ) {

}

