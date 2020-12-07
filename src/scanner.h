#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <stdbool.h>
#include "token.h"

typedef struct
{
    char *source;
    long len;
    long pc;

    int line;
    short lc;
} scanner_t;

scanner_t *init_scanner(char *source);
void destroy_scanner(scanner_t *scan);

bool reached_end(scanner_t *scan);
void advance(scanner_t *scan);
void skip_whitespace(scanner_t *scan);
char peek_char(scanner_t *scan);
char get_char(scanner_t *scan);

token_t *get_next_token(scanner_t *scan);

#endif
