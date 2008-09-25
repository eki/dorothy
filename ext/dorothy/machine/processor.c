
#include "machine.h"

/*
 * load_operand
 *
 * Load an operand, either a variable or a constant.
 *
 */

void load_operand( zmachine *zm, zbyte type ) {
  zword value;

  if( type & 2 ) {                   /* variable */
    value = p_load( zm );
  } 
  else if( type & 1 ) {              /* small constant */
    value = *zm->pcp++;
  } 
  else {                             /* large constant */
    value = ((zword) zm->pcp[0] << 8) | zm->pcp[1];
    zm->pcp += 2;
  }

  zm->zargs[zm->zargc++] = value;
}

/*
 * load_all_operands
 *
 * Given the operand specifier byte, load all (up to four) operands
 * for a VAR or EXT opcode.
 *
 */

void load_all_operands( zmachine *zm, zbyte specifier ) {   
  int i;

  for( i = 6; i >= 0; i -= 2 ) {
    zbyte type = (specifier >> i) & 0x03;

    if( type == 3 ) {
      break;
    }

    load_operand( zm, type );
  }
}

/*
 *  Execute the next instruction.
 */

void p_step( zmachine *zm ) {
  if( zm->finished == 0 ) {
    if( h_version(zm) != V6 ) {
      zm->pcp = zm->program + h_initial_program_counter(zm);
      trace( zm, "first step (pc:%d)\n", PC(zm) );
    } 
    else {
      p_call( zm, h_start_pc(zm), 0, NULL, 0 );
      return;
    }
  }
  else if( zm->finished >= 9999 ) {
    return;
  }

  int pc = PC(zm);

  zbyte opcode = *zm->pcp++;

  zm->zargc = 0;

  if( opcode < 0x80 ) {

    load_operand( zm, (zbyte) (opcode & 0x40) ? 2 : 1 );
    load_operand( zm, (zbyte) (opcode & 0x20) ? 2 : 1 );

    trace( zm, "  (%d) (var:%d) Executing %s\n", pc, (opcode & 0x1f),
           zm->var_opcode_names[opcode & 0x1f] );

    zm->var_opcodes[opcode & 0x1f]( zm );

  }
  else if( opcode < 0xb0 ) {

    load_operand( zm, (zbyte) (opcode >> 4) );

    trace( zm, "  (%d) (op1:%d) Executing %s\n", pc, (opcode & 0x0f),
           zm->op1_opcode_names[opcode & 0x0f] );

    zm->op1_opcodes[opcode & 0x0f]( zm );

  } 
  else if( opcode < 0xc0 ) {

    trace( zm, "  (%d) (op0:%d) Executing %s\n", pc, (opcode - 0xb0),
           zm->op0_opcode_names[opcode - 0xb0] );

    zm->op0_opcodes[opcode - 0xb0]( zm );

  } 
  else {
    if (opcode == 0xec || opcode == 0xfa) { 
      zbyte s1 = *zm->pcp++;       /* NOTE:  must read both specifier bytes */
      zbyte s2 = *zm->pcp++;       /*        before loading the operands!   */
      load_all_operands( zm, s1 );
      load_all_operands( zm, s2 );
    } 
    else {
      load_all_operands( zm, *zm->pcp++ );
    }

    trace( zm, "  (%d) (var:%d) Executing %s\n", pc, (opcode - 0xc0),
           zm->var_opcode_names[opcode - 0xc0] );

    zm->var_opcodes[opcode - 0xc0]( zm );
  }

  zm->finished++;
}

/*
 * p_call
 *
 * Call a subroutine. Save PC and FP then load new PC and initialise
 * new stack frame. Note that the caller may legally provide less or
 * more arguments than the function actually has. The call type "ct"
 * can be 0 (z_call_s), 1 (z_call_n) or 2 (direct call).
 *
 */

void p_call( zmachine *zm, zword routine, int argc, zword *args, int ct ) {   
  long pc;
  zword value;
  zbyte count;
  int i;

  if( zm->sp - zm->stack < 4 ) {
    runtime_error( "stack overflow" );
  }

  pc = PC(zm);

  *--zm->sp = (zword) (pc >> 9);
  *--zm->sp = (zword) (pc & 0x1ff);
  *--zm->sp = (zword) (zm->fp - zm->stack - 1);
  *--zm->sp = (zword) (argc | (ct << 12)); /* save quetzal ??? */

  zm->fp = zm->sp;
  zm->frame_count++;

  /* Calculate byte address of routine */

  if( h_version(zm) <= V3 ) {
    pc = (long) routine << 1;
  }
  else if( h_version(zm) <= V5 ) {
    pc = (long) routine << 2;
  }
  else if( h_version(zm) <= V7 ) {
    pc = ((long) routine << 2) + ((long) h_routines(zm) << 3);
  }
  else {
    pc = (long) routine << 3;
  }

  if( pc >= zm->program_length ) {
    runtime_error( "invalid call address" );
  }

  zm->pcp = zm->program + pc;

  /* Initialise local variables */

  count = *zm->pcp++;

  if( count > 15 ) {
    runtime_error( "call to non-routine" );
  }

  if( zm->sp - zm->stack < count ) {
    runtime_error( "stack overflow" );
  }

  zm->fp[0] |= (zword) count << 8;    /* Save local var count for Quetzal. */

  value = 0;

  for( i = 0; i < count; i++ ) {
    if( h_version(zm) <= V4 ) {       /* V1 to V4 games provide default */
                                      /* values for all local variables */

      value = ((zword) zm->pcp[0] << 8) | zm->pcp[1];
      zm->pcp += 2;
    }

    *--zm->sp = (zword) ((argc-- > 0) ? args[i] : value);
  }

  /* Start main loop for direct calls */
  /*
  if (ct == 2)
    interpret ();
  */
}

