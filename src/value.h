#pragma once

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
