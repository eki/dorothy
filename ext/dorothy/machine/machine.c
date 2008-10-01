
#include <stdio.h>
#include "machine.h"

/*
 *  Allocate memory for a Machine.
 */

VALUE machine_alloc( VALUE klass ) {
  zmachine *zm = ALLOC( zmachine );
  VALUE obj = Data_Wrap_Struct( klass, 0, machine_free, zm );
}

/*
 *  Initialize the machine.  Loads a story file into memory.
 */

VALUE machine_initialize( VALUE self, VALUE filename ) {
  VALUE header;
  long i;
  char *fn = StringValuePtr( filename );
  FILE *fp;
  zmachine *zm;

  Data_Get_Struct( self, zmachine, zm );

  zm->self = self;

  fp = fopen( fn, "rb" );

  if( fp == NULL ) {
    rb_raise( rb_eRuntimeError, "Unable to open file: %s", fn );
  }

  zm->program = ALLOC_N( zbyte, 64 );

  if( fread( zm->program, 1, 64, fp ) != 64 ) {
    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading header: %s", fn );
  }

  /* Set the actual program length (don't trust the program header) */

  fseek( fp, 0, SEEK_END );
  zm->program_length = ftell( fp );
  fseek( fp, 0, SEEK_SET );

  /* Read the entire program into memory */

  REALLOC_N( zm->program, zbyte, zm->program_length );

  if( fread( zm->program, 1, zm->program_length, fp ) != zm->program_length ) {
    fclose( fp );
    rb_raise( rb_eRuntimeError, "Error reading program: %s", fn );
  }

  fclose( fp );

  zm->program_checksum = 0;
  for( i = 64; i < zm->program_length; i++ ) {
    zm->program_checksum += zm->program[i];
  }

  zm->finished = 0;

  zm->sp = zm->fp = zm->stack + STACK_SIZE;
  zm->frame_count = 0;


  /* Now that the program is loaded, finish setting up the Ruby stuff */

  rb_iv_set( self, "@header", rb_funcall( Header, id_new, 1, self ) );

  rb_iv_set( self, "@output", rb_ary_new() );

  rb_iv_set( self, "@keyboard",
    rb_funcall( InputStream, id_new, 1, UINT2NUM(0) ) );

  rb_iv_set( self, "@trace", rb_ary_new() );

  rb_iv_set( self, "@dictionary", 
    rb_funcall( Dictionary, id_new, 2, self, UINT2NUM(h_dictionary(zm)) ) );


  /* setup the z_op arrays */

  zm->op0_opcodes[0]  = z_rtrue;
  zm->op0_opcodes[1]  = z_rfalse;
  zm->op0_opcodes[2]  = z_print;
  zm->op0_opcodes[3]  = z_print_ret;
  zm->op0_opcodes[4]  = z_nop;
  zm->op0_opcodes[5]  = z_save;
  zm->op0_opcodes[6]  = z_restore;
  zm->op0_opcodes[7]  = z_restart;
  zm->op0_opcodes[8]  = z_ret_popped;
  zm->op0_opcodes[9]  = z_catch;
  zm->op0_opcodes[10] = z_quit;
  zm->op0_opcodes[11] = z_new_line;
  zm->op0_opcodes[12] = z_show_status;
  zm->op0_opcodes[13] = z_verify;
  zm->op0_opcodes[14] = z_extended;
  zm->op0_opcodes[15] = z_piracy;

  zm->op0_opcode_names[0]  = "rtrue";
  zm->op0_opcode_names[1]  = "rfalse";
  zm->op0_opcode_names[2]  = "print";
  zm->op0_opcode_names[3]  = "print_ret";
  zm->op0_opcode_names[4]  = "nop";
  zm->op0_opcode_names[5]  = "save";
  zm->op0_opcode_names[6]  = "restore";
  zm->op0_opcode_names[7]  = "restart";
  zm->op0_opcode_names[8]  = "ret_popped";
  zm->op0_opcode_names[9]  = "catch";
  zm->op0_opcode_names[10] = "quit";
  zm->op0_opcode_names[11] = "new_line";
  zm->op0_opcode_names[12] = "show_status";
  zm->op0_opcode_names[13] = "verify";
  zm->op0_opcode_names[14] = "extended";
  zm->op0_opcode_names[15] = "piracy";

  zm->op1_opcodes[0]  = z_jz;
  zm->op1_opcodes[1]  = z_get_sibling;
  zm->op1_opcodes[2]  = z_get_child;
  zm->op1_opcodes[3]  = z_get_parent;
  zm->op1_opcodes[4]  = z_get_prop_len;
  zm->op1_opcodes[5]  = z_inc;
  zm->op1_opcodes[6]  = z_dec;
  zm->op1_opcodes[7]  = z_print_addr;
  zm->op1_opcodes[8]  = z_call_s;
  zm->op1_opcodes[9] = z_remove_obj;
  zm->op1_opcodes[10] = z_print_obj;
  zm->op1_opcodes[11] = z_ret;
  zm->op1_opcodes[12] = z_jump;
  zm->op1_opcodes[13] = z_print_paddr;
  zm->op1_opcodes[14] = z_load;
  zm->op1_opcodes[15] = z_call_n;

  zm->op1_opcode_names[0]  = "jz";
  zm->op1_opcode_names[1]  = "get_sibling";
  zm->op1_opcode_names[2]  = "get_child";
  zm->op1_opcode_names[3]  = "get_parent";
  zm->op1_opcode_names[4]  = "get_prop_len";
  zm->op1_opcode_names[5]  = "inc";
  zm->op1_opcode_names[6]  = "dec";
  zm->op1_opcode_names[7]  = "print_addr";
  zm->op1_opcode_names[8]  = "call_s";
  zm->op1_opcode_names[9] = "remove_obj";
  zm->op1_opcode_names[10] = "print_obj";
  zm->op1_opcode_names[11] = "ret";
  zm->op1_opcode_names[12] = "jump";
  zm->op1_opcode_names[13] = "print_paddr";
  zm->op1_opcode_names[14] = "load";
  zm->op1_opcode_names[15] = "call_n";

  zm->var_opcodes[0]  = z_illegal;
  zm->var_opcodes[1]  = z_je;
  zm->var_opcodes[2]  = z_jl;
  zm->var_opcodes[3]  = z_jg;
  zm->var_opcodes[4]  = z_dec_chk;
  zm->var_opcodes[5]  = z_inc_chk;
  zm->var_opcodes[6]  = z_jin;
  zm->var_opcodes[7]  = z_test;
  zm->var_opcodes[8]  = z_or;
  zm->var_opcodes[9]  = z_and;
  zm->var_opcodes[10] = z_test_attr;
  zm->var_opcodes[11] = z_set_attr;
  zm->var_opcodes[12] = z_clear_attr;
  zm->var_opcodes[13] = z_store;
  zm->var_opcodes[14] = z_insert_obj;
  zm->var_opcodes[15] = z_loadw;
  zm->var_opcodes[16] = z_loadb;
  zm->var_opcodes[17] = z_get_prop;
  zm->var_opcodes[18] = z_get_prop_addr;
  zm->var_opcodes[19] = z_get_next_prop;
  zm->var_opcodes[20] = z_add;
  zm->var_opcodes[21] = z_sub;
  zm->var_opcodes[22] = z_mul;
  zm->var_opcodes[23] = z_div;
  zm->var_opcodes[24] = z_mod;
  zm->var_opcodes[25] = z_call_s;
  zm->var_opcodes[26] = z_call_n;
  zm->var_opcodes[27] = z_set_colour;
  zm->var_opcodes[28] = z_throw;
  zm->var_opcodes[29] = z_illegal;
  zm->var_opcodes[30] = z_illegal;
  zm->var_opcodes[31] = z_illegal;
  zm->var_opcodes[32] = z_call_s;
  zm->var_opcodes[33] = z_storew;
  zm->var_opcodes[34] = z_storeb;
  zm->var_opcodes[35] = z_put_prop;
  zm->var_opcodes[36] = z_read;
  zm->var_opcodes[37] = z_print_char;
  zm->var_opcodes[38] = z_print_num;
  zm->var_opcodes[39] = z_random;
  zm->var_opcodes[40] = z_push;
  zm->var_opcodes[41] = z_pull;
  zm->var_opcodes[42] = z_split_window;
  zm->var_opcodes[43] = z_set_window;
  zm->var_opcodes[44] = z_call_s;
  zm->var_opcodes[45] = z_erase_window;
  zm->var_opcodes[46] = z_erase_line;
  zm->var_opcodes[47] = z_set_cursor;
  zm->var_opcodes[48] = z_get_cursor;
  zm->var_opcodes[49] = z_set_text_style;
  zm->var_opcodes[50] = z_buffer_mode;
  zm->var_opcodes[51] = z_output_stream;
  zm->var_opcodes[52] = z_input_stream;
  zm->var_opcodes[53] = z_sound_effect;
  zm->var_opcodes[54] = z_read_char;
  zm->var_opcodes[55] = z_scan_table;
  zm->var_opcodes[56] = z_not;
  zm->var_opcodes[57] = z_call_n;
  zm->var_opcodes[58] = z_call_n;
  zm->var_opcodes[59] = z_tokenise;
  zm->var_opcodes[60] = z_encode_text;
  zm->var_opcodes[61] = z_copy_table;
  zm->var_opcodes[62] = z_print_table;
  zm->var_opcodes[63] = z_check_arg_count;

  zm->var_opcode_names[0]  = "illegal";
  zm->var_opcode_names[1]  = "je";
  zm->var_opcode_names[2]  = "jl";
  zm->var_opcode_names[3]  = "jg";
  zm->var_opcode_names[4]  = "dec_chk";
  zm->var_opcode_names[5]  = "inc_chk";
  zm->var_opcode_names[6]  = "jin";
  zm->var_opcode_names[7]  = "test";
  zm->var_opcode_names[8]  = "or";
  zm->var_opcode_names[9] = "and";
  zm->var_opcode_names[10] = "test_attr";
  zm->var_opcode_names[11] = "set_attr";
  zm->var_opcode_names[12] = "clear_attr";
  zm->var_opcode_names[13] = "store";
  zm->var_opcode_names[14] = "insert_obj";
  zm->var_opcode_names[15] = "loadw";
  zm->var_opcode_names[16] = "loadb";
  zm->var_opcode_names[17] = "get_prop";
  zm->var_opcode_names[18] = "get_prop_addr";
  zm->var_opcode_names[19] = "get_next_prop";
  zm->var_opcode_names[20] = "add";
  zm->var_opcode_names[21] = "sub";
  zm->var_opcode_names[22] = "mul";
  zm->var_opcode_names[23] = "div";
  zm->var_opcode_names[24] = "mod";
  zm->var_opcode_names[25] = "call_s";
  zm->var_opcode_names[26] = "call_n";
  zm->var_opcode_names[27] = "set_colour";
  zm->var_opcode_names[28] = "throw";
  zm->var_opcode_names[29] = "illegal";
  zm->var_opcode_names[30] = "illegal";
  zm->var_opcode_names[31] = "illegal";
  zm->var_opcode_names[32] = "call_s";
  zm->var_opcode_names[33] = "storew";
  zm->var_opcode_names[34] = "storeb";
  zm->var_opcode_names[35] = "put_prop";
  zm->var_opcode_names[36] = "read";
  zm->var_opcode_names[37] = "print_char";
  zm->var_opcode_names[38] = "print_num";
  zm->var_opcode_names[39] = "random";
  zm->var_opcode_names[40] = "push";
  zm->var_opcode_names[41] = "pull";
  zm->var_opcode_names[42] = "split_window";
  zm->var_opcode_names[43] = "set_window";
  zm->var_opcode_names[44] = "call_s";
  zm->var_opcode_names[45] = "erase_window";
  zm->var_opcode_names[46] = "erase_line";
  zm->var_opcode_names[47] = "set_cursor";
  zm->var_opcode_names[48] = "get_cursor";
  zm->var_opcode_names[49] = "set_text_style";
  zm->var_opcode_names[50] = "buffer_mode";
  zm->var_opcode_names[51] = "output_stream";
  zm->var_opcode_names[52] = "input_stream";
  zm->var_opcode_names[53] = "sound_effect";
  zm->var_opcode_names[54] = "read_char";
  zm->var_opcode_names[55] = "scan_table";
  zm->var_opcode_names[56] = "not";
  zm->var_opcode_names[57] = "call_n";
  zm->var_opcode_names[58] = "call_n";
  zm->var_opcode_names[59] = "tokenise";
  zm->var_opcode_names[60] = "encode_text";
  zm->var_opcode_names[61] = "copy_table";
  zm->var_opcode_names[62] = "print_table";
  zm->var_opcode_names[63] = "check_arg_count";

  zm->ext_opcodes[0]  = z_save;
  zm->ext_opcodes[1]  = z_restore;
  zm->ext_opcodes[2]  = z_log_shift;
  zm->ext_opcodes[3]  = z_art_shift;
  zm->ext_opcodes[4]  = z_set_font;
  zm->ext_opcodes[5]  = z_draw_picture;
  zm->ext_opcodes[6]  = z_picture_data;
  zm->ext_opcodes[7]  = z_erase_picture;
  zm->ext_opcodes[8]  = z_set_margins;
  zm->ext_opcodes[9] = z_save_undo;
  zm->ext_opcodes[10] = z_restore_undo;
  zm->ext_opcodes[11] = z_print_unicode;
  zm->ext_opcodes[12] = z_check_unicode;
  zm->ext_opcodes[13] = z_illegal;
  zm->ext_opcodes[14] = z_illegal;
  zm->ext_opcodes[15] = z_illegal;
  zm->ext_opcodes[16] = z_move_window;
  zm->ext_opcodes[17] = z_window_size;
  zm->ext_opcodes[18] = z_window_style;
  zm->ext_opcodes[19] = z_get_wind_prop;
  zm->ext_opcodes[20] = z_scroll_window;
  zm->ext_opcodes[21] = z_pop_stack;
  zm->ext_opcodes[22] = z_read_mouse;
  zm->ext_opcodes[23] = z_mouse_window;
  zm->ext_opcodes[24] = z_push_stack;
  zm->ext_opcodes[25] = z_put_wind_prop;
  zm->ext_opcodes[26] = z_print_form;
  zm->ext_opcodes[27] = z_make_menu;
  zm->ext_opcodes[28] = z_picture_table;

  zm->ext_opcode_names[0]  = "save";
  zm->ext_opcode_names[1]  = "restore";
  zm->ext_opcode_names[2]  = "log_shift";
  zm->ext_opcode_names[3]  = "art_shift";
  zm->ext_opcode_names[4]  = "set_font";
  zm->ext_opcode_names[5]  = "draw_picture";
  zm->ext_opcode_names[6]  = "picture_data";
  zm->ext_opcode_names[7]  = "erase_picture";
  zm->ext_opcode_names[8]  = "set_margins";
  zm->ext_opcode_names[9] = "save_undo";
  zm->ext_opcode_names[10] = "restore_undo";
  zm->ext_opcode_names[11] = "print_unicode";
  zm->ext_opcode_names[12] = "check_unicode";
  zm->ext_opcode_names[13] = "illegal";
  zm->ext_opcode_names[14] = "illegal";
  zm->ext_opcode_names[15] = "illegal";
  zm->ext_opcode_names[16] = "move_window";
  zm->ext_opcode_names[17] = "window_size";
  zm->ext_opcode_names[18] = "window_style";
  zm->ext_opcode_names[19] = "get_wind_prop";
  zm->ext_opcode_names[20] = "scroll_window";
  zm->ext_opcode_names[21] = "pop_stack";
  zm->ext_opcode_names[22] = "read_mouse";
  zm->ext_opcode_names[23] = "mouse_window";
  zm->ext_opcode_names[24] = "push_stack";
  zm->ext_opcode_names[25] = "put_wind_prop";
  zm->ext_opcode_names[26] = "print_form";
  zm->ext_opcode_names[27] = "make_menu";
  zm->ext_opcode_names[28] = "picture_table";

  if( h_version(zm) <= V4 ) {
    zm->op0_opcodes[0x09] = z_pop;
    zm->op1_opcodes[0x0f] = z_not;
    zm->op0_opcode_names[0x09] = "pop";
    zm->op1_opcode_names[0x0f] = "not";
  }
  else {
    zm->op0_opcodes[0x09] = z_catch;
    zm->op1_opcodes[0x0f] = z_call_n;
    zm->op0_opcode_names[0x09] = "catch";
    zm->op1_opcode_names[0x0f] = "call_n";
  }

}

