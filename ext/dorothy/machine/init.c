
#include "machine.h"

void Init_machine() {
  Machine = rb_define_class( "Machine", rb_cObject );
  rb_define_alloc_func( Machine, machine_alloc );
  rb_define_method( Machine, "initialize", machine_initialize, 1 );
  rb_define_method( Machine, "program_length", machine_program_length, 0 );
  rb_define_method( Machine, "step", machine_step, 0 );
  rb_define_method( Machine, "finished?", machine_finished, 0 );
  rb_define_method( Machine, "read_byte", machine_read_byte, 1 );
  rb_define_method( Machine, "read_word", machine_read_word, 1 );
  rb_define_method( Machine, "read_string", machine_read_string, 1 );

  Header = rb_define_class( "Header", rb_cObject );

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


  Dictionary = rb_define_class( "Dictionary", rb_cObject );
  
  rb_define_method( Dictionary, "num_word_separators", 
                                dictionary_num_word_separators, 0 );

  rb_define_method( Dictionary, "entry_length", dictionary_entry_length, 0 );
  rb_define_method( Dictionary, "num_entries", dictionary_num_entries, 0 );
  rb_define_method( Dictionary, "entries", dictionary_entries, 0 );

}

