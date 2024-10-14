#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t *data;
    size_t capacity;
    size_t ptr;
} Arena;

Arena arena_init(size_t capacity);
void arena_deinit(Arena *arena);
void* arena_alloc(Arena *arena, size_t capacity);