/*
 *  Free the memory used by a Machine.  
 */

void machine_free( void *p ) {
  free( p );
}

/*
 *  Get the actual program_length (may be different from the length reported
 *  by the header.
 */

VALUE machine_program_length( VALUE self ) {
  zmachine *zm;

  Data_Get_Struct( self, zmachine, zm );

  return INT2NUM(zm->program_length);
}

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

VALUE machine_read_byte( VALUE self, VALUE addr ) {
  zmachine *zm;
  zaddr a = (zaddr) NUM2INT(addr);

  Data_Get_Struct( self, zmachine, zm );

  return INT2NUM(read_byte( zm, a ));
}

VALUE machine_read_word( VALUE self, VALUE addr ) {
  zmachine *zm;
  zaddr a = (zaddr) NUM2INT(addr);

  Data_Get_Struct( self, zmachine, zm );

  return INT2NUM(read_word( zm, a ));
}

VALUE machine_read_string_array( VALUE self, VALUE addr, VALUE length ) {
  zmachine *zm;
  zaddr a = (zaddr) NUM2INT(addr);

  Data_Get_Struct( self, zmachine, zm );

  zbyte zs[2] = { 5, 0 };
  zbyte *c = &zs[0];
  VALUE str = rb_str_new2( "" );
  int len = NUM2INT(length);
  int i;

  for( i = 0; i < len; i++ ) {
    *c = translate_from_zscii( zm, read_byte( zm, a + i ) );

    rb_str_append( str, rb_str_new2( c ) );
  }

  return str;
}

