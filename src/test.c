#include "test.h"
#include <time.h>
#include <stdlib.h>

bool get_random_str(char *buff, size_t len)
{
    if (len <= 0 || !buff) return false;

    size_t i = 0;
    for (; i < len - 1; i++) {
        char c = rand() % 128;
        buff[i] = c;
    }

    buff[i] = '\0';

    return true;
}
