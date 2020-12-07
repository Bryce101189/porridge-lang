#ifndef __SCOPE_H__
#define __SCOPE_H__

#include "map.h"

typedef struct
{
    map_t *vars;
    map_t *subs;
    void *prev;
    int pc;
} scope_t;

scope_t *init_scope(scope_t *prev);
void destroy_scope(scope_t *scope);

#endif
