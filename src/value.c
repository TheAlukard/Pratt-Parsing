#include "value.h"
#include "arena.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

String string_create(const char *text, size_t len)
{
    String str;
    str.len = len;
    str.data = malloc(sizeof(char) * (str.len + 1));
    memcpy(str.data, text, sizeof(char) * len);
    str.data[str.len] = '\0';

    return str;
}

void string_destroy(String *string)
{
    free(string->data);
    string->data = NULL;
    string->len = 0;
}

String string_create_arena(Arena *arena, const char *text, size_t len)
{
    String str;
    str.len = len;
    str.data = arena_alloc(arena, sizeof(char) * (str.len + 1));
    memcpy(str.data, text, sizeof(char) * len);
    str.data[str.len] = '\0';

    return str;
}

void value_to_str(char *buffer, Value *value)
{
    switch (value->type) {
        case VALUE_NUM:
            sprintf(buffer, "%0.15Lf", AS_NUM(*value));
            break;
        case VALUE_STR:
            sprintf(buffer, "%.*s", (int)AS_STR(*value).len, AS_STR(*value).data);
            break;
        case VALUE_BOOL:
            sprintf(buffer, "%s", AS_BOOL(*value) ? "true" : "false");
            break;
    }
}

char* value_type_to_str(ValueType type)
{
    switch (type) {
        case VALUE_NUM: return "Number";
        case VALUE_STR: return "String";
        case VALUE_BOOL: return "Bool";
    }

    return NULL;
}

String string_add(Arena *arena, String *one, String *two)
{
    String str;
    str.len = one->len + two->len;
    str.data = arena_alloc(arena, str.len * sizeof(char));
    memcpy(str.data, one->data, one->len * sizeof(char));
    memcpy(&str.data[one->len], two->data, two->len * sizeof(char));

    return str;
}

bool string_compare(String* one, String *two)
{
    if (one->len == two->len) {
        return memcmp(one->data, two->data, one->len * sizeof(char)) == 0;
    }

    return false;
}

