#pragma once

#ifndef _MAP_H
#define _MAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lexer.h"
#include "value.h"

#define KEY Token
#define VALUE Value

#define CMP(key, other) (memcmp((key).start, (other).start, (key).len > (other).len ? (key).len : (other).len) == 0) 

typedef struct {
    KEY key;
    VALUE value;
    bool valid;
} Map_Node;

typedef struct {
    Map_Node *items;
    size_t count;
    size_t capacity;
    uint32_t(*hash)(KEY);
} Map;

#define DEFAULT_MAP_CAP 300 

Map map_new(void);
void map_delete(Map *map);
bool map_set(Map *map, KEY key, VALUE value);
VALUE map_get(Map *map, KEY key);
uint32_t map_get_hash(Map *map, KEY key);
uint32_t map_get_i(Map *map, KEY key);
bool map_has(Map *map, KEY key);
uint32_t map_get_hash(Map *map, KEY key);
uint32_t map_get_i(Map *map, KEY key);

#endif // _MAP_H
