
#include "machine.h"

long h_program_length( zmemory *m ) {
  long length = (long) read_word( m, 0x1a ) * 2;
  zbyte version = h_version( m );

  if( version >= V4 ) {
    length *= 2;
  }

  if( version >= V6 ) {
    length *= 2;
  }
  
  return length;
}

void h_set_status_line_available( zmemory *m, bool b ) {

}

void h_set_screen_splitting_available( zmemory *m, bool b ) {

}

void h_set_variable_width_font_default( zmemory *m, bool b ) {

}

void h_set_colors_available( zmemory *m, bool b ) {

}

void h_set_picture_display_available( zmemory *m, bool b ) {

}

void h_set_bold_available( zmemory *m, bool b ) {

}

void h_set_italics_available( zmemory *m, bool b ) {

}

void h_set_fixed_width_available( zmemory *m, bool b ) {

}

void h_set_sound_available( zmemory *m, bool b ) {

}

void h_set_timed_input_available( zmemory *m, bool b ) {

}


void h_set_transcripting_on( zmemory *m, bool b ) {

}

void h_set_force_fixed_width( zmemory *m, bool b ) {

}

void h_set_status_line_redraw( zmemory *m, bool b ) {

}

void h_set_wants_pictures( zmemory *m, bool b ) {

}

void h_set_wants_undo( zmemory *m, bool b ) {

}

void h_set_wants_mouse( zmemory *m, bool b ) {

}

void h_set_wants_color( zmemory *m, bool b ) {

}

void h_set_wants_sound( zmemory *m, bool b ) {

}

void h_set_wants_menus( zmemory *m, bool b ) {

}


void h_set_mouse_x( zmemory *m, int x ) {

}

void h_set_mouse_y( zmemory *m, int y ) {

}


void h_set_interpreter_number( zmemory *m, int n ) {

}

void h_set_interpreter_version( zmemory *m, int v ) {

}


void h_set_screen_rows( zmemory *m, int r ) {

}

void h_set_screen_cols( zmemory *m, int c ) {

}


void h_set_screen_width( zmemory *m, int w ) {

}

void h_set_screen_height( zmemory *m, int h ) {

}


void h_set_font_width( zmemory *m, int w ) {

}

void h_set_font_height( zmemory *m, int h ) {

}


void h_set_default_background_color( zmemory *m, int c ) {

}

void h_set_default_foreground_color( zmemory *m, int c ) {

}


void h_set_output_stream3_width( zmemory *m, int w ) {

}


void h_set_standard_major( zmemory *m, int n ) {

}

void h_set_standard_minor( zmemory *m, int n ) {

}



VALUE header_version( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_version( m ));
}


VALUE header_status_line_score( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_status_line_score( m ) ? Qtrue : Qfalse;
}

VALUE header_status_line_time( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_status_line_time( m ) ? Qtrue : Qfalse;
}

VALUE header_story_split( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_story_split( m ) ? Qtrue : Qfalse;
}

VALUE header_status_line_available( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_status_line_available( m ) ? Qtrue : Qfalse;
}

VALUE header_screen_splitting_available( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_screen_splitting_available( m ) ? Qtrue : Qfalse;
}

VALUE header_variable_width_font_default( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_variable_width_font_default( m ) ? Qtrue : Qfalse;
}

VALUE header_colors_available( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_colors_available( m ) ? Qtrue : Qfalse;
}

VALUE header_picture_display_available( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_picture_display_available( m ) ? Qtrue : Qfalse;
}

VALUE header_bold_available( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_bold_available( m ) ? Qtrue : Qfalse;
}

VALUE header_italics_available( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_italics_available( m ) ? Qtrue : Qfalse;
}

VALUE header_fixed_width_available( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_fixed_width_available( m ) ? Qtrue : Qfalse;
}

VALUE header_sound_available( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_sound_available( m ) ? Qtrue : Qfalse;
}

VALUE header_timed_input_available( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_timed_input_available( m ) ? Qtrue : Qfalse;
}


VALUE header_transcripting_on( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_transcripting_on( m ) ? Qtrue : Qfalse;
}

VALUE header_force_fixed_width( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_force_fixed_width( m ) ? Qtrue : Qfalse;
}

VALUE header_status_line_redraw( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_status_line_redraw( m ) ? Qtrue : Qfalse;
}

VALUE header_wants_pictures( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_wants_pictures( m ) ? Qtrue : Qfalse;
}

VALUE header_wants_undo( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_wants_undo( m ) ? Qtrue : Qfalse;
}

