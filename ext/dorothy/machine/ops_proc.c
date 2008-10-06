
#include "machine.h"

/*
 * z_call_n, call a subroutine and discard its result.
 *
 *      zargs[0] = packed address of subroutine
 *      zargs[1] = first argument (optional)
 *      ...
 *      zargs[7] = seventh argument (optional)
 *
 */

void z_call_n( zmachine *zm ) {
  if( zm->zargs[0] != 0 ) {
    p_call( zm, zm->zargs[0], zm->zargc - 1, zm->zargs + 1, 1 );
  }
}

/*
 * z_call_s, call a subroutine and store its result.
 *
 *      zargs[0] = packed address of subroutine
 *      zargs[1] = first argument (optional)
 *      ...
 *      zargs[7] = seventh argument (optional)
 *
 */

void z_call_s( zmachine *zm ) {   
  if( zm->zargs[0] != 0 ) {
    p_call( zm, zm->zargs[0], zm->zargc - 1, zm->zargs + 1, 0 );
  }
  else {
    p_store( zm, 0 );
  }
}

/*
 * z_catch, store the current stack frame for later use with z_throw.
 *
 *      no zargs used
 *
 */

void z_catch( zmachine *zm ) {
  p_store( zm, zm->frame_count );
}

/*
 * z_check_arg_count, branch if subroutine was called with >= n arg's.
 *
 *      zargs[0] = number of arguments
 *
 */

void z_check_arg_count( zmachine *zm ) {
  if( zm->fp == zm->stack + STACK_SIZE) {
    p_branch( zm, zm->zargs[0] == 0 );
  }
  else {
    p_branch( zm, zm->zargs[0] <= (*zm->fp & 0xff) );
  }
}

/*
 * z_dec, decrement a variable.
 *
 *      zargs[0] = variable to decrement
 *
 */

void z_dec( zmachine *zm ) {
  zword variable = zm->zargs[0];

  if( variable == 0 ) {
    --(*zm->sp);
  }
  else if( variable < 16 ) {
    --(*(zm->fp - variable));
  }
  else {
    p_store_global( zm, variable, p_load_global( zm, variable ) - 1 );
  }
}

/*
 * z_dec_chk, decrement a variable and branch if now less than value.
 *
 *      zargs[0] = variable to decrement
 *      zargs[1] = value to check variable against
 *
 */

void z_dec_chk( zmachine *zm ) {
  zword variable = zm->zargs[0];
  zword value;

  if( variable == 0 ) {
    value = --(*zm->sp);
  }
  else if( variable < 16 ) {
    value = --(*(zm->fp - variable));
  }
  else {
    value = p_load_global( zm, variable ) - 1;
    p_store_global( zm, variable, value );
  }

  p_branch( zm, (short) value < (short) zm->zargs[1] );
}

/*
 *  z_extended, not really an instruction.  Execute an instruction from the
 *  EXT set.
 */

void z_extended( zmachine *zm ) {
  int pc = PC(zm);
  zbyte opcode = *zm->pcp++;

  zm->zargc = 0;

  load_all_operands( zm, *zm->pcp++ );

  if( opcode < 0x1d ) {
    trace( zm, "  (%d) (ext:%d) Executing %s\n", pc, opcode,
           ext_opcode_names[opcode] );

    ext_opcodes[opcode]( zm );
  }
  else {
    runtime_error( "Attempt to execute unknown extended opcode" );
  }
}

/*
 *  z_illegal, not really an instruction.  Stop execution if an illegal
 *  instruction is encountered.
 */

void z_illegal( zmachine *zm ) {
  runtime_error( "Attempt to execute illegal instruction" );
}

/*
 * z_inc, increment a variable.
 *
 *      zargs[0] = variable to increment
 *
 */

void z_inc( zmachine *zm ) {
  zword variable = zm->zargs[0];

  if( variable == 0 ) {
    ++(*zm->sp);
  }
  else if( variable < 16 ) {
    ++(*(zm->fp - variable));
  }
  else {
    p_store_global( zm, variable, p_load_global( zm, variable ) + 1 );
  }
}

/*
 * z_inc_chk, increment a variable and branch if now greater than value.
 *
 *      zargs[0] = variable to increment
 *      zargs[1] = value to check variable against
 *
 */

void z_inc_chk( zmachine *zm ) {
  zword variable = zm->zargs[0];
  zword value;

  if( variable == 0 ) {
    value = ++(*zm->sp);
  }
  else if( variable < 16 ) {
    value = ++(*(zm->fp - variable));
  }
  else {
    value = p_load_global( zm, variable ) + 1;
    p_store_global( zm, variable, value );
  }

  p_branch( zm, (short) value > (short) zm->zargs[1] );
}

/*
 * z_jump, jump unconditionally to the given address.
 *
 *      zargs[0] = PC relative address
 *
 */

void z_jump( zmachine *zm ) {
  long pc = PC(zm);

  pc += (short) zm->zargs[0] - 2;

  if( pc >= zm->program_length ) {
    runtime_error( "jump past end of memory" );
  }

  zm->pcp = zm->program + pc;
}

/*
 * z_load, store the value of a variable.
 *
 *      zargs[0] = variable to store
 *
 */

