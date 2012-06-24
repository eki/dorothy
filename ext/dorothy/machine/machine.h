
#include "ruby.h"

/*
 *  Ruby access into the interpreter.
 */

/* Classes */

extern VALUE Z;
extern VALUE Machine;
extern VALUE Memory;
extern VALUE Header;
extern VALUE ObjectTable;
extern VALUE Object;
extern VALUE Status;
extern VALUE Dictionary;
extern VALUE Entry;
extern VALUE InputStream;
extern VALUE Screen;
extern VALUE Program;
extern VALUE RuntimeError;
extern VALUE RandomNumberGenerator;


/* Prototypes */

VALUE memory_alloc( VALUE klass );
VALUE memory_initialize( VALUE self, VALUE filename );
VALUE memory_initialize_copy( VALUE self, VALUE original );
VALUE memory_length( VALUE self );
VALUE memory_dynamic_length( VALUE self );
VALUE memory_static_length( VALUE self );
VALUE memory_read_byte( VALUE self, VALUE addr );
VALUE memory_read_word( VALUE self, VALUE addr );
VALUE memory_read_string( VALUE self, VALUE addr );
VALUE memory_read_string_array( VALUE self, VALUE addr, VALUE length );
VALUE memory_dump_data( VALUE self );
VALUE memory_load_data( VALUE self, VALUE ary );
void  memory_mark( void * );
void  memory_free( void * );

VALUE machine_alloc( VALUE klass );
VALUE machine_initialize( VALUE self, VALUE filename );
VALUE machine_step( VALUE self );
VALUE machine_finished( VALUE self );
VALUE machine_pc( VALUE self );
VALUE machine_marshal_dump( VALUE self );
VALUE machine_marshal_load( VALUE self, VALUE ary );
void  machine_free( void * );

VALUE program_alloc( VALUE klass );
VALUE program_initialize( VALUE self, VALUE filename );
VALUE program_checksum( VALUE self );
VALUE program_version( VALUE self );
VALUE program_release( VALUE self );
VALUE program_serial( VALUE self );
void  program_free( void * );

VALUE object_table_find( VALUE self, VALUE n );
VALUE object_table_each( VALUE self );

VALUE object_name( VALUE self );
VALUE object_parent( VALUE self );
VALUE object_child( VALUE self );
VALUE object_sibling( VALUE self );
VALUE object_attributes( VALUE self );

VALUE status_type( VALUE self );
VALUE status_score( VALUE self );
VALUE status_turns( VALUE self );
VALUE status_hours( VALUE self );
VALUE status_minutes( VALUE self );
VALUE status_location( VALUE self );

VALUE header_version( VALUE self );

VALUE header_status_line_score( VALUE self );
VALUE header_status_line_time( VALUE self );
VALUE header_story_split( VALUE self );
VALUE header_status_line_available( VALUE self );
VALUE header_screen_splitting_available( VALUE self );
VALUE header_variable_width_font_default( VALUE self );
VALUE header_colors_available( VALUE self );
VALUE header_picture_display_available( VALUE self );
VALUE header_bold_available( VALUE self );
VALUE header_italics_available( VALUE self );
VALUE header_fixed_width_available( VALUE self );
VALUE header_sound_available( VALUE self );
VALUE header_timed_input_available( VALUE self );

VALUE header_transcripting_on( VALUE self );
VALUE header_force_fixed_width( VALUE self );
VALUE header_status_line_redraw( VALUE self );
VALUE header_wants_pictures( VALUE self );
VALUE header_wants_undo( VALUE self );
VALUE header_wants_mouse( VALUE self );
VALUE header_wants_color( VALUE self );
VALUE header_wants_sound( VALUE self );
VALUE header_wants_menus( VALUE self );

VALUE header_set_status_line_available( VALUE self, VALUE b );
VALUE header_set_screen_splitting_available( VALUE self, VALUE b );
VALUE header_set_variable_width_font_default( VALUE self, VALUE b );
VALUE header_set_colors_available( VALUE self, VALUE b );
VALUE header_set_picture_display_available( VALUE self, VALUE b );
VALUE header_set_bold_available( VALUE self, VALUE b );
VALUE header_set_italics_available( VALUE self, VALUE b );
VALUE header_set_fixed_width_available( VALUE self, VALUE b );
VALUE header_set_sound_available( VALUE self, VALUE b );
VALUE header_set_timed_input_available( VALUE self, VALUE b );

