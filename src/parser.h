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
} Parser;

typedef double (*ParseFn)(Parser *parser);

typedef struct {
    ParseFn prefix;
    ParseFn infix; 
    int lbp;
} ParseRule;

Parser parser_new(TokenList *list);
double expression(Parser *parser, precedence rbp);
