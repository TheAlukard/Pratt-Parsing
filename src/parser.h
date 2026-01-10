#pragma once

#include "lexer.h"
#include "map.h"
#include "value.h"
#include "arena.h"
#include <stdbool.h>

typedef enum {
    PREC_NONE,
    PREC_OR,
    PREC_AND,
    PREC_EQUALITY,
    PREC_COMP,
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
    Arena arena;
    bool error;
} Parser;

typedef Value (*ParseFn)(Parser *parser);

typedef struct {
    ParseFn prefix;
    ParseFn infix; 
    int lbp;
} ParseRule;

Parser parser_create();
void parser_destroy(Parser *parser);
void parser_reset(Parser *parser, TokenList *list);
Value expression(Parser *parser, precedence rbp, TokenType expected_first_token);
Value parse_expr(Parser *parser);