VALUE header_set_transcripting_on( VALUE self, VALUE b );
VALUE header_set_force_fixed_width( VALUE self, VALUE b );
VALUE header_set_status_line_redraw( VALUE self, VALUE b );
VALUE header_set_wants_pictures( VALUE self, VALUE b );
VALUE header_set_wants_undo( VALUE self, VALUE b );
VALUE header_set_wants_mouse( VALUE self, VALUE b );
VALUE header_set_wants_color( VALUE self, VALUE b );
VALUE header_set_wants_sound( VALUE self, VALUE b );
VALUE header_set_wants_menus( VALUE self, VALUE b );

VALUE header_static_memory( VALUE self );
VALUE header_high_memory( VALUE self );

VALUE header_initial_program_counter( VALUE self );
VALUE header_main_routine( VALUE self );

VALUE header_dictionary( VALUE self );
VALUE header_object_table( VALUE self );
VALUE header_global_table( VALUE self );
VALUE header_abbreviations_table( VALUE self );
VALUE header_terminating_table( VALUE self );
VALUE header_alphabet_table( VALUE self );

VALUE header_routines( VALUE self );
VALUE header_strings( VALUE self );

VALUE header_extension_table( VALUE self );

VALUE header_extension_table_size( VALUE self );

VALUE header_mouse_x( VALUE self );
VALUE header_mouse_y( VALUE self );

VALUE header_set_mouse_x( VALUE self, VALUE x );
VALUE header_set_mouse_y( VALUE self, VALUE y );

VALUE header_unicode_table( VALUE self );

VALUE header_program_length( VALUE self );
VALUE header_program_checksum( VALUE self );

VALUE header_interpreter_number( VALUE self );
VALUE header_interpreter_version( VALUE self );

VALUE header_set_interpreter_number( VALUE self, VALUE n );
VALUE header_set_interpreter_version( VALUE self, VALUE v );

VALUE header_screen_rows( VALUE self );
VALUE header_screen_cols( VALUE self );

VALUE header_set_screen_rows( VALUE self, VALUE r );
VALUE header_set_screen_cols( VALUE self, VALUE c );

VALUE header_screen_width( VALUE self );
VALUE header_screen_height( VALUE self );

VALUE header_set_screen_width( VALUE self, VALUE w );
VALUE header_set_screen_height( VALUE self, VALUE h );

VALUE header_font_width( VALUE self );
VALUE header_font_height( VALUE self );

VALUE header_set_font_width( VALUE self, VALUE w );
VALUE header_set_font_height( VALUE self, VALUE h );

VALUE header_default_background_color( VALUE self );
VALUE header_default_foreground_color( VALUE self );

VALUE header_set_default_background_color( VALUE self, VALUE c );
VALUE header_set_default_foreground_color( VALUE self, VALUE c );

VALUE header_output_stream3_width( VALUE self );

VALUE header_set_output_stream3_width( VALUE self, VALUE w );

VALUE header_standard_major( VALUE self );
VALUE header_standard_minor( VALUE self );

VALUE header_set_standard_major( VALUE self, VALUE n );
VALUE header_set_standard_minor( VALUE self, VALUE n );

VALUE dictionary_load( VALUE self );

extern ID id_new, id_dup, id_srand, id_rand, id_append_op,
  id_set_foreground, id_set_background, id_set_window, id_set_style, id_split,
  id_line_available, id_char_available, id_read_line, id_read_char,
  id_dictionary, id_parse,
  id_score, id_time;

extern VALUE sym_score, sym_time;


/*** Ruby 1.9 and 1.8 compatibility ***/

#ifndef RSTRING_PTR
#define RSTRING_PTR(x) (RSTRING(x)->ptr)
#endif

#ifndef RSTRING_LEN
#define RSTRING_LEN(x) (RSTRING(x)->len)
#endif

#ifndef RARRAY_PTR
#define RARRAY_PTR(x) (RARRAY(x)->ptr)
#endif

#ifndef RARRAY_LEN
#define RARRAY_LEN(x) (RARRAY(x)->len)
#endif


/*** Constants ***/

#define true 1
#define false 0
#define STACK_SIZE 1024
#define MAX_NESTING 16

/*** Various Z-machine constants ***/

