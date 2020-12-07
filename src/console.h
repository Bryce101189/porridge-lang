#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "token.h"
#include "scanner.h"


typedef enum
{
    STL_NONE = 0,
    STL_BOLD = 1,
    STL_DIM = 2,
    STL_ITALIC = 3,
    STL_UNDERLINE = 4,
    STL_STRIKETHROUGH = 9,
} style_n;

typedef enum
{
    COL_NONE = 0,
    COL_BLACK = 30,
    COL_RED = 31,
    COL_GREEN = 32,
    COL_YELLOW = 33,
    COL_BLUE = 34,
    COL_MAGENTA = 35,
    COL_CYAN = 36,
    COL_WHITE = 37,
} color_n;

char *get_line(char *text, int line);

void set_text_style(style_n style);
void set_text_color(color_n color);
void reset_text_color();

void print_with_header(char *header, color_n header_color, char *body, char *footer);
void print_token_error(scanner_t *scan, token_t *tok, char *error_message, char *extra_message);

#endif
