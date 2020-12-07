#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stdbool.h>


typedef enum
{
    TOK_BANG,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_PLUS,
    TOK_MINUS,
    TOK_EQUAL,
    TOK_IF_ZERO,
    TOK_NOT_ZERO,
    TOK_PRINT,
    TOK_PRINT_NUM,
    TOK_INPUT,
    TOK_JUMP,
    TOK_RETURN,

    TOK_ID,

    TOK_NEWLINE,
    TOK_ERROR,
    TOK_EOF,
} tokentype_n;

typedef struct
{
    tokentype_n type;
    int line, lc;
    char *raw;
} token_t;

token_t *init_token(tokentype_n type, char *raw, int line, int lc);
void destroy_token(token_t *token);

bool is_valid_symbol(char c);

#endif