/*
 *  This is the start of code that supports decoding strings from the
 *  Z-machine into Ruby Strings.  This is a several step process.  The
 *  strings are stored in z-encoded format.  After they've been decoded
 *  they need to be translated from zscii to ascii (technically they should
 *  be converted to unicode...).
 */

static zchar zscii_to_latin1[] = {
    0xe4, 0xf6, 0xfc, 0xc4, 0xd6, 0xdc, 0xdf, 0xbb,
    0xab, 0xeb, 0xef, 0xff, 0xcb, 0xcf, 0xe1, 0xe9,
    0xed, 0xf3, 0xfa, 0xfd, 0xc1, 0xc9, 0xcd, 0xd3,
    0xda, 0xdd, 0xe0, 0xe8, 0xec, 0xf2, 0xf9, 0xc0,
    0xc8, 0xcc, 0xd2, 0xd9, 0xe2, 0xea, 0xee, 0xf4,
    0xfb, 0xc2, 0xca, 0xce, 0xd4, 0xdb, 0xe5, 0xc5,
    0xf8, 0xd8, 0xe3, 0xf1, 0xf5, 0xc3, 0xd1, 0xd5,
    0xe6, 0xc6, 0xe7, 0xc7, 0xfe, 0xf0, 0xde, 0xd0,
    0xa3, 0x00, 0x00, 0xa1, 0xbf
};