/*
 * p_store_global
 *
 * Store a value to a global variable.
 *
 */

void p_store_global( zmachine *zm, zbyte variable, zword value ) {
  zword addr = h_global_table(zm) + 2 * (variable - 16);
  write_word( zm, addr, value );
}

/*
 * p_load_global
 *
 * Fetch a value from a global variable.
 *
 */

zword p_load_global( zmachine *zm, zbyte variable ) {
  zword addr = h_global_table(zm) + 2 * (variable - 16);
  return read_word( zm, addr );
}

/*
 * p_store
 *
 * Store an operand, either as a variable or pushed on the stack.
 *
 */

void p_store( zmachine *zm, zword value ) {
  zbyte variable = *zm->pcp++;

  if( variable == 0 ) {
    *--zm->sp = value;
  }
  else if( variable < 16 ) {
    *(zm->fp - variable) = value;
  }
  else {
    p_store_global( zm, variable, value );
  }
}

/*
 *  p_load
 *
 *  Load an operand.
 */

zword p_load( zmachine *zm ) {
  zword value;
  zbyte variable = *zm->pcp++;

  if( variable == 0 ) {
    value = *zm->sp++;
  }
  else if( variable < 16 ) {
    value = *(zm->fp - variable);
  }
  else {
    value = p_load_global( zm, variable );
  }

  return value;
}

/*
 * ret
 *
 * Return from the current subroutine and restore the previous stack
 * frame. The result may be stored (0), thrown away (1) or pushed on
 * the stack (2). In the latter case a direct call has been finished
 * and we must exit the interpreter loop.
 *
 */

void p_ret( zmachine *zm, zword value ) {
  long pc;
  int ct;

  if( zm->sp > zm->fp ) {
    runtime_error( "stack underflow" );
  }

  zm->sp = zm->fp;

  ct = *zm->sp++ >> 12;
  zm->frame_count--;
  zm->fp = zm->stack + 1 + *zm->sp++;
  pc = *zm->sp++;
  pc = ((long) *zm->sp++ << 9) | pc;

  zm->pcp = zm->program + pc;

  /* Handle resulting value */

  if( ct == 0 ) {
    p_store( zm, value );
  }

  if( ct == 2 ) {
    *--zm->sp = value;
  }

  /* Stop main loop for direct calls */
  /*
  if( ct == 2 ) {
    zm->finished++;
  }
  */
}

/*
 * branch
 *
 * Take a jump after an instruction based on the flag, either true or
 * false. The branch can be short or long; it is encoded in one or two
 * bytes respectively. When bit 7 of the first byte is set, the jump
 * takes place if the flag is true; otherwise it is taken if the flag
 * is false. When bit 6 of the first byte is set, the branch is short;
 * otherwise it is long. The offset occupies the bottom 6 bits of the
 * first byte plus all the bits in the second byte for long branches.
 * Uniquely, an offset of 0 means return false, and an offset of 1 is
 * return true.
 *
 */

void p_branch( zmachine *zm, bool flag ) {
  long pc;
  zword offset;
  zbyte off1;
  zbyte off2;
  zbyte specifier = *zm->pcp++;

  off1 = specifier & 0x3f;

  if( !flag ) {
    specifier ^= 0x80;
  }

  if( ! (specifier & 0x40) ) {             /* it's a long branch */

    if( off1 & 0x20 ) {                    /* propagate sign bit */
      off1 |= 0xc0;
    }

    off2 = *zm->pcp++;
    offset = (off1 << 8) | off2;

  } 
  else {
    offset = off1;                         /* it's a short branch */
  }

  if (specifier & 0x80) {

    if (offset > 1) {                      /* normal branch */
      pc = PC(zm) + ((short) offset - 2);
      zm->pcp = zm->program + pc;
    } 
    else {
      p_ret( zm, offset );                 /* special case, return 0 or 1 */
    }
  }
}