#define V1 1
#define V2 2
#define V3 3
#define V4 4
#define V5 5
#define V6 6
#define V7 7
#define V8 8

#define CONFIG_BYTE_SWAPPED 0x01 /* Story file is byte swapped         - V3  */
#define CONFIG_TIME         0x02 /* Status line displays time          - V3  */
#define CONFIG_TWODISKS     0x04 /* Story file occupied two disks      - V3  */
#define CONFIG_TANDY        0x08 /* Tandy licensed game                - V3  */
#define CONFIG_NOSTATUSLINE 0x10 /* Interpr can't support status lines - V3  */
#define CONFIG_SPLITSCREEN  0x20 /* Interpr supports split screen mode - V3  */
#define CONFIG_PROPORTIONAL 0x40 /* Interpr uses proportional font     - V3  */

#define CONFIG_COLOUR       0x01 /* Interpr supports colour            - V5+ */
#define CONFIG_PICTURES     0x02 /* Interpr supports pictures          - V6  */
#define CONFIG_BOLDFACE     0x04 /* Interpr supports boldface style    - V4+ */
#define CONFIG_EMPHASIS     0x08 /* Interpr supports emphasis style    - V4+ */
#define CONFIG_FIXED        0x10 /* Interpr supports fixed width style - V4+ */
#define CONFIG_SOUND        0x20 /* Interpr supports sound             - V6  */

#define CONFIG_TIMEDINPUT   0x80 /* Interpr supports timed input       - V4+ */
#define CONFIG_TIMEDINPUT   0x80 /* Interpr supports timed input       - V4+ */

#define SCRIPTING_FLAG    0x0001 /* Outputting to transscription file  - V1+ */
#define FIXED_FONT_FLAG   0x0002 /* Use fixed width font               - V3+ */
#define REFRESH_FLAG      0x0004 /* Refresh the screen                 - V6  */
#define GRAPHICS_FLAG     0x0008 /* Game wants to use graphics         - V5+ */
#define OLD_SOUND_FLAG    0x0010 /* Game wants to use sound effects    - V3  */
#define UNDO_FLAG         0x0010 /* Game wants to use UNDO feature     - V5+ */
#define MOUSE_FLAG        0x0020 /* Game wants to use a mouse          - V5+ */
#define COLOUR_FLAG       0x0040 /* Game wants to use colours          - V5+ */
#define SOUND_FLAG        0x0080 /* Game wants to use sound effects    - V5+ */
#define MENU_FLAG         0x0100 /* Game wants to use menus            - V6  */

#define INTERP_DEFAULT 0
#define INTERP_DEC_20 1
#define INTERP_APPLE_IIE 2
#define INTERP_MACINTOSH 3
#define INTERP_AMIGA 4
#define INTERP_ATARI_ST 5
#define INTERP_MSDOS 6
#define INTERP_CBM_128 7
#define INTERP_CBM_64 8
#define INTERP_APPLE_IIC 9
#define INTERP_APPLE_IIGS 10
#define INTERP_TANDY 11


#define BLACK_COLOUR 2
#define RED_COLOUR 3
#define GREEN_COLOUR 4
#define YELLOW_COLOUR 5
#define BLUE_COLOUR 6
#define MAGENTA_COLOUR 7
#define CYAN_COLOUR 8
#define WHITE_COLOUR 9
#define GREY_COLOUR 10          /* INTERP_MSDOS only */
#define LIGHTGREY_COLOUR 10     /* INTERP_AMIGA only */
#define MEDIUMGREY_COLOUR 11    /* INTERP_AMIGA only */
#define DARKGREY_COLOUR 12      /* INTERP_AMIGA only */

#define REVERSE_STYLE 1
#define BOLDFACE_STYLE 2
#define EMPHASIS_STYLE 4
#define FIXED_WIDTH_STYLE 8

#define TEXT_FONT 1
#define PICTURE_FONT 2
#define GRAPHICS_FONT 3
#define FIXED_WIDTH_FONT 4

#define BEEP_HIGH       1
#define BEEP_LOW        2

/*** Constants for os_restart_game */

#define RESTART_BEGIN 0
#define RESTART_WPROP_SET 1
#define RESTART_END 2

/*** Character codes ***/

