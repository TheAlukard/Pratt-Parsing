#pragma once

#include <stdio.h>

#define LOG(...)                                                \
    do {                                                        \
        const size_t max_len = 1024;                            \
        char buffer[max_len];                                   \
        int written = snprintf(buffer, max_len, __VA_ARGS__);   \
        if (written) {                                          \
            FILE *f = fopen("log.txt", "ab");                   \
            if (f) {                                            \
                fwrite(buffer, sizeof(char), written, f);       \
                fclose(f);                                      \
            }                                                   \
        }                                                       \
    } while (0)
