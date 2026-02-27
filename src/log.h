#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdarg.h>

typedef struct {
    const char *path;
    FILE *file;
} LoginInfo;

bool log_info(LoginInfo *li, const char *s, ...);
