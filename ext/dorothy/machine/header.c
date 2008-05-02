
#include "machine.h"

long h_program_length( zmachine *zm ) {
  long length = (long) read_word( zm, 0x1a ) * 2;
  zbyte version = h_version( zm );

  if( version >= V4 ) {
    length *= 2;
  }

  if( version >= V6 ) {
    length *= 2;
  }
  
  return length;
}

void h_set_status_line_available( zmachine *zm, bool b ) {

}

void h_set_screen_splitting_available( zmachine *zm, bool b ) {

}

void h_set_variable_width_font_default( zmachine *zm, bool b ) {

}

void h_set_colors_available( zmachine *zm, bool b ) {

}

void h_set_picture_display_available( zmachine *zm, bool b ) {

}

void h_set_bold_available( zmachine *zm, bool b ) {

}

void h_set_italics_available( zmachine *zm, bool b ) {

}

void h_set_fixed_width_available( zmachine *zm, bool b ) {

}

void h_set_sound_available( zmachine *zm, bool b ) {

}

void h_set_timed_input_available( zmachine *zm, bool b ) {

}


void h_set_transcripting_on( zmachine *zm, bool b ) {

}

void h_set_force_fixed_width( zmachine *zm, bool b ) {

}

void h_set_status_line_redraw( zmachine *zm, bool b ) {

}

void h_set_wants_pictures( zmachine *zm, bool b ) {

}

void h_set_wants_undo( zmachine *zm, bool b ) {

}

void h_set_wants_mouse( zmachine *zm, bool b ) {

}

void h_set_wants_color( zmachine *zm, bool b ) {

}

void h_set_wants_sound( zmachine *zm, bool b ) {

}

void h_set_wants_menus( zmachine *zm, bool b ) {

}


void h_set_mouse_x( zmachine *zm, int x ) {

}

void h_set_mouse_y( zmachine *zm, int y ) {

}


void h_set_interpreter_number( zmachine *zm, int n ) {

}

void h_set_interpreter_version( zmachine *zm, int v ) {

}


void h_set_screen_rows( zmachine *zm, int r ) {

}

void h_set_screen_cols( zmachine *zm, int c ) {

}


void h_set_screen_width( zmachine *zm, int w ) {

}

void h_set_screen_height( zmachine *zm, int h ) {

}


void h_set_font_width( zmachine *zm, int w ) {

}

void h_set_font_height( zmachine *zm, int h ) {

}


void h_set_default_background_color( zmachine *zm, int c ) {

}

void h_set_default_foreground_color( zmachine *zm, int c ) {

}


void h_set_output_stream3_width( zmachine *zm, int w ) {

}


void h_set_standard_major( zmachine *zm, int n ) {

}

void h_set_standard_minor( zmachine *zm, int n ) {

}



VALUE header_version( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_version( zm ));
}


VALUE header_status_line_score( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_status_line_score( zm ) ? Qtrue : Qfalse;
}

VALUE header_status_line_time( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_status_line_time( zm ) ? Qtrue : Qfalse;
}

VALUE header_story_split( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_story_split( zm ) ? Qtrue : Qfalse;
}

VALUE header_status_line_available( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_status_line_available( zm ) ? Qtrue : Qfalse;
}

VALUE header_screen_splitting_available( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_screen_splitting_available( zm ) ? Qtrue : Qfalse;
}

VALUE header_variable_width_font_default( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_variable_width_font_default( zm ) ? Qtrue : Qfalse;
}

VALUE header_colors_available( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_colors_available( zm ) ? Qtrue : Qfalse;
}

VALUE header_picture_display_available( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_picture_display_available( zm ) ? Qtrue : Qfalse;
}

VALUE header_bold_available( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_bold_available( zm ) ? Qtrue : Qfalse;
}

VALUE header_italics_available( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_italics_available( zm ) ? Qtrue : Qfalse;
}

VALUE header_fixed_width_available( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_fixed_width_available( zm ) ? Qtrue : Qfalse;
}

VALUE header_sound_available( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_sound_available( zm ) ? Qtrue : Qfalse;
}

VALUE header_timed_input_available( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_timed_input_available( zm ) ? Qtrue : Qfalse;
}


VALUE header_transcripting_on( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_transcripting_on( zm ) ? Qtrue : Qfalse;
}

VALUE header_force_fixed_width( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_force_fixed_width( zm ) ? Qtrue : Qfalse;
}

