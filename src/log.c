#include "log.h"

bool log_info(const char *s, ...)
{
    #define buff_len 1024
    #define t_len 50

    va_list args;
    va_start(args, s);
    static char buffer[buff_len];
    static char t_buff[50];
    time_t timer;
    time(&timer);
    struct tm *t = localtime(&timer);
    const char *time_zone = t->tm_zone;
    const int year = t->tm_year + 1900;
    const int month = t->tm_mon + 1;
    const int day= t->tm_mday;
    const int hour= t->tm_hour;
    const int minute= t->tm_min;
    const int second= t->tm_sec;

    int time_w = snprintf(t_buff, t_len, "-- %s %d-%d-%d %d:%d:%d -- ", time_zone, day, month, year, hour, minute, second);
    int buff_w = vsnprintf(buffer, buff_len, s, args);

    bool success = true;

    if (time_w && buff_w) {
        FILE *f = fopen("log.txt", "ab");

        if (f) {
            fwrite(t_buff, sizeof(char), time_w, f);
            fwrite(buffer, sizeof(char), buff_w, f);
            fclose(f);
        }
        else success = false;
    }
    else success = false;

    va_end(args);

    return success;

    #undef buff_len
    #undef t_len
}