#define ZC_TIME_OUT 0x00
#define ZC_NEW_STYLE 0x01
#define ZC_NEW_FONT 0x02
#define ZC_BACKSPACE 0x08
#define ZC_INDENT 0x09
#define ZC_GAP 0x0b
#define ZC_RETURN 0x0d
#define ZC_HKEY_MIN 0x0e
#define ZC_HKEY_RECORD 0x0e
#define ZC_HKEY_PLAYBACK 0x0f
#define ZC_HKEY_SEED 0x10
#define ZC_HKEY_UNDO 0x11
#define ZC_HKEY_RESTART 0x12
#define ZC_HKEY_QUIT 0x13
#define ZC_HKEY_DEBUG 0x14
#define ZC_HKEY_HELP 0x15
#define ZC_HKEY_MAX 0x15
#define ZC_ESCAPE 0x1b
#define ZC_ASCII_MIN 0x20
#define ZC_ASCII_MAX 0x7e
#define ZC_BAD 0x7f
#define ZC_ARROW_MIN 0x81
#define ZC_ARROW_UP 0x81
#define ZC_ARROW_DOWN 0x82
#define ZC_ARROW_LEFT 0x83
#define ZC_ARROW_RIGHT 0x84
#define ZC_ARROW_MAX 0x84
#define ZC_FKEY_MIN 0x85
#define ZC_FKEY_MAX 0x90
#define ZC_NUMPAD_MIN 0x91
#define ZC_NUMPAD_MAX 0x9a
#define ZC_SINGLE_CLICK 0x9b
#define ZC_DOUBLE_CLICK 0x9c
#define ZC_MENU_CLICK 0x9d
#define ZC_LATIN1_MIN 0xa0
#define ZC_LATIN1_MAX 0xff

/*** File types ***/

#define FILE_RESTORE 0
#define FILE_SAVE 1
#define FILE_SCRIPT 2
#define FILE_PLAYBACK 3
#define FILE_RECORD 4
#define FILE_LOAD_AUX 5
#define FILE_SAVE_AUX 6

/*** Macros ***/

/*** Access data from the program the machine is running. */

#define mem(zm, addr) \
  (((addr) < zm->m->dynamic_length) ? zm->m->m_dynamic : zm->m->m_static)

#define maddr(zm, addr) \
  (((addr) < zm->m->dynamic_length) ? (addr) : (addr) - zm->m->dynamic_length)

#define read_byte(zm,addr) \
  (mem(zm,addr)[maddr(zm,addr)])

#define read_word(zm,addr) \
  (((zword) read_byte(zm,(addr)) << 8) | read_byte(zm, (addr)+1))

#define read_n(zm,addr,n) \
  ((n) == 1 ? read_byte( zm, (addr) ) : read_word( zm, (addr) ))

#define write_byte(zm,addr,value) (read_byte(zm,addr) = (value))

#define write_word(zm,addr,value) \
  { read_byte(zm,addr)   = (value) >> 8; \
    read_byte(zm,addr+1) = (value) & 0xff; }

#define PC(zm) (zm->pcp - zm->m->m_static + zm->m->dynamic_length)

#define m_screen(zm) (rb_iv_get( zm->self, "@screen" ))
#define print_cstr(zm,str) \
  (rb_funcall( m_screen( zm ), id_append_op, 1, rb_str_new2( (char *)str ) ))
#define print_rstr(zm,str) (rb_funcall( m_screen( zm ), id_append_op, 1, str ))

