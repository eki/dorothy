
#include "machine.h"

/*
 *  Given the address of an object's property ID, parse out the size of the
 *  property DATA.  If the at_id bool is true, the given address is the start
 *  of the property table, otherwise it's the start of the property data.
 */

zaddr obj_prop_data_size( zmachine *zm, zaddr prop, bool at_id ) {
  if( ! at_id ) {
    prop--;
  }

  zbyte size = read_byte( zm, prop );

  if( h_version(zm) < 4 ) {
    size = (size >> 5) + 1;
  }
  else if( !(size & 0x80) ) {
    size = (size >> 6) + 1;
  }
  else {
    if( at_id ) {
      size = read_byte( zm, prop+1 );
    }

    size &= obj_prop_mask(zm);

    size = size == 0 ? 64 : size;
  }

  return size;
}

/*
 *  Given the address of an object's property ID, parse out the size of the
 *  entire property block (ID+DATA).
 */

zaddr obj_prop_size( zmachine *zm, zaddr prop ) {
  if( h_version(zm) < 4 ) {
    return obj_prop_data_size( zm, prop, true ) + 1;
  }
  else {
    zbyte size = read_byte( zm, prop );
    
    return obj_prop_data_size( zm, prop, true ) + ((size & 0x80) ? 2 : 1);
  }
}

/*
 *  Given the address of an object's property ID, return the address of the
 *  next property's ID.
 */

zaddr obj_next_prop_addr( zmachine *zm, zaddr prop ) {
  return prop + obj_prop_size( zm, prop );
}

/*
 *  Given the address of an object's property ID, return the property's 
 *  number.
 */

zword obj_prop_number( zmachine *zm, zaddr prop ) {
  return read_byte( zm, prop ) & obj_prop_mask(zm);
}

/*
 *  Find the address of property number n of the given object.  The returned
 *  address points at the start of the property ID, not the DATA.  If the
 *  property address couldn't be found 0 is returned.  This function doesn't
 *  do any error checking... bad things could happen if given an illegal
 *  object number.
 */

zaddr obj_find_prop_number( zmachine *zm, zword n, zword pn ) {
  zaddr prop_addr = obj_first_prop_addr( zm, n );
  zword last;

  while( (last = obj_prop_number( zm, prop_addr )) > pn ) {
    prop_addr = obj_next_prop_addr( zm, prop_addr );
  }

  return last == pn ? prop_addr : 0;
}

/*
 *  Given the address of a property, return the address of the property's
 *  DATA.  The data is offset 1 or 2 bytes depending on version / property
 *  format.
 */

zaddr obj_prop_data_addr( zmachine *zm, zaddr prop ) {
  if( h_version(zm) > 3 && (read_byte( zm, prop ) & 0x80) ) {
    prop++;
  }

  return prop + 1;
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
    write_byte( zm, obj_addr(zm,n) + obj_sibling_offset(zm), sn );
  }
  else {
    write_word( zm, obj_addr(zm,n) + obj_sibling_offset(zm), sn );
  }
}

