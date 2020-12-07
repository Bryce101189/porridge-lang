#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "scanner.h"
#include "token.h"
#include "console.h"

scanner_t *init_scanner(char *source)
{
    if(source == NULL)
        return NULL;

    scanner_t *scan = malloc(sizeof(scanner_t));

    scan->source = source;
    scan->len = strlen(source);
    scan->pc = 0;

    return scan;
}

void destroy_scanner(scanner_t *scan)
{
    if(scan == NULL)
        return;

    if(scan->source != NULL)
        free(scan->source);

    free(scan);
}

bool reached_end(scanner_t *scan)
{
    return !(scan->pc < scan->len);
}

void advance(scanner_t *scan)
{
    if(!reached_end(scan))
    {
        scan->pc++;
        scan->lc++;
    }
}

void skip_whitespace(scanner_t *scan)
{
    while(isblank(peek_char(scan)))
    {
        advance(scan);
    }
}

char peek_char(scanner_t *scan)
{
    return scan->source[scan->pc];
}

char get_char(scanner_t *scan)
{
    char c = peek_char(scan);
    advance(scan);
    return c;
}

token_t *get_next_token(scanner_t *scan)
{
    if(scan == NULL)
        return NULL;

    if(reached_end(scan))
        return init_token(TOK_EOF, NULL, scan->line, scan->lc);

    if(isspace(peek_char(scan)))
        skip_whitespace(scan);

    char c = get_char(scan);

    switch(c)
    {
    case '!':
        return init_token(TOK_BANG, NULL, scan->line, scan->lc);
        break;
    case '{':
        return init_token(TOK_LBRACE, NULL, scan->line, scan->lc);
        break;
    case '}':
        return init_token(TOK_RBRACE, NULL, scan->line, scan->lc);
        break;
    case '+':
        return init_token(TOK_PLUS, NULL, scan->line, scan->lc);
        break;
    case '-':
        return init_token(TOK_MINUS, NULL, scan->line, scan->lc);
        break;
    case '=':
        return init_token(TOK_EQUAL, NULL, scan->line, scan->lc);
        break;
    case '~':
        return init_token(TOK_IF_ZERO, NULL, scan->line, scan->lc);
        break;
    case '?':
        return init_token(TOK_NOT_ZERO, NULL, scan->line, scan->lc);
        break;
    case '@':
        return init_token(TOK_PRINT, NULL, scan->line, scan->lc);
        break;
    case '#':
        return init_token(TOK_PRINT_NUM, NULL, scan->line, scan->lc);
        break;
    case '$':
        return init_token(TOK_INPUT, NULL, scan->line, scan->lc);
        break;
    case '>':
        return init_token(TOK_JUMP, NULL, scan->line, scan->lc);
        break;
    case '<':
        return init_token(TOK_RETURN, NULL, scan->line, scan->lc);
        break;

    case ';':
        while(peek_char(scan) != '\n' && !reached_end(scan))
            advance(scan);

        return get_next_token(scan);
        break;

    case '\n':
    case '\r':
        scan->line++;
        scan->lc = 0;

        return init_token(TOK_NEWLINE, NULL, scan->line, scan->lc);
        break;

    case '\0':
        return init_token(TOK_EOF, NULL, scan->line, scan->lc);
        break;
    
    default:
        if(isalnum(c) || c == '_')  // Get identifier
        {
            int len = 1;
            char *raw = malloc(len);
            raw[0] = c;


            while(isalnum(peek_char(scan)) || peek_char(scan) == '_')
            {
                len++;
                raw = realloc(raw, len);
                raw[len - 1] = get_char(scan);
            }

            raw = realloc(raw, len + 1);
            raw[len] = '\0';

            return init_token(TOK_ID, raw, scan->line, scan->lc);
        }
        else
        {
            token_t *tok = init_token(TOK_ERROR, NULL, scan->line, scan->lc);

            print_token_error(scan, tok, "unrecognized symbol in script", NULL);

            return tok;
        }
        break;
    }
}
