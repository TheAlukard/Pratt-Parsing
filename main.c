#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
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
    TOKEN_END,
    TOKEN_ERROR,
} TokenType;

typedef enum {
    OP_NUM,
    OP_ADD,
    OP_SUB,
    OP_NEGATE,
    OP_MUL,
    OP_DIV,
    OP_POW,
} OpCode;

typedef enum {
    PREC_NONE,
    PREC_ADSUB,
    PREC_MULDIV,
    PREC_POW,
    PREC_UNARY,
} precedence;

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
LIST_DEF(OpStack, int);

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

double unary(Parser *parser);
double binary(Parser *parser);
double number(Parser *parser);
double grouping(Parser *parser);

ParseRule rules[] = {
    {number, NULL, PREC_NONE},
    {NULL, binary, PREC_ADSUB},
    {unary, binary, PREC_ADSUB},
    {NULL, binary, PREC_MULDIV},
    {NULL, binary, PREC_MULDIV},
    {NULL, binary, PREC_POW},
    {grouping, NULL, PREC_NONE},
    {NULL, NULL, PREC_NONE},
    {NULL, NULL, PREC_NONE},
    {NULL, NULL, PREC_NONE},
};

ParseRule* get_rule(Token token)
{
    return &rules[token.type];
}

Lexer lexer_new(const char *text)
{
    Lexer lexer = {
        .text = text,
        .current = 0,
    };

    return lexer;
}

Parser parser_new(TokenList *list)
{
    Parser parser = {
        .tokens = list,
        .current = 0,
    };

    return parser;
}

Token parser_peek(Parser *parser)
{
    return parser->tokens->items[parser->current];
}

Token parser_consume(Parser *parser)
{
    return parser->tokens->items[parser->current++];
}

char lexer_peek(Lexer *lexer)
{
    return lexer->text[lexer->current];
}

char lexer_consume(Lexer *lexer)
{
    return lexer->text[lexer->current++];
}

void trim_left(Lexer *lexer)
{
    while (isspace(lexer_peek(lexer))) {
        lexer_consume(lexer);
    }
}

Token scan_token(Lexer *lexer)
{
    trim_left(lexer);

    char c = lexer_peek(lexer);
    Token token;

    if (isdigit(c)) {
        token.type = TOKEN_NUM;
        token.start = &lexer->text[lexer->current];
        while (isdigit(lexer_peek(lexer))) {
            lexer_consume(lexer);
        }
        if (lexer_peek(lexer) == '.') {
            lexer_consume(lexer);
            while (isdigit(lexer_peek(lexer))) {
                lexer_consume(lexer);
            }
        }
        token.len = &lexer->text[lexer->current] - token.start;
    }
    else if (c == '\0') {
        token.type = TOKEN_END;
        token.start = "END";
        token.len = 3;
    }
    else {
        switch (c) {
            case '+':
                token.type = TOKEN_PLUS;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                lexer_consume(lexer);
                break;
            case '-':
                token.type = TOKEN_MINUS;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                lexer_consume(lexer);
                break;
            case '*':
                token.type = TOKEN_STAR;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                lexer_consume(lexer);
                break;
            case '/':
                token.type = TOKEN_SLASH;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                lexer_consume(lexer);
                break;
            case '^':
                token.type = TOKEN_CARET;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                lexer_consume(lexer);
                break;
            case '(':
                token.type = TOKEN_LEFT_PAREN;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                lexer_consume(lexer);
                break;
            case ')':
                token.type = TOKEN_RIGHT_PAREN;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                lexer_consume(lexer);
                break;
            default:
                token.type = TOKEN_ERROR;
                token.start = &lexer->text[lexer->current];
                token.len = strlen(token.start);
                break;
        }
    }

    return token;
}

TokenList tokenize(const char *text)
{
    TokenList list;
    list_alloc(&list);
    Lexer lexer = lexer_new(text); 
    Token token = scan_token(&lexer);

    while (token.type != TOKEN_END && token.type != TOKEN_ERROR) {
        list_push(&list, token);
        token = scan_token(&lexer);
    }

    list_push(&list, token);

    return list;
}

void print_tokenlist(TokenList *list)
{
    char* token_names[] = {
        "NUM", 
        "PLUS",
        "MINUS",
        "STAR",
        "SLASH",
        "CARET", 
        "LEFT_PAREN",
        "RIGHT_PAREN",
        "END",
        "ERROR",
    };

    for (int i = 0; i < list->count; i++) {
        Token token = list->items[i];
        printf("{\n");
        printf("  type: %s\n", token_names[token.type]);
        printf("  text: %.*s\n", token.len, token.start);
        printf("}\n");
    }
}

double expression(Parser *parser, precedence rbp)
{
    Token token = parser_consume(parser);
    double result = get_rule(token)->prefix(parser);

    while (rbp < get_rule(parser_peek(parser))->lbp) {
        token = parser_consume(parser);
        double right = get_rule(token)->infix(parser); 
        switch (token.type) {
            case TOKEN_PLUS: result = result + right; break;
            case TOKEN_MINUS: result = result - right; break;
            case TOKEN_STAR: result = result * right; break;
            case TOKEN_SLASH: result = result / right; break;
            case TOKEN_CARET: result = pow(result, right); break;
            default: break;// Unreachable
        }
    }

    return result;
}

double grouping(Parser *parser)
{
    double result = expression(parser, PREC_NONE);
    parser_consume(parser);

    return result;
}

double unary(Parser *parser)
{
    double result = expression(parser, PREC_UNARY);

    return -result;
}

double binary(Parser *parser)
{
    double result = expression(parser, get_rule(parser->tokens->items[parser->current - 1])->lbp);

    return result;
}

double number(Parser *parser)
{
    Token num = parser->tokens->items[parser->current - 1];
    char temp[num.len + 1];
    memcpy(temp, num.start, num.len * sizeof(char));
    temp[num.len] = '\0';

    return strtod(temp, NULL); 
}

int main(void)
{
    const size_t buffer_len = 1000;
    char buffer[buffer_len]; 

    while (true) {
        fgets(buffer, sizeof(char) * buffer_len, stdin);
        TokenList list = tokenize(buffer);
        Parser parser = parser_new(&list);
        printf("result: %lf\n", expression(&parser, PREC_NONE));
        list_free(&list);
    }
}
