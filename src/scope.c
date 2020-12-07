#include <stdlib.h>

#include "scope.h"

scope_t *init_scope(scope_t *prev)
{
    scope_t *scope = malloc(sizeof(scope_t));

    scope->vars = init_map();
    scope->subs = init_map();
    scope->prev = (scope_t*)prev;
    scope->pc = 0;

    return scope;
}

void destroy_scope(scope_t *scope)
{
    if(scope == NULL)
        return;

    if(scope->vars != NULL)
        destroy_map(scope->vars);

    if(scope->subs != NULL)
        destroy_map(scope->subs);

    free(scope);
}