#define m_trace(zm) (rb_iv_get( zm->self, "@trace" ))
#define trace(zm,fmt,...)  \
  { char * buf = ALLOC_N( char, 255 );  \
    sprintf( buf, fmt, ##__VA_ARGS__ ); \
    rb_ary_push( m_trace(zm), rb_str_new2( (char *)buf ) ); }

/*** Access data from the header of the program running. */
/***    These are higher-level functions, intended to extract info from
   *    the header, not individual zbytes and zwords.
   */

#define h_version(zm) (read_byte( zm, 0x00 ))
#define h_release(zm) (read_word( zm, 0x02 ))
#define h_status_line_score(zm) ((read_byte( zm, 0x01 ) & 0x02) == 0)
#define h_status_line_time(zm)  ((read_byte( zm, 0x01 ) & 0x02) == 1)
#define h_story_split(zm)       (read_byte( zm, 0x01 ) & 0x04)
#define h_status_line_available(zm) (! (read_byte( zm, 0x01 ) & 0x10))
#define h_screen_splitting_available(zm) (read_byte( zm, 0x01 ) & 0x20)
#define h_variable_width_font_default(zm) (read_byte( zm, 0x01 ) & 0x40)
#define h_colors_available(zm) (read_byte( zm, 0x02 ) & 0x01)
#define h_picture_display_available(zm) (read_byte( zm, 0x02 ) & 0x02)
#define h_bold_available(zm) (read_byte( zm, 0x02 ) & 0x04)
#define h_italics_available(zm) (read_byte( zm, 0x02 ) & 0x08)
#define h_fixed_width_available(zm) (read_byte( zm, 0x02 ) & 0x10)
#define h_sound_available(zm) (read_byte( zm, 0x02 ) & 0x20)
#define h_timed_input_available(zm) (read_byte( zm, 0x02 ) & 0x80)
#define h_transcripting_on(zm) (read_word( zm, 0x10 ) & 0x0001)
#define h_force_fixed_width(zm) (read_word( zm, 0x10 ) & 0x0002)
#define h_status_line_redraw(zm) (read_word( zm, 0x10 ) & 0x0004)
#define h_wants_pictures(zm) (read_word( zm, 0x10 ) & 0x0008)
#define h_wants_undo(zm) (read_word( zm, 0x10 ) & 0x0010)
#define h_wants_mouse(zm) (read_word( zm, 0x10 ) & 0x0020)
#define h_wants_color(zm) (read_word( zm, 0x10 ) & 0x0040)
#define h_wants_sound(zm) (h_version(zm) == V3 ? \
                           (read_word( zm, 0x10 ) & 0x0010) : \
                           (read_word( zm, 0x10 ) & 0x0080))
#define h_wants_menus(zm) (read_word( zm, 0x10 ) & 0x0100)
#define h_static_memory(zm) (read_word( zm, 0x0e ))
#define h_high_memory(zm) (read_word( zm, 0x04 ))
#define h_initial_program_counter(zm) (read_word( zm, 0x06 ))
#define h_main_routine(zm) (read_word( zm, 0x06 ))
#define h_dictionary(zm) (read_word( zm, 0x08 ))
#define h_object_table(zm) (read_word( zm, 0x0a ))
#define h_global_table(zm) (read_word( zm, 0x0c ))
#define h_abbreviations_table(zm) (read_word( zm, 0x18 ))
#define h_terminating_table(zm) (read_word( zm, 0x2e ))
#define h_alphabet_table(zm) (read_word( zm, 0x34 ))
#define h_routines(zm) (read_word( zm, 0x28 ))
#define h_strings(zm) (read_word( zm, 0x2a ))
#define h_extension_table(zm) (read_word( zm, 0x36 ))
#define h_extension_table_size(zm) (read_word( zm, h_extension_table( zm ) ))
#define h_mouse_x(zm) (read_word( zm, h_extension_table( zm ) + 2 ))
#define h_mouse_y(zm) (read_word( zm, h_extension_table( zm ) + 4 ))
#define h_unicode_table(zm) (read_word( zm, h_extension_table( zm ) + 6 ))
#define h_program_checksum(zm) (read_word( zm, 0x1c ))
#define h_interpreter_number(zm) (read_byte( zm, 0x1e ))
#define h_interpreter_version(zm) (read_byte( zm, 0x1f ))
#define h_screen_rows(zm) (read_byte( zm, 0x20 ))
#define h_screen_cols(zm) (read_byte( zm, 0x21 ))
#define h_screen_width(zm) (read_word( zm, 0x22 ))
#define h_screen_height(zm) (read_word( zm, 0x24 ))
#define h_font_width(zm) (h_version(zm) == V5 ? read_byte( zm, 0x26 ) : \
                                                read_byte( zm, 0x27 ))
#define h_font_height(zm) (h_version(zm) == V5 ? read_byte( zm, 0x27 ) : \
                                                 read_byte( zm, 0x26 ))
#define h_default_background_color(zm) (read_byte( zm, 0x2c ))
#define h_default_foreground_color(zm) (read_byte( zm, 0x2d ))
#define h_output_stream3_width(zm) (read_word( zm, 0x30 ))
#define h_standard_major(zm) (read_byte( zm, 0x32 ))
#define h_standard_minor(zm) (read_byte( zm, 0x33 ))

#define dict_num_word_separators(zm,dict) (read_byte( zm, dict ))

