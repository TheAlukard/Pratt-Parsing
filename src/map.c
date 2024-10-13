#include "map.h"
#include <stdlib.h>
#include <stdint.h>

uint32_t hash(Token token)
{
    uint32_t hash = 5381;
    int c = 1;

    for (int i = 0; i < token.len; i++) {
        c = token.start[i];
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

Map map_new()
{
    Map map;
    map.capacity = DEFAULT_MAP_CAP;
    map.count = 0;
    map.items = (Map_Node*)calloc(map.capacity, sizeof(Map_Node));
    map.hash = hash;

    return map;
}

void map_delete(Map *map)
{
    free(map->items);
    map->items = NULL;
    map->capacity = 0;
    map->count = 0;

    return;
}

uint32_t map_get_hash(Map *map, KEY key)
{
    return map->hash(key) % map->capacity;
}

uint32_t map_get_i(Map *map, KEY key)
{
    uint32_t hash_value = map_get_hash(map, key);

    for (uint32_t i = 0; i < map->capacity; i++) {
        uint32_t index = (hash_value + i) % map->capacity;

        if (map->items[index].valid && CMP(key, map->items[index].key)) {
            return index;
        }
    }

    return 0;
}

bool map_set(Map *map, KEY key, VALUE value)
{
    uint32_t hash_value = map_get_hash(map, key);
    
    for (uint32_t i = 0; i < map->capacity; i++) {
        uint32_t index = (hash_value + i) % map->capacity;

        if (!map->items[index].valid) {
            map->items[index].key = key;
            map->items[index].value = value;
            map->items[index].valid = true;
            map->count++;

            return true;
        }
        else if (CMP(key, map->items[index].key)) { 
            map->items[index].value = value;

            return true;
        }
    }
    
    return false;
}

VALUE map_get(Map *map, KEY key)
{
    uint32_t hash_value = map_get_hash(map, key);
    
    for (uint32_t i = 0; i < map->capacity; i++) {
        uint32_t index = (hash_value + i) % map->capacity;

        if (map->items[index].valid && CMP(key, map->items[index].key)) {
            return map->items[index].value;
        }
    }
    
    return (VALUE){0};
}

bool map_has(Map *map, KEY key)
{
    uint32_t hash_value = map_get_hash(map, key);
    
    for (uint32_t i = 0; i < map->capacity; i++) {
        uint32_t index = (hash_value + i) % map->capacity;

        if (map->items[index].valid && CMP(key, map->items[index].key)) {
            return true;
        }
    }
    
    return false;
}
