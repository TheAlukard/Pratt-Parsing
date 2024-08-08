#pragma once

#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define INLINE __attribute__((always_inline)) static inline

#define array_len(array) (sizeof(array) / sizeof((array)[0]))

#ifndef DEFAULT_LIST_CAP
  #define DEFAULT_LIST_CAP 32
#endif

#define list_of(type) \
  struct {\
    type *items;\
    size_t count;\
    size_t capacity;\
  }

#define LIST_DEF(name, type) \
  typedef struct {\
    type *items;\
    size_t count;\
    size_t capacity;\
  } name;\

#define list_alloc(list)                                                    \
  do {                                                                      \
    (list)->capacity = DEFAULT_LIST_CAP;                                    \
    (list)->count = 0;                                                      \
    (list)->items = malloc((list)->capacity * sizeof(*(list)->items));      \
  } while (0)

#define list_free(list)                                                     \
  do {                                                                      \
    if ((list)->items != NULL) {                                            \
      free((list)->items);                                                  \
      (list)->items = NULL;                                                 \
    }                                                                       \
    (list)->count = 0;                                                      \
    (list)->capacity = 0;                                                   \
  } while (0)

#define list_push(list, item)                                                            \
  do {                                                                                   \
    if ((list)->count >= (list)->capacity) {                                             \
      (list)->capacity *= 2;                                                             \
      (list)->items = realloc((list)->items, (list)->capacity * sizeof(*(list)->items)); \
    }                                                                                    \
    (list)->items[(list)->count] = item;                                                 \
    (list)->count += 1;                                                                  \
  } while (0)

INLINE void* LIST_GET_POPPED(void* *list_items, size_t type_size, size_t *list_count, size_t *list_cap) 
{
    void *popped = NULL; 

    if (*list_count == 0) return popped;

    if (*list_count < (*list_cap) / 3) {
        *list_cap /= 2;
        *list_items = realloc(*list_items, (*list_cap) * type_size);
    }

    *list_count -= 1;

    popped = (uint8_t*)(*list_items) + ((*list_count) * type_size);

    return popped;
}

#define list_pop(list) (*(typeof(*(list)->items)*)LIST_GET_POPPED((void*)(&(list)->items), sizeof(*(list)->items), &(list)->count, &(list)->capacity))

#define list_copy(dest, src, start, count)                                   \
  do {                                                                       \
    if ((start) < 0) {                                                       \
      break;                                                                 \
    }                                                                        \
    size_t i = (start);                                                      \
    size_t j = 0;                                                            \
    while (i < (count) && i < (src)->count && j < (dest)->count) {           \
      (dest)->items[j] = (src)->items[i];                                    \
      i += 1;                                                                \
      j += 1;                                                                \
    }                                                                        \
  } while (0)

#define list_transfer(dest, src)                                             \
  do {                                                                       \
    if ((dest)->items != NULL) {                                             \
      free((dest)->items);                                                   \
      (dest)->items = NULL;                                                  \
    }                                                                        \
    (dest)->items = (src)->items;                                            \
    (dest)->capacity = (src)->capacity;                                      \
    (dest)->count = (src)->count;                                            \
  } while (0)

#define list_print(list, format)                                             \
  do {                                                                       \
    printf("[");                                                             \
    for (size_t i = 0; i < (list)->count; i++) {                             \
      printf(format, (list)->items[i]);                                      \
      if (i < (list)->count - 1) {                                           \
        printf(", ");                                                        \
      }                                                                      \
    }                                                                        \
    printf("]\n");                                                           \
  } while (0)

#define list_clear(list) ((list)->count = 0)

#endif // _LIST_H_
