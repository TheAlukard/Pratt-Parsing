#pragma once

#include <stdio.h>

#define LOG(...)                                  \
    do {                                          \
        const size_t max_len = 1024;              \
        char buffer[max_len];                     \
        snprintf(buffer, max_len, __VA_ARGS__);   \
        FILE *f = fopen("log.txt", "ab");         \
        fwrite(buffer, sizeof(char), max_len, f); \
        fclose(f);                                \
    } while (0)