VALUE header_status_line_redraw( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_status_line_redraw( zm ) ? Qtrue : Qfalse;
}

VALUE header_wants_pictures( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_wants_pictures( zm ) ? Qtrue : Qfalse;
}

VALUE header_wants_undo( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_wants_undo( zm ) ? Qtrue : Qfalse;
}

VALUE header_wants_mouse( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_wants_mouse( zm ) ? Qtrue : Qfalse;
}

VALUE header_wants_color( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_wants_color( zm ) ? Qtrue : Qfalse;
}

VALUE header_wants_sound( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_wants_sound( zm ) ? Qtrue : Qfalse;
}

VALUE header_wants_menus( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return h_wants_menus( zm ) ? Qtrue : Qfalse;
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
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_static_memory( zm ));
}

VALUE header_high_memory( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_high_memory( zm ));
}


VALUE header_initial_program_counter( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_initial_program_counter( zm ));
}

VALUE header_main_routine( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_main_routine( zm ));
}


VALUE header_dictionary( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_dictionary( zm ));
}

VALUE header_object_table( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_object_table( zm ));
}

VALUE header_global_table( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_global_table( zm ));
}

VALUE header_abbreviations_table( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_abbreviations_table( zm ));
}

VALUE header_terminating_table( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_terminating_table( zm ));
}

VALUE header_alphabet_table( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_alphabet_table( zm ));
}


VALUE header_routines( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_routines( zm ));
}

VALUE header_strings( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_strings( zm ));
}


VALUE header_extension_table( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_extension_table( zm ));
}


VALUE header_extension_table_size( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_extension_table_size( zm ));
}


VALUE header_mouse_x( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_mouse_x( zm ));
}

VALUE header_mouse_y( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_mouse_y( zm ));
}


VALUE header_set_mouse_x( VALUE self, VALUE x ) {
  return self;
}

VALUE header_set_mouse_y( VALUE self, VALUE y ) {
  return self;
}


VALUE header_unicode_table( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_unicode_table( zm ));
}


VALUE header_program_length( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_program_length( zm ));
}

VALUE header_program_checksum( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_program_checksum( zm ));
}


VALUE header_interpreter_number( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_interpreter_number( zm ));
}

VALUE header_interpreter_version( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_interpreter_version( zm ));
}


VALUE header_set_interpreter_number( VALUE self, VALUE n ) {
  return self;
}

VALUE header_set_interpreter_version( VALUE self, VALUE v ) {
  return self;
}


VALUE header_screen_rows( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_screen_rows( zm ));
}

VALUE header_screen_cols( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_screen_cols( zm ));
}


VALUE header_set_screen_rows( VALUE self, VALUE r ) {
  return self;
}

VALUE header_set_screen_cols( VALUE self, VALUE c ) {
  return self;
}


VALUE header_screen_width( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_screen_width( zm ));
}

VALUE header_screen_height( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_screen_height( zm ));
}


VALUE header_set_screen_width( VALUE self, VALUE w ) {
  return self;
}

VALUE header_set_screen_height( VALUE self, VALUE h ) {
  return self;
}


VALUE header_font_width( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_font_width( zm ));
}

VALUE header_font_height( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_font_height( zm ));
}


VALUE header_set_font_width( VALUE self, VALUE w ) {
  return self;
}

VALUE header_set_font_height( VALUE self, VALUE h ) {
  return self;
}


VALUE header_default_background_color( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_default_background_color( zm ));
}

VALUE header_default_foreground_color( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_default_foreground_color( zm ));
}


VALUE header_set_default_background_color( VALUE self, VALUE c ) {
  return self;
}

VALUE header_set_default_foreground_color( VALUE self, VALUE c ) {
  return self;
}


VALUE header_output_stream3_width( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_output_stream3_width( zm ));
}


VALUE header_set_output_stream3_width( VALUE self, VALUE w ) {
  return self;
}


VALUE header_standard_major( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_standard_major( zm ));
}

VALUE header_standard_minor( VALUE self ) {
  VALUE machine = rb_iv_get( self, "@machine" );
  zmachine *zm;

  Data_Get_Struct( machine, zmachine, zm );

  return UINT2NUM(h_standard_minor( zm ));
}


VALUE header_set_standard_major( VALUE self, VALUE n ) {
  return self;
}

VALUE header_set_standard_minor( VALUE self, VALUE n ) {
  return self;
}


