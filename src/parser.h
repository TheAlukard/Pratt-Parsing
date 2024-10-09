#pragma once

#include "lexer.h"
#include "map.h"
#include "value.h"

typedef enum {
    PREC_NONE,
    PREC_ADSUB,
    PREC_MULDIV,
    PREC_POW,
    PREC_UNARY,
} precedence;

typedef struct {
    TokenList *tokens;
    int current;
    Value ans;
    Map map;
} Parser;

typedef Value (*ParseFn)(Parser *parser);

typedef struct {
    ParseFn prefix;
    ParseFn infix; 
    int lbp;
} ParseRule;

Parser parser_create();
void parser_reset(Parser *parser, TokenList *list);
Value expression(Parser *parser, precedence rbp);
Value parse_expr(Parser *parser);
