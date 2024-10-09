#include "value.h"
#include <stdlib.h>
#include <string.h>

String string_create(char *text, size_t len)
{
	String str;
	str.len = len;
	str.data = malloc(sizeof(char) * str.len);
	memcpy(str.data, text, sizeof(char) * len);

	return str;
}
