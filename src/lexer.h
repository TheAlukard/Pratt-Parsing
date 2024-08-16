#pragma once

#include "list.h"

typedef enum {
    TOKEN_NUM, 
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_CARET,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_COMMA,
    TOKEN_ANS,
    TOKEN_IDENTIFIER,
    TOKEN_END,
    TOKEN_ERROR,
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    int len;
} Token;

typedef struct {
    const char *text;
    int current;
} Lexer;

LIST_DEF(TokenList, Token);

Lexer lexer_new(const char *text);
TokenList tokenize(const char *text);
void print_tokenlist(TokenList *list);

