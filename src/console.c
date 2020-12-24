#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "console.h"
#include "token.h"
#include "scanner.h"

style_n stl = STL_NONE;
color_n col = COL_NONE;

char *get_line(char *text, int line)
{
    int len = strlen(text);

    int lines = 0;
    int start, end;

    // Get start position
    for(int i = 0; i < len; i++)
    {
        if(lines == line || i + 1 == len)
        {
            start = i;
            break;
        }

        if(text[i] == '\n' || text[i] == '\r')
            lines++;
    }

    // Get end position
    for(int i = start; i < len; i++)
    {
        if(text[i] == '\n' || text[i] == '\r')
        {
            end = i;
            break;
        }

        if(i + 1 == len)
        {
            end = i + 1;
            break;
        }
    }

    // Make sub-string
    char *substr = malloc((end - start) + 1);
    memcpy(substr, text + start, end - start);
    substr[end - start] = '\0';

    return substr;
}

void format_update()
{
    printf("\e[%i;%im", stl, col);
}

void set_text_style(style_n style)
{
    stl = style;
    format_update();
}

void set_text_color(color_n color)
{
    col = color;
    format_update();
}

void reset_text_color()
{
    stl = STL_NONE;
    col = COL_NONE;
    format_update();
}

void print_with_header(char *header, color_n header_color, char *body, char *footer)
{
    reset_text_color();

    if(header != NULL)
    {
        set_text_style(STL_BOLD);
        set_text_color(header_color);

        printf("%s: ", header);

        reset_text_color();
    }

    if(body != NULL)
        printf("%s", body);

    printf("\n");   // Padding

    if(footer != NULL)
    {
        set_text_style(STL_DIM);
        set_text_color(COL_WHITE);
        
        printf(" %s\n", footer);

        reset_text_color();
    }
}

void print_token_error(scanner_t *scan, token_t *tok, char *error_message, char *extra_message)
{
    if(scan == NULL || tok == NULL)
        return;

    printf("\n");

    reset_text_color();

    // print red 'error: ' text
    set_text_style(STL_BOLD);
    set_text_color(COL_RED);

    printf("%s: ", "error");

    // print error message
    reset_text_color();

    if(error_message != NULL)
        printf("%s", error_message);

    printf("\n");   // Padding

    // error location
    set_text_style(STL_DIM);
    set_text_color(COL_WHITE);
    
    printf(" error located on line %i, column %i\n", tok->line + 1, tok->lc);

    reset_text_color();

    if(scan->source == NULL)
        return;

    printf("\n");
    
    char *src = malloc(scan->len + 1);
    src[scan->len] = '\0';
    memcpy(src, scan->source, scan->len);

    char *line = get_line(src, tok->line);

    if(line == NULL)
    {
        print_with_header
        (
            "error", COL_RED,
            "failed to get line from source",
            NULL
        );

        return;
    }

    char *side_text = malloc(15);
    sprintf(side_text, " %i | ", tok->line + 1);

    set_text_style(STL_DIM);
    set_text_color(COL_WHITE);

    printf("%s", side_text);

    int len = strlen(line);
    int rawlen = 1;

    if(tok->raw != NULL)
        rawlen = strlen(tok->raw);
    
    bool comment = false;
    bool remove_spaces = true;
    int whitespace = 0;

    for(int i = 0; i < len; i++)
    {
        reset_text_color();

        if(line[i] == ';' || comment)  // Comment
        {
            set_text_style(STL_DIM);
            set_text_color(COL_GREEN);

            comment = true;
        }
        else if(isalnum(line[i]) || line[i] == '_')  // Identifier
        {
            set_text_style(STL_NONE);
            set_text_color(COL_CYAN);
        }
        else if(is_valid_symbol(line[i]))   // Symbol
        {
            set_text_style(STL_DIM);
            set_text_color(COL_WHITE);
        }
        else 
        {
            set_text_style(STL_DIM);
            set_text_color(COL_RED);
        }

        if(i + 1 <= tok->lc && i + 1 > tok->lc - rawlen)
        {
            reset_text_color();

            set_text_style(STL_BOLD);
            set_text_color(COL_RED);
        }

        if(isblank(line[i]) && remove_spaces)
        {
            whitespace++;
        }
        else
        {
            remove_spaces = false;
            putchar(line[i]);
        }
    }

    // Print clarification pointer
    printf("\n ");  // Padding

    reset_text_color();

    set_text_style(STL_BOLD);
    set_text_color(COL_RED);

    int side_length = strlen(side_text) - 1;

    for(int i = 0; i < tok->lc - rawlen + side_length - whitespace; i++)
        putchar(' ');

    for(int i = 0; i < rawlen; i++)
        putchar('^');

    if(extra_message != NULL)
        printf(" %s", extra_message);

    printf("\n");

    reset_text_color();
}
