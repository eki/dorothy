
#include "machine.h"

/* Global variable definitions */

VALUE Z;
VALUE Machine;
VALUE Memory;
VALUE Header;
VALUE ObjectTable;
VALUE Object;
VALUE Status;
VALUE Dictionary;
VALUE Entry;
VALUE InputStream;
VALUE Screen;
VALUE Program;
VALUE RuntimeError;
VALUE RandomNumberGenerator;

ID id_new, id_dup, id_srand, id_rand, id_append_op,
   id_set_foreground, id_set_background, id_set_window, id_set_style, id_split,
   id_line_available, id_char_available, id_read_line, id_read_char,
   id_dictionary, id_parse,
   id_score, id_time;

VALUE sym_score, sym_time;

z_op op0_opcodes[0x10];
z_op op1_opcodes[0x10];
z_op var_opcodes[0x40];
z_op ext_opcodes[0x1d];

char* op0_opcode_names[0x10];
char* op1_opcode_names[0x10];
char* var_opcode_names[0x40];
char* ext_opcode_names[0x1d];


/* Initialize extension */

void Init_machine() {
  Z = rb_define_module( "Z" );

  Memory = rb_define_class_under( Z, "Memory", rb_cObject );
  rb_define_alloc_func( Memory, memory_alloc );
  rb_define_method( Memory, "initialize", memory_initialize, 1 );
  rb_define_method( Memory, "initialize_copy", memory_initialize_copy, 1 );
  rb_define_method( Memory, "length", memory_length, 0 );
  rb_define_method( Memory, "dynamic_length", memory_dynamic_length, 0 );
  rb_define_method( Memory, "static_length", memory_static_length, 0 );
  rb_define_method( Memory, "read_byte", memory_read_byte, 1 );
  rb_define_method( Memory, "read_word", memory_read_word, 1 );
  rb_define_method( Memory, "read_string", memory_read_string, 1 );
  rb_define_method( Memory, "_dump_data", memory_dump_data, 0 );
  rb_define_method( Memory, "_load_data", memory_load_data, 1 );

  Machine = rb_define_class_under( Z, "Machine", rb_cObject );
  rb_define_alloc_func( Machine, machine_alloc );
  rb_define_method( Machine, "initialize", machine_initialize, 1 );
  rb_define_method( Machine, "step", machine_step, 0 );
  rb_define_method( Machine, "finished?", machine_finished, 0 );
  rb_define_method( Machine, "pc", machine_pc, 0 );
  rb_define_method( Machine, "marshal_dump", machine_marshal_dump, 0 );
  rb_define_method( Machine, "marshal_load", machine_marshal_load, 1 );

  Program = rb_define_class_under( Z, "Program", rb_cObject );
  rb_define_alloc_func( Program, program_alloc );
  rb_define_method( Program, "initialize", program_initialize, 1 );
  rb_define_method( Program, "checksum", program_checksum, 0 );
  rb_define_method( Program, "version", program_version, 0 );
  rb_define_method( Program, "release", program_release, 0 );
  rb_define_method( Program, "serial", program_serial, 0 );

  ObjectTable = rb_define_class_under( Z, "ObjectTable", rb_cObject );
  rb_define_method( ObjectTable, "[]", object_table_find, 1 );
  rb_define_method( ObjectTable, "each", object_table_each, 0 );

  Object = rb_define_class_under( Z, "Object", rb_cObject );
  rb_define_method( Object, "name", object_name, 0 );
  rb_define_method( Object, "parent", object_parent, 0 );
  rb_define_method( Object, "child", object_child, 0 );
  rb_define_method( Object, "sibling", object_sibling, 0 );
  rb_define_method( Object, "attributes", object_attributes, 0 );

  Status = rb_define_class_under( Z, "Status", rb_cObject );
  rb_define_method( Status, "type", status_type, 0 );
  rb_define_method( Status, "score", status_score, 0 );
  rb_define_method( Status, "turns", status_turns, 0 );
  rb_define_method( Status, "hours", status_hours, 0 );
  rb_define_method( Status, "minutes", status_minutes, 0 );
  rb_define_method( Status, "location", status_location, 0 );

  Header = rb_define_class_under( Z, "Header", rb_cObject );

  rb_define_method( Header, "version", header_version, 0 );

  rb_define_method( Header, "status_line_score?", header_status_line_score, 0 );
  rb_define_method( Header, "status_line_time?", header_status_line_time, 0 );
  rb_define_method( Header, "story_split?", header_story_split, 0 );

  rb_define_method( Header, "status_line_available?", 
                            header_status_line_available, 0 );

  rb_define_method( Header, "screen_splitting_available?",
                            header_screen_splitting_available, 0 );

  rb_define_method( Header, "variable_width_font_default?",
                            header_variable_width_font_default, 0 );

  rb_define_method( Header, "colors_available?", header_colors_available, 0 );

  rb_define_method( Header, "picture_display_available?",
                            header_picture_display_available, 0 );

  rb_define_method( Header, "bold_available?", header_bold_available, 0 );
  rb_define_method( Header, "italics_available?", header_italics_available, 0 );

  rb_define_method( Header, "fixed_width_available?",
                            header_fixed_width_available, 0 );

  rb_define_method( Header, "sound_available?", header_sound_available, 0 );

  rb_define_method( Header, "timed_input_available?", 
                            header_timed_input_available, 0 );

  rb_define_method( Header, "transcripting_on?", header_transcripting_on, 0 );
  rb_define_method( Header, "force_fixed_width?", header_force_fixed_width, 0 );

  rb_define_method( Header, "status_line_redraw?", 
                            header_status_line_redraw, 0 );

  rb_define_method( Header, "wants_pictures?", header_wants_pictures, 0 );
  rb_define_method( Header, "wants_undo?", header_wants_undo, 0 );
  rb_define_method( Header, "wants_mouse?", header_wants_mouse, 0 );
  rb_define_method( Header, "wants_color?", header_wants_color, 0 );
  rb_define_method( Header, "wants_sound?", header_wants_sound, 0 );
  rb_define_method( Header, "wants_mouse?", header_wants_menus, 0 );

  rb_define_method( Header, "status_line_available=",
                            header_set_status_line_available, 1 );

  rb_define_method( Header, "screen_splitting_available=",
                            header_set_screen_splitting_available, 1 );

  rb_define_method( Header, "variable_width_font_default=",
                            header_set_variable_width_font_default, 1 );

  rb_define_method( Header, "colors_available=",
                            header_set_colors_available, 1 );

  rb_define_method( Header, "picture_display_available=",
                            header_set_picture_display_available, 1 );

  rb_define_method( Header, "bold_available=", header_set_bold_available, 1 );

  rb_define_method( Header, "italics_available=",
                            header_set_italics_available, 1 );

  rb_define_method( Header, "fixed_width_available=",
                            header_set_fixed_width_available, 1 );

  rb_define_method( Header, "sound_available=",
                            header_set_sound_available, 1 );

  rb_define_method( Header, "timed_input_available=",
                            header_set_timed_input_available, 1 );

  rb_define_method( Header, "transcripting_on=",
                            header_set_transcripting_on, 1 );

  rb_define_method( Header, "force_fixed_width=",
                            header_set_force_fixed_width, 1 );

  rb_define_method( Header, "status_line_redraw=",
                            header_set_status_line_redraw, 1 );

  rb_define_method( Header, "wants_pictures=", header_set_wants_pictures, 1 );
  rb_define_method( Header, "wants_undo=", header_set_wants_undo, 1 );
  rb_define_method( Header, "wants_mouse=", header_set_wants_mouse, 1 );
  rb_define_method( Header, "wants_color=", header_set_wants_color, 1 );
  rb_define_method( Header, "wants_sound=", header_set_wants_sound, 1 );
  rb_define_method( Header, "wants_menus=", header_set_wants_menus, 1 );

  rb_define_method( Header, "static_memory", header_static_memory, 0 );
  rb_define_method( Header, "high_memory", header_high_memory, 0 );

  rb_define_method( Header, "initial_program_counter",
                            header_initial_program_counter, 0 );

  rb_define_method( Header, "main_routine", header_main_routine, 0 );
  rb_define_method( Header, "dictionary", header_dictionary, 0 );
  rb_define_method( Header, "object_table", header_object_table, 0 );
  rb_define_method( Header, "global_table", header_global_table, 0 );
  rb_define_method( Header, "abbreviations_table",
                            header_abbreviations_table, 0 );

  rb_define_method( Header, "terminating_table", header_terminating_table, 0 );
  rb_define_method( Header, "alphabet_table", header_alphabet_table, 0 );

  rb_define_method( Header, "routines", header_routines, 0 );
  rb_define_method( Header, "strings", header_strings, 0 );

  rb_define_method( Header, "extension_table", header_extension_table, 0 );

  rb_define_method( Header, "extension_table_size", 
                            header_extension_table_size, 0 );

  rb_define_method( Header, "mouse_x", header_mouse_x, 0 );
  rb_define_method( Header, "mouse_y", header_mouse_y, 0 );

  rb_define_method( Header, "mouse_x=", header_set_mouse_x, 1 );
  rb_define_method( Header, "mouse_y=", header_set_mouse_y, 1 );

  rb_define_method( Header, "unicode_table", header_unicode_table, 0 );

  rb_define_method( Header, "program_length", header_program_length, 0 );
  rb_define_method( Header, "program_checksum", header_program_checksum, 0 );

  rb_define_method( Header, "interpreter_number", 
                            header_interpreter_number, 0 );

  rb_define_method( Header, "interpreter_version",
                            header_interpreter_version, 0 );

  rb_define_method( Header, "interpreter_number=",
                            header_set_interpreter_number, 1 );

  rb_define_method( Header, "interpreter_version=",
                            header_set_interpreter_version, 1 );

  rb_define_method( Header, "screen_rows", header_screen_rows, 0 );
  rb_define_method( Header, "screen_cols", header_screen_cols, 0 );

  rb_define_method( Header, "screen_rows=", header_set_screen_rows, 1 );
  rb_define_method( Header, "screen_cols=", header_set_screen_cols, 1 );

  rb_define_method( Header, "screen_width", header_screen_width, 0 );
  rb_define_method( Header, "screen_height", header_screen_height, 0 );

  rb_define_method( Header, "screen_width=", header_set_screen_width, 1 );
  rb_define_method( Header, "screen_height=", header_set_screen_height, 1 );

  rb_define_method( Header, "font_width", header_font_width, 0 );
  rb_define_method( Header, "font_height", header_font_height, 0 );

  rb_define_method( Header, "font_width=", header_set_font_width, 1 );
  rb_define_method( Header, "font_height=", header_set_font_height, 1 );

  rb_define_method( Header, "default_background_color",
                            header_default_background_color, 0 );

  rb_define_method( Header, "default_foreground_color",
                            header_default_foreground_color, 0 );

  rb_define_method( Header, "default_background_color=",
                            header_set_default_background_color, 1 );

  rb_define_method( Header, "default_foreground_color=",
                            header_set_default_foreground_color, 1 );

  rb_define_method( Header, "output_stream3_width",
                            header_output_stream3_width, 0 );

  rb_define_method( Header, "output_stream3_width=",
                            header_set_output_stream3_width, 1 );

  rb_define_method( Header, "standard_major", header_standard_major, 0 );
  rb_define_method( Header, "standard_minor", header_standard_minor, 0 );

  rb_define_method( Header, "standard_major=", header_set_standard_major, 1 );
  rb_define_method( Header, "standard_minor=", header_set_standard_minor, 1 );


  Dictionary = rb_define_class_under( Z, "Dictionary", rb_cObject );
  
  rb_define_method( Dictionary, "load", dictionary_load, 0 );

  Entry = rb_define_class_under( Dictionary, "Entry", rb_cObject );

  InputStream = rb_define_class_under( Machine, "InputStream", rb_cObject );

  Screen = rb_define_class_under( Z, "Screen", rb_cObject );

  RandomNumberGenerator = rb_define_class( "RandomNumberGenerator", rb_cObject);

  RuntimeError = rb_define_class_under( Z, "RuntimeError", rb_eRuntimeError );

  id_new = rb_intern( "new" );
  id_dup = rb_intern( "dup" );
  id_srand = rb_intern( "srand" );
  id_rand = rb_intern( "rand" );
  id_append_op = rb_intern( "<<" );
  id_set_foreground = rb_intern( "foreground=" );
  id_set_background = rb_intern( "background=" );
  id_set_window = rb_intern( "window=" );
  id_set_style = rb_intern( "style=" );
  id_split = rb_intern( "split" );
  id_line_available = rb_intern( "line_available?" );
  id_char_available = rb_intern( "char_available?" );
  id_read_line = rb_intern( "read_line" );
  id_read_char = rb_intern( "read_char" );
  id_dictionary = rb_intern( "dictionary" );
  id_parse = rb_intern( "parse" );
  id_score = rb_intern( "score" );
  id_time = rb_intern( "time" );

  sym_score = ID2SYM(id_score);
  sym_time = ID2SYM(id_time);

  /* setup the z_op arrays */

  op0_opcodes[0]  = z_rtrue;
  op0_opcodes[1]  = z_rfalse;
  op0_opcodes[2]  = z_print;
  op0_opcodes[3]  = z_print_ret;
  op0_opcodes[4]  = z_nop;
  op0_opcodes[5]  = z_save;
  op0_opcodes[6]  = z_restore;
  op0_opcodes[7]  = z_restart;
  op0_opcodes[8]  = z_ret_popped;
  op0_opcodes[9]  = z_catch;
  op0_opcodes[10] = z_quit;
  op0_opcodes[11] = z_new_line;
  op0_opcodes[12] = z_show_status;
  op0_opcodes[13] = z_verify;
  op0_opcodes[14] = z_extended;
  op0_opcodes[15] = z_piracy;

  op0_opcode_names[0]  = "rtrue";
  op0_opcode_names[1]  = "rfalse";
  op0_opcode_names[2]  = "print";
  op0_opcode_names[3]  = "print_ret";
  op0_opcode_names[4]  = "nop";
  op0_opcode_names[5]  = "save";
  op0_opcode_names[6]  = "restore";
  op0_opcode_names[7]  = "restart";
  op0_opcode_names[8]  = "ret_popped";
  op0_opcode_names[9]  = "catch";
  op0_opcode_names[10] = "quit";
  op0_opcode_names[11] = "new_line";
  op0_opcode_names[12] = "show_status";
  op0_opcode_names[13] = "verify";
  op0_opcode_names[14] = "extended";
  op0_opcode_names[15] = "piracy";

  op1_opcodes[0]  = z_jz;
  op1_opcodes[1]  = z_get_sibling;
  op1_opcodes[2]  = z_get_child;
  op1_opcodes[3]  = z_get_parent;
  op1_opcodes[4]  = z_get_prop_len;
  op1_opcodes[5]  = z_inc;
  op1_opcodes[6]  = z_dec;
  op1_opcodes[7]  = z_print_addr;
  op1_opcodes[8]  = z_call_s;
  op1_opcodes[9]  = z_remove_obj;
  op1_opcodes[10] = z_print_obj;
  op1_opcodes[11] = z_ret;
  op1_opcodes[12] = z_jump;
  op1_opcodes[13] = z_print_paddr;
  op1_opcodes[14] = z_load;
  op1_opcodes[15] = z_call_n;

  op1_opcode_names[0]  = "jz";
  op1_opcode_names[1]  = "get_sibling";
  op1_opcode_names[2]  = "get_child";
  op1_opcode_names[3]  = "get_parent";
  op1_opcode_names[4]  = "get_prop_len";
  op1_opcode_names[5]  = "inc";
  op1_opcode_names[6]  = "dec";
  op1_opcode_names[7]  = "print_addr";
  op1_opcode_names[8]  = "call_s";
  op1_opcode_names[9]  = "remove_obj";
  op1_opcode_names[10] = "print_obj";
  op1_opcode_names[11] = "ret";
  op1_opcode_names[12] = "jump";
  op1_opcode_names[13] = "print_paddr";
  op1_opcode_names[14] = "load";
  op1_opcode_names[15] = "call_n";

  var_opcodes[0]  = z_illegal;
  var_opcodes[1]  = z_je;
  var_opcodes[2]  = z_jl;
  var_opcodes[3]  = z_jg;
  var_opcodes[4]  = z_dec_chk;
  var_opcodes[5]  = z_inc_chk;
  var_opcodes[6]  = z_jin;
  var_opcodes[7]  = z_test;
  var_opcodes[8]  = z_or;
  var_opcodes[9]  = z_and;
  var_opcodes[10] = z_test_attr;
  var_opcodes[11] = z_set_attr;
  var_opcodes[12] = z_clear_attr;
  var_opcodes[13] = z_store;
  var_opcodes[14] = z_insert_obj;
  var_opcodes[15] = z_loadw;
  var_opcodes[16] = z_loadb;
  var_opcodes[17] = z_get_prop;
  var_opcodes[18] = z_get_prop_addr;
  var_opcodes[19] = z_get_next_prop;
  var_opcodes[20] = z_add;
  var_opcodes[21] = z_sub;
  var_opcodes[22] = z_mul;
  var_opcodes[23] = z_div;
  var_opcodes[24] = z_mod;
  var_opcodes[25] = z_call_s;
  var_opcodes[26] = z_call_n;
  var_opcodes[27] = z_set_colour;
  var_opcodes[28] = z_throw;
  var_opcodes[29] = z_illegal;
  var_opcodes[30] = z_illegal;
  var_opcodes[31] = z_illegal;
  var_opcodes[32] = z_call_s;
  var_opcodes[33] = z_storew;
  var_opcodes[34] = z_storeb;
  var_opcodes[35] = z_put_prop;
  var_opcodes[36] = z_read;
  var_opcodes[37] = z_print_char;
  var_opcodes[38] = z_print_num;
  var_opcodes[39] = z_random;
  var_opcodes[40] = z_push;
  var_opcodes[41] = z_pull;
  var_opcodes[42] = z_split_window;
  var_opcodes[43] = z_set_window;
  var_opcodes[44] = z_call_s;
  var_opcodes[45] = z_erase_window;
  var_opcodes[46] = z_erase_line;
  var_opcodes[47] = z_set_cursor;
  var_opcodes[48] = z_get_cursor;
  var_opcodes[49] = z_set_text_style;
  var_opcodes[50] = z_buffer_mode;
  var_opcodes[51] = z_output_stream;
  var_opcodes[52] = z_input_stream;
  var_opcodes[53] = z_sound_effect;
  var_opcodes[54] = z_read_char;
  var_opcodes[55] = z_scan_table;
  var_opcodes[56] = z_not;
  var_opcodes[57] = z_call_n;
  var_opcodes[58] = z_call_n;
  var_opcodes[59] = z_tokenise;
  var_opcodes[60] = z_encode_text;
  var_opcodes[61] = z_copy_table;
  var_opcodes[62] = z_print_table;
  var_opcodes[63] = z_check_arg_count;

  var_opcode_names[0]  = "illegal";
  var_opcode_names[1]  = "je";
  var_opcode_names[2]  = "jl";
  var_opcode_names[3]  = "jg";
  var_opcode_names[4]  = "dec_chk";
  var_opcode_names[5]  = "inc_chk";
  var_opcode_names[6]  = "jin";
  var_opcode_names[7]  = "test";
  var_opcode_names[8]  = "or";
  var_opcode_names[9]  = "and";
  var_opcode_names[10] = "test_attr";
  var_opcode_names[11] = "set_attr";
  var_opcode_names[12] = "clear_attr";
  var_opcode_names[13] = "store";
  var_opcode_names[14] = "insert_obj";
  var_opcode_names[15] = "loadw";
  var_opcode_names[16] = "loadb";
  var_opcode_names[17] = "get_prop";
  var_opcode_names[18] = "get_prop_addr";
  var_opcode_names[19] = "get_next_prop";
  var_opcode_names[20] = "add";
  var_opcode_names[21] = "sub";
  var_opcode_names[22] = "mul";
  var_opcode_names[23] = "div";
  var_opcode_names[24] = "mod";
  var_opcode_names[25] = "call_s";
  var_opcode_names[26] = "call_n";
  var_opcode_names[27] = "set_colour";
  var_opcode_names[28] = "throw";
  var_opcode_names[29] = "illegal";
  var_opcode_names[30] = "illegal";
  var_opcode_names[31] = "illegal";
  var_opcode_names[32] = "call_s";
  var_opcode_names[33] = "storew";
  var_opcode_names[34] = "storeb";
  var_opcode_names[35] = "put_prop";
  var_opcode_names[36] = "read";
  var_opcode_names[37] = "print_char";
  var_opcode_names[38] = "print_num";
  var_opcode_names[39] = "random";
  var_opcode_names[40] = "push";
  var_opcode_names[41] = "pull";
  var_opcode_names[42] = "split_window";
  var_opcode_names[43] = "set_window";
  var_opcode_names[44] = "call_s";
  var_opcode_names[45] = "erase_window";
  var_opcode_names[46] = "erase_line";
  var_opcode_names[47] = "set_cursor";
  var_opcode_names[48] = "get_cursor";
  var_opcode_names[49] = "set_text_style";
  var_opcode_names[50] = "buffer_mode";
  var_opcode_names[51] = "output_stream";
  var_opcode_names[52] = "input_stream";
  var_opcode_names[53] = "sound_effect";
  var_opcode_names[54] = "read_char";
  var_opcode_names[55] = "scan_table";
  var_opcode_names[56] = "not";
  var_opcode_names[57] = "call_n";
  var_opcode_names[58] = "call_n";
  var_opcode_names[59] = "tokenise";
  var_opcode_names[60] = "encode_text";
  var_opcode_names[61] = "copy_table";
  var_opcode_names[62] = "print_table";
  var_opcode_names[63] = "check_arg_count";

  ext_opcodes[0]  = z_save;
  ext_opcodes[1]  = z_restore;
  ext_opcodes[2]  = z_log_shift;
  ext_opcodes[3]  = z_art_shift;
  ext_opcodes[4]  = z_set_font;
  ext_opcodes[5]  = z_draw_picture;
  ext_opcodes[6]  = z_picture_data;
  ext_opcodes[7]  = z_erase_picture;
  ext_opcodes[8]  = z_set_margins;
  ext_opcodes[9]  = z_save_undo;
  ext_opcodes[10] = z_restore_undo;
  ext_opcodes[11] = z_print_unicode;
  ext_opcodes[12] = z_check_unicode;
  ext_opcodes[13] = z_illegal;
  ext_opcodes[14] = z_illegal;
  ext_opcodes[15] = z_illegal;
  ext_opcodes[16] = z_move_window;
  ext_opcodes[17] = z_window_size;
  ext_opcodes[18] = z_window_style;
  ext_opcodes[19] = z_get_wind_prop;
  ext_opcodes[20] = z_scroll_window;
  ext_opcodes[21] = z_pop_stack;
  ext_opcodes[22] = z_read_mouse;
  ext_opcodes[23] = z_mouse_window;
  ext_opcodes[24] = z_push_stack;
  ext_opcodes[25] = z_put_wind_prop;
  ext_opcodes[26] = z_print_form;
  ext_opcodes[27] = z_make_menu;
  ext_opcodes[28] = z_picture_table;

  ext_opcode_names[0]  = "save";
  ext_opcode_names[1]  = "restore";
  ext_opcode_names[2]  = "log_shift";
  ext_opcode_names[3]  = "art_shift";
  ext_opcode_names[4]  = "set_font";
  ext_opcode_names[5]  = "draw_picture";
  ext_opcode_names[6]  = "picture_data";
  ext_opcode_names[7]  = "erase_picture";
  ext_opcode_names[8]  = "set_margins";
  ext_opcode_names[9]  = "save_undo";
  ext_opcode_names[10] = "restore_undo";
  ext_opcode_names[11] = "print_unicode";
  ext_opcode_names[12] = "check_unicode";
  ext_opcode_names[13] = "illegal";
  ext_opcode_names[14] = "illegal";
  ext_opcode_names[15] = "illegal";
  ext_opcode_names[16] = "move_window";
  ext_opcode_names[17] = "window_size";
  ext_opcode_names[18] = "window_style";
  ext_opcode_names[19] = "get_wind_prop";
  ext_opcode_names[20] = "scroll_window";
  ext_opcode_names[21] = "pop_stack";
  ext_opcode_names[22] = "read_mouse";
  ext_opcode_names[23] = "mouse_window";
  ext_opcode_names[24] = "push_stack";
  ext_opcode_names[25] = "put_wind_prop";
  ext_opcode_names[26] = "print_form";
  ext_opcode_names[27] = "make_menu";
  ext_opcode_names[28] = "picture_table";
}

