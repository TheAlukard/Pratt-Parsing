#include "arena.h"
#include <stdlib.h>

Arena arena_init(size_t capacity)
{
    Arena arena;
    arena.capacity = capacity;
    arena.ptr = 0;
    arena.data = malloc(sizeof(*arena.data) * arena.capacity);

    return arena;
}

void arena_deinit(Arena *arena)
{
    free(arena->data);
    arena->data = NULL;
    arena->capacity = 0;
    arena->ptr = 0;
}

void* arena_alloc(Arena *arena, size_t size)
{
    if (size > arena->capacity) return NULL;

    if (arena->ptr + size > arena->capacity) {
        arena->ptr = 0;
    }

    void *ptr = &arena->data[arena->ptr];
    arena->ptr += size;

    return ptr;
}