void obj_set_child( zmachine *zm, zword n, zword cn ) {
  if( h_version(zm) < 4 ) {
    write_byte( zm, obj_addr(zm,n) + obj_child_offset(zm), cn );
  }
  else {
    write_word( zm, obj_addr(zm,n) + obj_child_offset(zm), cn );
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

/*
 * z_clear_attr, clear an object attribute.
 *
 *      zargs[0] = object
 *      zargs[1] = number of attribute to be cleared
 *
 */

void z_clear_attr( zmachine *zm ) {
  if( zm->zargs[1] > obj_max_attr(zm) ) {
    runtime_error( "Illegal attribute number" );
  }

  if( zm->zargs[0] == 0 || zm->zargs[0] > obj_max_objects(zm) ) {
    runtime_error( "Illegal object number" );
  }

  obj_clear_attr( zm, zm->zargs[0], zm->zargs[1] );
}

/*
 * z_get_child, store the child of an object.
 *
 *      zargs[0] = object
 *
 */

void z_get_child( zmachine *zm ) {
  zword child = obj_child( zm, zm->zargs[0] );
  p_store( zm, child );
  p_branch( zm, child );
}

/*
 * z_get_next_prop, store the number of the first or next property.
 *
 *      zargs[0] = object
 *      zargs[1] = address of current property (0 gets the first property)
 *
 */

void z_get_next_prop( zmachine *zm ) {
  if( zm->zargs[0] == 0 || zm->zargs[0] > obj_max_objects(zm) ) {
    runtime_error( "Attempt to get next prop of illegal object" );
  }

  zaddr prop_addr = obj_first_prop_addr( zm, zm->zargs[0] );

  if( zm->zargs[1] != 0 ) {
    zword last_number;

    do {

      last_number = obj_prop_number( zm, prop_addr );
      prop_addr = obj_next_prop_addr( zm, prop_addr );

    } while( last_number > zm->zargs[1] );

    if( last_number != zm->zargs[1] ) {
      runtime_error( "Next property does not exist" );
    }
  }

  p_store( zm, obj_prop_number( zm, prop_addr ) );
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

/*
 * z_get_prop, store the value of an object property.
 *
 *      zargs[0] = object
 *      zargs[1] = number of property to be examined
 *
 */

void z_get_prop( zmachine *zm ) {
  if( zm->zargs[0] == 0 || zm->zargs[0] > obj_max_objects(zm) ) {
    runtime_error( "Attempt to get next prop of illegal object" );
  }
  
  zaddr prop_addr = obj_find_prop_number( zm, zm->zargs[0], zm->zargs[1] );
  zword value;

  if( prop_addr == 0 ) {
    value = obj_default_prop( zm, zm->zargs[1] );
  }
  else {
    zword size = obj_prop_data_size( zm, prop_addr, true );

    if( size == 1 ) {
      value = read_byte( zm, obj_prop_data_addr( zm, prop_addr ) );
    }
    else if( size == 2 ) {
      value = read_word( zm, obj_prop_data_addr( zm, prop_addr ) );
    }
    else {
      runtime_error( "Illegal call to get_prop, property size > 2" );
    }
  }

  p_store( zm, value );
}

/*
 * z_get_prop_addr, store the address of an object property.
 *
 *      zargs[0] = object
 *      zargs[1] = number of property to be examined
 *
 */

void z_get_prop_addr( zmachine *zm ) {
  if( zm->zargs[0] == 0 || zm->zargs[0] > obj_max_objects(zm) ) {
    runtime_error( "Attempt to get next prop of illegal object" );
  }

  zaddr prop_addr = obj_find_prop_number( zm, zm->zargs[0], zm->zargs[1] );

  if( prop_addr == 0 ) {
    p_store( zm, 0 );
  }
  else {
    p_store( zm, obj_prop_data_addr( zm, prop_addr ) );
  }
}

/*
 * z_get_prop_len, store the length of an object property.
 *
 *      zargs[0] = address of property to be examined
 *
 */

void z_get_prop_len( zmachine *zm ) {
  p_store( zm, obj_prop_data_size( zm, zm->zargs[0], false ) );
}

/*
 * z_get_sibling, store the sibling of an object.
 *
 *      zargs[0] = object
 *
 */

void z_get_sibling( zmachine *zm ) {
  zword sibling = obj_sibling( zm, zm->zargs[0] );
  p_store( zm, sibling );
  p_branch( zm, sibling );
}

/*
 * z_insert_obj, make an object the first child of another object.
 *
 *      zargs[0] = object to be moved
 *      zargs[1] = destination object
 *
 */

void z_insert_obj( zmachine *zm ) {
  if( zm->zargs[0] == 0 || zm->zargs[0] > obj_max_objects(zm) ) {
    runtime_error( "Attempt to move illegal object" );
  }

  if( zm->zargs[1] == 0 || zm->zargs[1] > obj_max_objects(zm) ) {
    runtime_error( "Attempt to move to illegal object" );
  }

  obj_unlink( zm, zm->zargs[0] );

  zword child = obj_child( zm, zm->zargs[1] );

  obj_set_parent( zm, zm->zargs[0], zm->zargs[1] );
  obj_set_child( zm, zm->zargs[1], zm->zargs[0] );
  obj_set_sibling( zm, zm->zargs[0], child );
}

/*
 * z_jin, branch if the first object is inside the second.
 *
 *      zargs[0] = first object
 *      zargs[1] = second object
 *
 */

void z_jin( zmachine *zm ) {
  if( zm->zargs[0] == 0 || zm->zargs[0] > obj_max_objects(zm) ) {
    runtime_error( "Illegal first object for jin" );
  }

  if( zm->zargs[1] == 0 || zm->zargs[1] > obj_max_objects(zm) ) {
    runtime_error( "Illegal second object for jin" );
  }

  p_branch( zm, obj_parent( zm, zm->zargs[0] ) == zm->zargs[1] );
}

/*
 * z_put_prop, set the value of an object property.
 *
 *      zargs[0] = object
 *      zargs[1] = number of property to set
 *      zargs[2] = value to set property to
 *
 */

void z_put_prop( zmachine *zm ) {
  if( zm->zargs[0] == 0 || zm->zargs[0] > obj_max_objects(zm) ) {
    runtime_error( "Attempt to get next prop of illegal object" );
  }
  
  zaddr prop_addr = obj_find_prop_number( zm, zm->zargs[0], zm->zargs[1] );

  if( prop_addr == 0 ) {
    runtime_error( "Attempt to write to nonexistant property" );
  }
  else {
    zword size = obj_prop_data_size( zm, prop_addr, true );

    if( size == 1 ) {
      write_byte( zm, obj_prop_data_addr( zm, prop_addr ), zm->zargs[2] );
    }
    else if( size == 2 ) {
      write_word( zm, obj_prop_data_addr( zm, prop_addr ), zm->zargs[2] );
    }
    else {
      runtime_error( "Illegal call to put_prop, property size > 2" );
    }
  }
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

/*
 * z_set_attr, set an object attribute.
 *
 *      zargs[0] = object
 *      zargs[1] = number of attribute to set
 *
 */

void z_set_attr( zmachine *zm ) {
  if( zm->zargs[1] > obj_max_attr(zm) ) {
    runtime_error( "Illegal attribute number" );
  }

  if( zm->zargs[0] == 0 || zm->zargs[0] > obj_max_objects(zm) ) {
    runtime_error( "Illegal object number" );
  }

  obj_set_attr( zm, zm->zargs[0], zm->zargs[1] );
}

/*
 * z_test_attr, branch if an object attribute is set.
 *
 *      zargs[0] = object
 *      zargs[1] = number of attribute to test
 *
 */

void z_test_attr( zmachine *zm ) {
  p_branch( zm, obj_attr( zm, zm->zargs[0], zm->zargs[1] ) );
}

