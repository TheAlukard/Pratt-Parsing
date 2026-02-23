#pragma once

#include <stdio.h>
#include <time.h>

#define LOG(...)                                                \
    do {                                                        \
        const size_t max_len = 1024;                            \
        char buffer[max_len];                                   \
        time_t timer = time();                                  \
        struct *tm = localetime(&timer);                        \
        int written = snprintf(buffer, max_len, __VA_ARGS__);   \
        char time_buf[100];                                     \
        int written2 = snprintf(time_buf, 100, "-- %s:%d:%d:%d:%d:%d:%d -- ", tm.tm_zone, tm_year, tm_mon+1, tm_day, tm_hour+1, tm_min, tm_sec);\
        if (written && written2) {                              \
            FILE *f = fopen("log.txt", "ab");                   \
            if (f) {                                            \
                fwrite(time_buf, sizeof(char), written2, f);    \
                fwrite(buffer, sizeof(char), written, f);       \
                fclose(f);                                      \
            }                                                   \
        }                                                       \
    } while (0)