#define dict_get_word_separator(zm,dict,i) \
  ((i) < dict_num_word_separators( zm, (dict) ) ? \
   read_byte( zm, (dict) + 1 + (i) ) : \
   0)

#define dict_entry_length(zm,dict) \
  (read_byte( zm, (dict) + 1 + dict_num_word_separators( zm, (dict) ) ))

#define dict_num_entries(zm,dict) \
  (read_word( zm, (dict) + 1 + dict_num_word_separators( zm, (dict) ) + 1 ))

#define keyboard(zm) (rb_iv_get( zm->self, "@keyboard" ))

#define line_available(zm) \
  (RTEST(rb_funcall( keyboard(zm), id_line_available, 0)))

#define char_available(zm) \
  (RTEST(rb_funcall( keyboard(zm), id_char_available, 0)))

#define text_buffer_offset(zm) (h_version(zm) < 5 ? 1 : 2)

#define runtime_error(s) \
  (rb_raise( RuntimeError, "Error running program: %s", (s) ))

#define obj_max_objects(zm) (h_version(zm) < 4 ? 255 : 65535)
#define obj_parent_offset(zm) (h_version(zm) < 4 ? 4 : 6)
#define obj_sibling_offset(zm) (h_version(zm) < 4 ? 5 : 8)
#define obj_child_offset(zm) (h_version(zm) < 4 ? 6 : 10)
#define obj_parent_size(zm) (h_version(zm) < 4 ? 1 : 2)
#define obj_sibling_size(zm) (h_version(zm) < 4 ? 1 : 2)
#define obj_child_size(zm) (h_version(zm) < 4 ? 1 : 2)
#define obj_prop_offset(zm) (h_version(zm) < 4 ? 7 : 12)
#define obj_prop_mask(zm) (h_version(zm) < 4 ? 0x1f : 0x3f)
#define obj_size(zm) (h_version(zm) < 4 ? 9 : 14)
#define obj_defaults_offset(zm) (h_version(zm) < 4 ? 62 : 126)

#define obj_addr(zm,n) \
  (h_object_table(zm) + ((n)-1) * obj_size(zm) + obj_defaults_offset(zm))

#define obj_name_addr(zm,n) \
  (read_word( zm, obj_addr(zm,(n)) + obj_prop_offset(zm) ))

#define obj_name_size(zm,n) \
  (read_byte( zm, obj_name_addr(zm,(n)) ))

#define obj_first_prop_addr(zm,n) \
  (obj_name_addr(zm,(n)) + 1 + 2 * obj_name_size(zm,(n)))

#define obj_default_prop(zm,n) \
  (read_word( zm, h_object_table(zm) + ((n) - 1) * 2 ))

#define obj_parent(zm,n) \
  (read_n( zm, obj_addr(zm,(n)) + obj_parent_offset(zm), obj_parent_size(zm) ))
#define obj_sibling(zm,n) \
  (read_n( zm, obj_addr(zm,(n)) + obj_sibling_offset(zm), obj_sibling_size(zm)))
#define obj_child(zm,n) \
  (read_n( zm, obj_addr(zm,(n)) + obj_child_offset(zm), obj_child_size(zm) ))

#define obj_max_attr(zm) (h_version(zm) < 4 ? 31 : 47)

#define obj_attr(zm,n,a) \
  (read_byte( zm, obj_addr(zm,(n)) + (a)/8 ) & (0x80 >> ((a) & 7)))

#define obj_set_attr(zm,n,a) \
  (write_byte( zm, obj_addr(zm,(n)) + (a)/8, \
    read_byte( zm, obj_addr(zm,(n)) + (a)/8 ) | (0x80 >> ((a) & 7)) ))

#define obj_clear_attr(zm,n,a) \
  (write_byte( zm, obj_addr(zm,(n)) + (a)/8, \
    read_byte( zm, obj_addr(zm,(n)) + (a)/8 ) & ~(0x80 >> ((a) & 7)) ))

/*
 *  The data and functions for the interpreter (C).
 */

typedef int bool;
typedef unsigned char zbyte;
typedef unsigned short zword;
typedef unsigned char zchar;

typedef unsigned short zaddr;

struct smemory;
typedef struct smemory zmemory;

struct smemory {
  long length;
  long dynamic_length;
  long static_length;

  zbyte *m_dynamic;
  zbyte *m_static;