zchar translate_from_zscii( zmachine *zm, zbyte c ) {   
  zaddr unicode = h_unicode_table( zm );

  if( c == 0xfc ) {
    return ZC_MENU_CLICK;
  }

  if( c == 0xfd ) {
    return ZC_DOUBLE_CLICK;
  }

  if( c == 0xfe ) {
    return ZC_SINGLE_CLICK;
  }

  if( c >= 0x9b ) {

    if( unicode != 0 ) {    /* game has its own Unicode table */

      zbyte N = read_byte( zm, unicode );

      if (c - 0x9b < N) {

        zaddr addr = unicode + 1 + 2 * (c - 0x9b);
        zword uc = read_word( zm, addr );

        return (uc < 0x100) ? (zchar) uc : '?';
      }
      else {
        return '?';
      }
    }
    else {                        /* game uses standard set */

      if( c <= 0xdf ) {

        if( c == 0xdc || c == 0xdd ) {   /* Oe and oe ligatures */
          return '?';                    /* are not ISO-Latin 1 */
        }

        return zscii_to_latin1[c - 0x9b];
      } 
      else {
        return '?';
      }
    }
  }

  return c;
}

zbyte translate_to_zscii( zmachine *zm, zchar c ) {
  zaddr unicode = h_unicode_table( zm );
  int i;

  if( c == ZC_SINGLE_CLICK ) {
    return 0xfe;
  }

  if( c == ZC_DOUBLE_CLICK ) {
    return 0xfd;
  }

  if( c == ZC_MENU_CLICK ) {
    return 0xfc;
  }

  if( c >= ZC_LATIN1_MIN ) {

    if( unicode != 0 ) {    /* game has its own Unicode table */
      zbyte N = read_byte( zm, unicode );

      for( i = 0x9b; i < 0x9b + N; i++ ) {
        if( c == read_word( zm, unicode + 1 + 2 * (i - 0x9b) ) ) {
          return (zbyte) i;
        }
      }

      return '?';
    } 
    else {                        /* game uses standard set */

      for( i = 0x9b; i <= 0xdf; i++ ) {
        if( c == zscii_to_latin1[i - 0x9b] ) {
          return (zbyte) i;
        }
      }

      return '?';
    }
  }

  if( c == 0 ) {    /* Safety thing from David Kinder */  
    c = '?';        /* regarding his Unicode patches */
  }                 /* Sept 15, 2002 */

  return c;
}

