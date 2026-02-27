#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdarg.h>

typedef struct {
    const char *path;
    FILE *file;
    double flush_interval;
    time_t last_flushed;
} LoggingInfo;

LoggingInfo log_create(const char *path, FILE *file, double flush_interval);
bool log_info(LoggingInfo *li, const char *s, ...);