  zmemory *m;        /* Pointer back to self, allows using zmemory, zmachine */
                     /* and zprogram interchangably. */

  zmemory *parent;   /* zmemory structs can share static memory, so to       */
  long children;     /* assist ruby's gc, we need to track the parent and    */
                     /* and count refs. */
  VALUE self;
};

struct sprogram;
typedef struct sprogram zprogram;

struct sprogram {
  zbyte version;
  zword release;
  char serial[6];

  zmemory *m;
  
  zword checksum;

  VALUE self;
};

struct smachine;
typedef struct smachine zmachine;

typedef void (*z_op)( zmachine * );

struct smachine {
  zbyte version;

  zprogram *zp;
  zmemory *m;

  zbyte *pcp;

  zword stack[STACK_SIZE];
  zword *sp;
  zword *fp;
  zword frame_count;

  zword zargs[8];
  int zargc;

  int finished;

  VALUE self;
};


/* Function prototypes */

long h_program_length( zmemory *m );

void h_set_status_line_available( zmemory *m, bool b );
void h_set_screen_splitting_available( zmemory *m, bool b );
void h_set_variable_width_font_default( zmemory *m, bool b );
void h_set_colors_available( zmemory *m, bool b );
void h_set_picture_display_available( zmemory *m, bool b );
void h_set_bold_available( zmemory *m, bool b );
void h_set_italics_available( zmemory *m, bool b );
void h_set_fixed_width_available( zmemory *m, bool b );
void h_set_sound_available( zmemory *m, bool b );
void h_set_timed_input_available( zmemory *m, bool b );

void h_set_transcripting_on( zmemory *m, bool b );
void h_set_force_fixed_width( zmemory *m, bool b );
void h_set_status_line_redraw( zmemory *m, bool b );
void h_set_wants_pictures( zmemory *m, bool b );
void h_set_wants_undo( zmemory *m, bool b );
void h_set_wants_mouse( zmemory *m, bool b );
void h_set_wants_color( zmemory *m, bool b );
void h_set_wants_sound( zmemory *m, bool b );
void h_set_wants_menus( zmemory *m, bool b );

void h_set_mouse_x( zmemory *m, int x );
void h_set_mouse_y( zmemory *m, int y );

void h_set_interpreter_number( zmemory *m, int n );
void h_set_interpreter_version( zmemory *m, int v );

void h_set_screen_rows( zmemory *m, int r );
void h_set_screen_cols( zmemory *m, int c );

void h_set_screen_width( zmemory *m, int w );
void h_set_screen_height( zmemory *m, int h );

void h_set_font_width( zmemory *m, int w );
void h_set_font_height( zmemory *m, int h );

void h_set_default_background_color( zmemory *m, int c );
void h_set_default_foreground_color( zmemory *m, int c );

void h_set_output_stream3_width( zmemory *m, int w );

void h_set_standard_major( zmemory *m, int n );
void h_set_standard_minor( zmemory *m, int n );

/** Object related functions. **/

zaddr obj_next_prop_addr( zmachine *zm, zaddr prop );

void obj_set_parent( zmachine *zm, zword n, zword pn );
void obj_set_sibling( zmachine *zm, zword n, zword sn );
void obj_set_child( zmachine *zm, zword n, zword cn );

/** Processor related functions.  **/

void load_operand( zmachine *zm, zbyte type );
void load_all_operands( zmachine *zm, zbyte specifier );

bool p_step( zmachine *zm );

zword p_load_global( zmachine *zm, zbyte variable );
zword p_load( zmachine *zm );

void p_store_global( zmachine *zm, zbyte variable, zword value );
void p_store( zmachine *zm, zword value );

void p_call( zmachine *zm, zword routine, int argc, zword *args, int ct );
void p_ret( zmachine *zm, zword value );

void p_branch( zmachine *zm, bool flag );

void p_skip_embedded_string( zmachine *zm );

void storew( zmachine *zm, zaddr addr, zword value );
void storeb( zmachine *zm, zaddr addr, zbyte value );

/** Text processing **/

zchar translate_from_zscii( zmemory *zm, zbyte c );
zbyte translate_to_zscii( zmemory *zm, zchar c );
zchar alphabet( zmemory *zm, int set, int index );

/* Ops */

