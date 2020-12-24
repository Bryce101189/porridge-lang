#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "map.h"

map_t *init_map()
{
    map_t *map = malloc(sizeof(map));
    
    map->keys = NULL;
    map->values = NULL;
    map->len = 0;
    
    return map;
}

map_t *copy_map(map_t *map)
{
    map_t *copy = malloc(sizeof(map));

    copy->keys = malloc(map->len * sizeof(char*));
    memcpy(copy->keys, map->keys, map->len * sizeof(char*));

    copy->values = malloc(map->len * sizeof(uint8_t));
    memcpy(copy->values, map->values, map->len * sizeof(uint8_t));

    copy->len = map->len;

    return copy;
}

void destroy_map(map_t *map)
{
    if(map == NULL)
        return;

    if(map->keys != NULL)
        free(map->keys);

    if(map->values != NULL)
        free(map->values);

    free(map);
}

void map_add(map_t *map, char *key, uint8_t value)
{
    if(map == NULL)
        return;

    if(map_contains_key(map, key))
        return;

    map->len++;
    map->keys = realloc(map->keys, map->len * sizeof(char*));
    map->values = realloc(map->values, map->len * sizeof(uint8_t));

    map->keys[map->len - 1] = key;
    map->values[map->len - 1] = value;
}

uint8_t map_get(map_t *map, char *key)
{
    if(map == NULL)
        return -1;

    for(int i = 0; i < map->len; i++)
    {
        if(map->keys[i][0] == key[0])
        {
            if(!strcmp(map->keys[i], key))
                return map->values[i];
        } 
    }

    return 0;
}

void map_set(map_t *map, char *key, uint8_t value)
{
    if(map == NULL)
        return;

    for(int i = 0; i < map->len; i++)
    {
        if(map->keys[i][0] == key[0])
        {
            if(!strcmp(map->keys[i], key))
            {
                map->values[i] = value;
            }
        } 
    }
}

void map_clear(map_t *map)
{
    if(map == NULL)
        return;

    if(map->keys != NULL)
        free(map->keys);
    if(map->values != NULL)
        free(map->values);
    
    map->len = 0;
}

bool map_contains_key(map_t *map, char *key)
{
    if(map == NULL)
        return false;

    for(int i = 0; i < map->len; i++)
    {
        if(map->keys[i][0] == key[0])
        {
            if(!strcmp(map->keys[i], key))
                return true;
        } 
    }

    return false;
}

bool map_contains_value(map_t *map, uint8_t value)
{
    if(map == NULL)
        return false;

    for(int i = 0; i < map->len; i++)
    {
        if(map->values[i] == value)
            return true;
    }

    return false;
}