static zchar alphabet( zmachine *zm, int set, int index ) {   
  int version = h_version( zm );
  zaddr alphabet = h_alphabet_table( zm );

  if( alphabet != 0 ) {      /* game uses its own alphabet */
      trace( zm, "Game uses its own alphabet!\n" );
      zbyte c = read_byte( zm, alphabet + set * 26 + index );
      return translate_from_zscii( zm, c );

  } 
  else {                    /* game uses default alphabet */
    if( set == 0 ) {
      return 'a' + index;
    }
    else if( set == 1 ) {
      return 'A' + index;
    }
    else if( version == V1 ) {
      return " 0123456789.,!?_#'\"/\\<-:()"[index];
    }
    else {
      return " ^0123456789.,!?_#'\"/\\-:()"[index];
    }
  }
}


VALUE machine_read_string( VALUE self, VALUE a ) {
  zmachine *zm;
  Data_Get_Struct( self, zmachine, zm );

  long addr = NUM2LONG(a);
  bool consume = false;
  int limit;

  if( addr > 0 ) {
    trace( zm, "read_string called with addr (%d)\n", addr );
  }
  else if( addr == -1 ) {
    addr = PC(zm);
    consume = true;
    trace( zm, "read_string called with PC (%d)\n", addr );
  }

  int version = h_version( zm );
  zaddr abbreviations = h_abbreviations_table( zm );

  zword w;
  zbyte zs[2] = { 5, 0 };
  zbyte *c = &zs[0];
  zbyte last_c = 0;

  zaddr a_addr;
  VALUE a_str;

  int shift_state = 0;
  int shift_lock = 0;
  int status = 0;

  VALUE str = rb_str_new2( "" );
  do {
    int i;
    w = read_word( zm, addr );
    addr += 2;

    if( consume ) {
      zm->pcp += 2;
    }

    trace( zm, "  raw word: %x\n", w );

    for( i = 10; i >= 0; i -= 5 ) {
      *c = (w >> i) & 0x1f;

      trace( zm, "  raw c: %2d    ", *c );

      switch( status ) {
        case 0:
          if( shift_state == 2 && *c == 6 ) {
            status = 2;

            trace( zm, "  status 2\n" );
          }
          else if( version == V1 && *c == 1 ) {
            *c = '\n';
            rb_str_append( str, rb_str_new2( c ) );

            trace( zm, "  (newline) read c: %s\n", c ); 
          }
          else if( version >= V2 && shift_state == 2 && *c == 7 ) {
            *c = '\n';
            rb_str_append( str, rb_str_new2( c ) );

            trace( zm, "  (newline) read c: %s\n", c ); 
          }
          else if( *c >= 6 ) {
            *c = alphabet( zm, shift_state, *c - 6 );
            rb_str_append( str, rb_str_new2( c ) );

            trace( zm, "  (alpha) read c: %s\n", c ); 
          }
          else if( *c == 0 ) {
            *c = ' ';
            rb_str_append( str, rb_str_new2( c ) );

            trace( zm, "  (space) read c: %s\n", c ); 
          }
          else if( (version >= V2 && *c == 1) ||
                   (version >= V3 && *c <= 3) ) {

            status = 1;

            trace( zm, "status 1\n" );
          }
          else {
             shift_state = (shift_lock + (*c & 1) + 1) % 3;

             trace( zm, "  shift_state %d\n", shift_state );

             if( version <= V2 && *c >= 4 ) {
               shift_lock = shift_state;
             }

             break;
          }

          shift_state = shift_lock;
          break;

        case 1:

          a_addr = abbreviations + (32 * (last_c - 1) + *c) * 2;
          a_addr = read_word( zm, a_addr ) << 1;  /* word address! */

          trace( zm, "recursing to look up abbreviation (%d)\n", a_addr );

          a_str = machine_read_string( self, INT2NUM((long) a_addr) );
          rb_str_append( str, a_str );

          status = 0;
          break;

        case 2:

          trace( zm, "  status 3\n" );
          status = 3;
          break;

        case 3:

          *c = translate_from_zscii( zm, (zbyte) ((last_c << 5) | *c) );
          rb_str_append( str, rb_str_new2( c ) );

          trace( zm, "  (10bit) read c: %s\n", c ); 

          status = 0;
          break;
      }

      last_c = *c;


    }
  }
  while( !(w & 0x8000) );

  trace( zm, "returing str\n" );

  return str;
}

