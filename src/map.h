#pragma once

#ifndef _MAP_H
#define _MAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lexer.h"
#include "value.h"

#define MAP_KEY String
#define MAP_VALUE Value

#define CMP(key, other) ((key).len != (other).len ? false : memcmp((key).data, (other).data, (key).len) == 0) 

typedef struct {
    MAP_KEY key;
    MAP_VALUE value;
    bool valid;
} Map_Node;

typedef struct {
    Map_Node *items;
    size_t count;
    size_t capacity;
    uint32_t(*hash)(MAP_KEY);
} Map;

#define DEFAULT_MAP_CAP 300 

Map map_new(void);
void map_delete(Map *map);
bool map_set(Map *map, MAP_KEY key, MAP_VALUE value);
MAP_VALUE map_get(Map *map, MAP_KEY key);
uint32_t map_get_hash(Map *map, MAP_KEY key);
uint32_t map_get_i(Map *map, MAP_KEY key);
bool map_has(Map *map, MAP_KEY key);
uint32_t map_get_hash(Map *map, MAP_KEY key);
uint32_t map_get_i(Map *map, MAP_KEY key);
MAP_VALUE* map_array(Map *map);

#endif // _MAP_H
