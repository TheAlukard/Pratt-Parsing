#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef enum {
	VALUE_NUM,
	VALUE_STR,
	VALUE_BOOL,
} ValueType;

typedef struct {
	char *data;
	size_t len;
} String;

typedef struct {
	ValueType type;
	union {
		double num;
		String str;
		bool   bol;
	} as;
} Value;

#define AS_NUM(val)  ((val).as.num)
#define AS_STR(val)  ((val).as.str)
#define AS_BOOL(val) ((val).as.bol)

#define VAL_NUM(val)  ((Value){.type = VALUE_NUM,  .as = {.num = val}})
#define VAL_STR(val)  ((Value){.type = VALUE_STR,  .as = {.str = val}})
#define VAL_BOOL(val) ((Value){.type = VALUE_BOOL, .as = {.bol = val}})