void z_add( zmachine *zm );
void z_and( zmachine * );
void z_art_shift( zmachine * );
void z_buffer_mode( zmachine * );
void z_call_n( zmachine * );
void z_call_s( zmachine * );
void z_catch( zmachine * );
void z_check_arg_count( zmachine * );
void z_check_unicode( zmachine * );
void z_clear_attr( zmachine * );
void z_copy_table( zmachine * );
void z_dec( zmachine * );
void z_dec_chk( zmachine * );
void z_div( zmachine * );
void z_draw_picture( zmachine * );
void z_encode_text( zmachine * );
void z_erase_line( zmachine * );
void z_erase_picture( zmachine * );
void z_erase_window( zmachine * );
void z_extended( zmachine * );
void z_get_child( zmachine * );
void z_get_cursor( zmachine * );
void z_get_next_prop( zmachine * );
void z_get_parent( zmachine * );
void z_get_prop( zmachine * );
void z_get_prop_addr( zmachine * );
void z_get_prop_len( zmachine * );
void z_get_sibling( zmachine * );
void z_get_wind_prop( zmachine * );
void z_illegal( zmachine * );
void z_inc( zmachine * );
void z_inc_chk( zmachine * );
void z_input_stream( zmachine * );
void z_insert_obj( zmachine * );
void z_je( zmachine * );
void z_jg( zmachine * );
void z_jin( zmachine * );
void z_jl( zmachine * );
void z_jump( zmachine * );
void z_jz( zmachine * );
void z_load( zmachine * );
void z_loadb( zmachine * );
void z_loadw( zmachine * );
void z_log_shift( zmachine * );
void z_make_menu( zmachine * );
void z_mod( zmachine * );
void z_mouse_window( zmachine * );
void z_move_window( zmachine * );
void z_mul( zmachine * );
void z_new_line( zmachine * );
void z_nop( zmachine * );
void z_not( zmachine * );
void z_or( zmachine * );
void z_output_stream( zmachine * );
void z_picture_data( zmachine * );
void z_picture_table( zmachine * );
void z_piracy( zmachine * );
void z_pop( zmachine * );
void z_pop_stack( zmachine * );
void z_print( zmachine * );
void z_print_addr( zmachine * );
void z_print_char( zmachine * );
void z_print_form( zmachine * );
void z_print_num( zmachine * );
void z_print_obj( zmachine * );
void z_print_paddr( zmachine * );
void z_print_ret( zmachine * );
void z_print_table( zmachine * );
void z_print_unicode( zmachine * );
void z_pull( zmachine * );
void z_push( zmachine * );
void z_push_stack( zmachine * );
void z_put_prop( zmachine * );
void z_put_wind_prop( zmachine * );
void z_quit( zmachine * );
void z_random( zmachine * );
void z_read( zmachine * );
void z_read_char( zmachine * );
void z_read_mouse( zmachine * );
void z_remove_obj( zmachine * );
void z_restart( zmachine * );
void z_restore( zmachine * );
void z_restore_undo( zmachine * );
void z_ret( zmachine * );
void z_ret_popped( zmachine * );
void z_rfalse( zmachine * );
void z_rtrue( zmachine * );
void z_save( zmachine * );
void z_save_undo( zmachine * );
void z_scan_table( zmachine * );
void z_scroll_window( zmachine * );
void z_set_attr( zmachine * );
void z_set_font( zmachine * );
void z_set_colour( zmachine * );
void z_set_cursor( zmachine * );
void z_set_margins( zmachine * );
void z_set_window( zmachine * );
void z_set_text_style( zmachine * );
void z_show_status( zmachine * );
void z_sound_effect( zmachine * );
void z_split_window( zmachine * );
void z_store( zmachine * );
void z_storeb( zmachine * );
void z_storew( zmachine * );
void z_sub( zmachine * );
void z_test( zmachine * );
void z_test_attr( zmachine * );
void z_throw( zmachine * );
void z_tokenise( zmachine * );
void z_verify( zmachine * );
void z_window_size( zmachine * );
void z_window_style( zmachine * );

extern z_op op0_opcodes[0x10];
extern z_op op1_opcodes[0x10];
extern z_op var_opcodes[0x40];
extern z_op ext_opcodes[0x1d];

extern char* op0_opcode_names[0x10];
extern char* op1_opcode_names[0x10];
extern char* var_opcode_names[0x40];
extern char* ext_opcode_names[0x1d];