VALUE header_wants_mouse( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_wants_mouse( m ) ? Qtrue : Qfalse;
}

VALUE header_wants_color( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_wants_color( m ) ? Qtrue : Qfalse;
}

VALUE header_wants_sound( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_wants_sound( m ) ? Qtrue : Qfalse;
}

VALUE header_wants_menus( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return h_wants_menus( m ) ? Qtrue : Qfalse;
}


VALUE header_set_status_line_available( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_screen_splitting_available( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_variable_width_font_default( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_colors_available( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_picture_display_available( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_bold_available( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_italics_available( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_fixed_width_available( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_sound_available( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_timed_input_available( VALUE self, VALUE b ) {
  return self;
}


VALUE header_set_transcripting_on( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_force_fixed_width( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_status_line_redraw( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_wants_pictures( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_wants_undo( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_wants_mouse( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_wants_color( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_wants_sound( VALUE self, VALUE b ) {
  return self;
}

VALUE header_set_wants_menus( VALUE self, VALUE b ) {
  return self;
}


VALUE header_static_memory( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_static_memory( m ));
}

VALUE header_high_memory( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_high_memory( m ));
}


VALUE header_initial_program_counter( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_initial_program_counter( m ));
}

VALUE header_main_routine( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_main_routine( m ));
}


VALUE header_dictionary( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_dictionary( m ));
}

VALUE header_object_table( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_object_table( m ));
}

VALUE header_global_table( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_global_table( m ));
}

VALUE header_abbreviations_table( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_abbreviations_table( m ));
}

VALUE header_terminating_table( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_terminating_table( m ));
}

VALUE header_alphabet_table( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_alphabet_table( m ));
}


VALUE header_routines( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_routines( m ));
}

VALUE header_strings( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_strings( m ));
}


VALUE header_extension_table( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_extension_table( m ));
}


VALUE header_extension_table_size( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_extension_table_size( m ));
}


VALUE header_mouse_x( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_mouse_x( m ));
}

VALUE header_mouse_y( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_mouse_y( m ));
}


VALUE header_set_mouse_x( VALUE self, VALUE x ) {
  return self;
}

VALUE header_set_mouse_y( VALUE self, VALUE y ) {
  return self;
}


VALUE header_unicode_table( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_unicode_table( m ));
}


VALUE header_program_length( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_program_length( m ));
}

VALUE header_program_checksum( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_program_checksum( m ));
}


VALUE header_interpreter_number( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_interpreter_number( m ));
}

VALUE header_interpreter_version( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_interpreter_version( m ));
}


VALUE header_set_interpreter_number( VALUE self, VALUE n ) {
  return self;
}

VALUE header_set_interpreter_version( VALUE self, VALUE v ) {
  return self;
}


VALUE header_screen_rows( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_screen_rows( m ));
}

VALUE header_screen_cols( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_screen_cols( m ));
}


VALUE header_set_screen_rows( VALUE self, VALUE r ) {
  return self;
}

VALUE header_set_screen_cols( VALUE self, VALUE c ) {
  return self;
}


VALUE header_screen_width( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_screen_width( m ));
}

VALUE header_screen_height( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_screen_height( m ));
}


VALUE header_set_screen_width( VALUE self, VALUE w ) {
  return self;
}

VALUE header_set_screen_height( VALUE self, VALUE h ) {
  return self;
}


VALUE header_font_width( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_font_width( m ));
}

VALUE header_font_height( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_font_height( m ));
}


VALUE header_set_font_width( VALUE self, VALUE w ) {
  return self;
}

VALUE header_set_font_height( VALUE self, VALUE h ) {
  return self;
}


VALUE header_default_background_color( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_default_background_color( m ));
}

VALUE header_default_foreground_color( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_default_foreground_color( m ));
}


VALUE header_set_default_background_color( VALUE self, VALUE c ) {
  return self;
}

VALUE header_set_default_foreground_color( VALUE self, VALUE c ) {
  return self;
}


VALUE header_output_stream3_width( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_output_stream3_width( m ));
}


VALUE header_set_output_stream3_width( VALUE self, VALUE w ) {
  return self;
}


VALUE header_standard_major( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_standard_major( m ));
}

VALUE header_standard_minor( VALUE self ) {
  VALUE memory = rb_iv_get( self, "@memory" );
  zmemory *m;

  Data_Get_Struct( memory, zmemory, m );

  return UINT2NUM(h_standard_minor( m ));
}


VALUE header_set_standard_major( VALUE self, VALUE n ) {
  return self;
}

VALUE header_set_standard_minor( VALUE self, VALUE n ) {
  return self;
}


