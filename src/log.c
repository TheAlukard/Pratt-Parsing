#include "log.h"
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define FAIL(...) do { fprintf(stderr, __VA_ARGS__); return false; } while (0)

LoggingInfo log_create(const char *path, FILE *file, double flush_interval)
{
    if (!path) {
        fprintf(stderr, "Error 'log_create' : Empty file path\n");
        return (LoggingInfo){0};
    }

    LoggingInfo info = {
        .path = path,
        .file = file,
        .flush_interval = flush_interval > 0.f ? flush_interval : 0.25,
        .last_flushed = time(0),
    };

    return info;
}

bool log_info(LoggingInfo *li, const char *s, ...)
{
    if (!li)
        FAIL("Error: Login Info is NULL\n");
    if (!li->path)
        FAIL("Error 'log_info' : Empty file path\n");

    va_list args;
    enum { buff_len = 1024, t_len = 128 };
    char buffer[buff_len];
    char t_buff[t_len];
    time_t timer;
    time(&timer);
    struct tm t = {0};

    if (localtime_r(&timer, &t) != &t)
        FAIL("Failed to get local time\n");

    size_t time_written = strftime(t_buff, t_len, "-- %Z %d-%m-%Y %H:%M:%S -- ", &t);

    if (!time_written)
        FAIL("Failed to parse local time\n");

    va_start(args, s);
    int buff_written = vsnprintf(buffer, buff_len, s, args);
    va_end(args);

    if (buff_written < 0 || buff_written >= buff_len)
        FAIL("Failed to parse logging info\n");

    if (!li->file) {
        li->file = fopen(li->path, "ab");
        if (!li->file)
            FAIL("Failed to open '%s'\n", li->path); 
    }

    size_t time_fwrite = fwrite(t_buff, sizeof(char), time_written, li->file);
    if (time_fwrite != (size_t)time_written)
        FAIL("Failed to write time data to '%s'\n", li->path);

    size_t buff_fwrite = fwrite(buffer, sizeof(char), buff_written, li->file);
    if (buff_fwrite != (size_t)buff_written)
        FAIL("Failed to write logging data to '%s'\n", li->path);

    size_t newline = fwrite("\n", 1, 1, li->file);
    if (newline != 1)
        FAIL("Failed to write newline to '%s'\n", li->path);

    double elapsed = difftime(time(0), li->last_flushed);

    if (elapsed >= li->flush_interval) {
        li->last_flushed = time(0);
        if (fflush(li->file) != 0)
            FAIL("Failed to flush file '%s'\n", li->path);

        printf("Flushed!\n");
    }

    return true;
}
