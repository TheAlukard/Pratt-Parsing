#pragma once

#include "lexer.h"

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
    double ans;
} Parser;

typedef double (*ParseFn)(Parser *parser);

typedef struct {
    ParseFn prefix;
    ParseFn infix; 
    int lbp;
} ParseRule;

void parser_new(Parser *parser, TokenList *list);
double expression(Parser *parser, precedence rbp);
double parse_expr(Parser *parser);