void z_load( zmachine *zm ) {
  zword variable = zm->zargs[0];
  zword value;

  if( variable == 0 ) {
    value = *zm->sp;
  }
  else if( variable < 16 ) {
    value = *(zm->fp - variable);
  }
  else {
    value = p_load_global( zm, variable );
  }

  p_store( zm, value );
}

/*
 * z_nop, no operation.
 *
 *      no zargs used
 *
 */

void z_nop( zmachine *zm ) {
  /* Do Nothing */
}

/*
 * z_piracy, branch if the story file is a legal copy.
 *
 *      no zargs used
 *
 */

void z_piracy( zmachine *zm ) {
  p_branch( zm, true );
}

/*
 * z_pop, pop a value off the game stack and discard it.
 *
 *      no zargs used
 *
 */

void z_pop( zmachine *zm ) {
  zm->sp++;
}

/*
 * z_pop_stack, pop n values off the game or user stack and discard them.
 *
 *      zargs[0] = number of values to discard
 *      zargs[1] = address of user stack (optional)
 *
 */

void z_pop_stack( zmachine *zm ) {
  if( zm->zargc == 2 ) {
    storew( zm, zm->zargs[1], read_word( zm, zm->zargs[1] ) + zm->zargs[0] );
  }
  else {
    zm->sp += zm->zargs[0];
  }
}

/*
 * z_pull, pop a value off...
 *
 * a) ...the game or a user stack and store it (V6)
 *
 *      zargs[0] = address of user stack (optional)
 *
 * b) ...the game stack and write it to a variable (other than V6)
 *
 *      zargs[0] = variable to write value to
 *
 */

void z_pull( zmachine *zm ) {
  zword value;

  if( h_version(zm) != V6 ) {      /* not a V6 game, pop stack and write */

    zword variable = zm->zargs[0];
    value = *zm->sp++;

    if( variable == 0 ) {
      *zm->sp = value;
    }
    else if( variable < 16 ) {
      *(zm->fp - variable) = value;
    }
    else {
      p_store_global( zm, variable, value );
    }

  }
  else {                           /* it's V6, but is there a user stack? */
    if( zm->zargc == 1 ) {
      zword size = read_word( zm, zm->zargs[0] );
      zword addr = zm->zargs[0];

      size++;
      storew( zm, addr, size);

      addr += 2 * size;
      value = read_word( zm, addr );
    }
    else {
      value = *zm->sp++;
    }

    p_store( zm, value );

  }
}

/*
 * z_push, push a value onto the game stack.
 *
 *      zargs[0] = value to push onto the stack
 *
 */

void z_push( zmachine *zm ) {
  *--zm->sp = zm->zargs[0];
}

/*
 * z_push_stack, push a value onto a user stack then branch if successful.
 *
 *      zargs[0] = value to push onto the stack
 *      zargs[1] = address of user stack
 *
 */

void z_push_stack( zmachine *zm ) {
  zword addr = zm->zargs[1];
  zword size = read_word( zm, addr );

  if( size != 0 ) {
    storew( zm, (zword) (addr + 2 * size), zm->zargs[0] );

    size--;
    storew( zm, addr, size );
  }

  p_branch( zm, size );
}

/*
 * z_quit, stop the program.
 *
 *      no zargs used
 *
 */

void z_quit( zmachine *zm ) {
  zm->finished = 9999;
}

/*
 * z_ret, return from a subroutine with the given value.
 *
 *      zargs[0] = value to return
 *
 */

void z_ret( zmachine *zm ) {
  p_ret( zm, zm->zargs[0] );
}

/*
 * z_ret_popped, return from a subroutine with a value popped off the stack.
 *
 *      no zargs used
 *
 */

void z_ret_popped( zmachine *zm ) {
  p_ret( zm, *zm->sp++ );
}

/*
 * z_rfalse, return from a subroutine with false (0).
 *
 *      no zargs used
 *
 */

void z_rfalse( zmachine *zm ) {
  p_ret( zm, 0 );
}

/*
 * z_rtrue, return from a subroutine with true (1).
 *
 *      no zargs used
 *
 */

void z_rtrue( zmachine *zm ) {
  p_ret( zm, 1 );
}

/*
 * z_store, write a value to a variable.
 *
 *      zargs[0] = variable to be written to
 *      zargs[1] = value to write
 *
 */

void z_store( zmachine *zm ) {
  zword variable = zm->zargs[0];
  zword value = zm->zargs[1];

  if( variable == 0 )
    *zm->sp = value;
  else if( variable < 16 ) {
    *(zm->fp - variable) = value;
  }
  else {
    p_store_global( zm, variable, value );
  }
}

/*
 * z_throw, go back to the given stack frame and return the given value.
 *
 *      zargs[0] = value to return
 *      zargs[1] = stack frame
 *
 */

void z_throw( zmachine *zm ) {
  if( zm->zargs[1] > zm->frame_count) {
    runtime_error( "bad frame" );
  }

  /* Unwind the stack a frame at a time. */
  for( ; zm->frame_count > zm->zargs[1]; --(zm->frame_count) ) {
    zm->fp = zm->stack + 1 + zm->fp[1];
  }

  p_ret( zm, zm->zargs[0] );
}

