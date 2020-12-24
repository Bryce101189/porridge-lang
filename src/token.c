#include <stdlib.h>
#include <stdbool.h>

#include "token.h"


token_t *init_token(tokentype_n type, char *raw, int line, int lc)
{
    token_t *tok = malloc(sizeof(token_t));
    
    tok->type = type;
    tok->raw = raw;
    tok->line = line;
    tok->lc = lc;
    
    return tok;
}

void destroy_token(token_t *token)
{
    if(token == NULL)
        return;

    if(token->raw != NULL)
        free(token->raw);

    free(token);
}

bool is_valid_symbol(char c)
{
    return c == '!' || c == '{' || c == '}' || c == '+' || c == '-' || c == '=' || c == '?'
        || c == '~' || c == '#' || c == '@' || c == '$' || c == '>' || c == '<' || c == ';';
}
