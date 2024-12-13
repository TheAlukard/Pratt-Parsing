#include "map.h"
#include <stdlib.h>
#include <stdint.h>

uint32_t hash(String key)
{
    uint32_t hash = 5381;
    int c = 1;

    for (size_t i = 0; i < key.len; i++) {
        c = key.data[i];
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

uint32_t map_get_hash(Map *map, MAP_KEY key)
{
    return map->hash(key) % map->capacity;
}

uint32_t map_get_i(Map *map, MAP_KEY key)
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

bool map_set(Map *map, MAP_KEY key, MAP_VALUE value)
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

MAP_VALUE map_get(Map *map, MAP_KEY key)
{
    uint32_t hash_value = map_get_hash(map, key);
    
    for (uint32_t i = 0; i < map->capacity; i++) {
        uint32_t index = (hash_value + i) % map->capacity;

        if (map->items[index].valid && CMP(key, map->items[index].key)) {
            return map->items[index].value;
        }
    }
    
    return (MAP_VALUE){0};
}

bool map_has(Map *map, MAP_KEY key)
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

MAP_VALUE* map_array(Map *map)
{
    if (map->count <= 0 || map->count > map->capacity) return NULL;

    MAP_VALUE *array = malloc(sizeof(MAP_VALUE) * map->count);

    for (size_t i = 0, j = 0; i < map->capacity && j < map->count; i++) {
        if (map->items[i].valid) {
            array[j++] = map->items[i].value;
        }
    }

    return array;
}
