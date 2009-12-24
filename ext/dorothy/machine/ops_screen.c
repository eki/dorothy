
#include "machine.h"

void z_erase_line( zmachine *zm ) {
  runtime_error( "z_erase_line is not yet implemented!" );
}

void z_erase_window( zmachine *zm ) {
  runtime_error( "z_erase_window is not yet implemented!" );
}

void z_get_cursor( zmachine *zm ) {
  runtime_error( "z_get_cursor is not yet implemented!" );
}

void z_set_font( zmachine *zm ) {
  /* Our screen model doesn't support multiple fonts.  NOP */
}

void z_set_colour( zmachine *zm ) {
  rb_funcall( m_screen(zm), id_set_foreground, 1, INT2NUM(zm->zargs[0]) );
  rb_funcall( m_screen(zm), id_set_background, 1, INT2NUM(zm->zargs[1]) );
}

void z_set_cursor( zmachine *zm ) {
}

void z_set_window( zmachine *zm ) {
  rb_funcall( m_screen(zm), id_set_window, 1, INT2NUM(zm->zargs[0]) );
}

void z_set_text_style( zmachine *zm ) {
  rb_funcall( m_screen(zm), id_set_style, 1, INT2NUM(zm->zargs[0]) );
}

void z_split_window( zmachine *zm ) {
  rb_funcall( m_screen(zm), id_split, 1, INT2NUM(zm->zargs[0]) );
}

