#pragma once

#include "list.h"

typedef enum {
    TOKEN_NUM, 
    TOKEN_STRING,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_CARET,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_COMMA,
    TOKEN_EQUAL,
    TOKEN_DOLLAR,
    TOKEN_ANS,
    TOKEN_IDENTIFIER,
    TOKEN_LET,
    TOKEN_EXIT,
    TOKEN_OR,
    TOKEN_AND,
    TOKEN_EQEQ,
    TOKEN_NOT,
    TOKEN_NOTEQ,
    TOKEN_LESS,
    TOKEN_LESSEQ,
    TOKEN_GREATER,
    TOKEN_GREATEREQ,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_END,
    TOKEN_ERROR,
    TOKEN_COUNT,
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
void tokenize(const char *text, TokenList *output);
void print_tokenlist(TokenList *list);

