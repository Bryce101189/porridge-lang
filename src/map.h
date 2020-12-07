#ifndef __MAP_H__
#define __MAP_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct 
{
    char **keys;
    uint8_t *values;
    int len;
} map_t;

map_t *init_map();
map_t *copy_map(map_t *map);
void destroy_map(map_t *map);

void map_add(map_t *map, char *key, uint8_t value);
uint8_t map_get(map_t *map, char *key);
void map_set(map_t *map, char *key, uint8_t value);

bool map_contains_key(map_t *map, char *key);
bool map_contains_value(map_t *map, uint8_t value);

#endif
