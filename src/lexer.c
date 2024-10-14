#include "lexer.h"
#include "string.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

Lexer lexer_new(const char *text)
{
    Lexer lexer = {
        .text = text,
        .current = 0,
    };

    return lexer;
}

static char peek(Lexer *lexer)
{
    return lexer->text[lexer->current];
}

static char consume(Lexer *lexer)
{
    return lexer->text[lexer->current++];
}

static bool is_space(char c)
{
    return c == ' '  ||
           c == '\n' ||
           c == '\t' ||
           c == '\r';
}

static bool is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

static bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static bool is_alnum(char c)
{
    return is_alpha(c) || is_digit(c);
}

static void trim_left(Lexer *lexer)
{
    while (is_space(peek(lexer))) {
        consume(lexer);
    }
}

static Token match_identifier(Lexer *lexer, const char *text, int len, TokenType type)
{
    int start = lexer->current;

    Token token = {0};

    while (is_alnum(peek(lexer))) {
        consume(lexer);
    }

    if (lexer->current - start == len) {
        if (memcmp(&lexer->text[start], text, len) == 0) {
            token.type = type;
        }
    }
    else {
        token.type = TOKEN_IDENTIFIER;
    }

    token.start = &lexer->text[start];
    token.len = lexer->current - start;

    return token;
}

static void match(Lexer *lexer, Token *token, char expected, TokenType type)
{
    if (peek(lexer) == expected) {
        consume(lexer);
        token->len = 2;
        token->type = type;
    }
    else {
        fprintf(stderr, "Error: Unexpected Token\n");
    }
}

static void this_or_that(Lexer *lexer, Token *token, char expected, TokenType first_type, TokenType second_type)
{
    if (peek(lexer) == expected) {
        consume(lexer);
        token->len = 2;
        token->type = second_type;
    }
    else {
        token->len = 1;
        token->type = first_type;
    }
}

static Token parse_string_literal(Lexer *lexer)
{
    const char quote = consume(lexer);

    Token token;

    token.start = &lexer->text[lexer->current];

    while (peek(lexer) != '\0' && peek(lexer) != quote) {
        if (peek(lexer) == '\\') {
            consume(lexer);
        }
        consume(lexer); 
    }

    token.len = &lexer->text[lexer->current] - token.start;
    token.type = TOKEN_STRING;

    if (consume(lexer) != quote) {
        fprintf(stderr, "Error: Mismatching quotes.");
        exit(1);
    }
    
    return token;
}

static Token scan_token(Lexer *lexer)
{
    trim_left(lexer);

    char c = peek(lexer);
    Token token;

    if (is_digit(c)) {
        token.type = TOKEN_NUM;
        token.start = &lexer->text[lexer->current];
        while (is_digit(peek(lexer))) {
            consume(lexer);
        }
        if (peek(lexer) == '.') {
            consume(lexer);
            while (is_digit(peek(lexer))) {
                consume(lexer);
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
            case '\'':
            case '"':
                token = parse_string_literal(lexer);
                break;
            case '+':
                token.type = TOKEN_PLUS;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                consume(lexer);
                break;
            case '-':
                token.type = TOKEN_MINUS;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                consume(lexer);
                break;
            case '*':
                token.type = TOKEN_STAR;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                consume(lexer);
                break;
            case '/':
                token.type = TOKEN_SLASH;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                consume(lexer);
                break;
            case '^':
                token.type = TOKEN_CARET;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                consume(lexer);
                break;
            case '(':
                token.type = TOKEN_LEFT_PAREN;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                consume(lexer);
                break;
            case ')':
                token.type = TOKEN_RIGHT_PAREN;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                consume(lexer);
                break;
            case ',':
                token.type = TOKEN_COMMA;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                consume(lexer);
                break;
            case '$':
                token.type = TOKEN_DOLLAR;
                token.start = &lexer->text[lexer->current];
                token.len = 1;
                consume(lexer);
                break;
            case '=':
                token.start = &lexer->text[lexer->current];
                consume(lexer);
                this_or_that(lexer, &token, '=', TOKEN_EQUAL, TOKEN_EQEQ);  
                break;
            case '&':
                token.start = &lexer->text[lexer->current];
                consume(lexer);
                match(lexer, &token, '&', TOKEN_AND);
                break;
            case '|':
                token.start = &lexer->text[lexer->current];
                consume(lexer);
                match(lexer, &token, '|', TOKEN_OR);
                break;
            case '!':
                token.start = &lexer->text[lexer->current];
                consume(lexer);
                this_or_that(lexer, &token, '=', TOKEN_NOT, TOKEN_NOTEQ);
                break;
            case '<':
                token.start = &lexer->text[lexer->current];
                consume(lexer);
                this_or_that(lexer, &token, '=', TOKEN_LESS, TOKEN_LESSEQ);
                break;
            case '>':
                token.start = &lexer->text[lexer->current];
                consume(lexer);
                this_or_that(lexer, &token, '=', TOKEN_GREATER, TOKEN_GREATEREQ);
                break;
            case 'a':
                token = match_identifier(lexer, "ans", 3, TOKEN_ANS);
                break;
            case 'e':
                token = match_identifier(lexer, "exit", 4, TOKEN_EXIT);
                break;
            case 'l':
                token = match_identifier(lexer, "let", 3, TOKEN_LET);
                break;
            case 't':
                token = match_identifier(lexer, "true", 4, TOKEN_TRUE);
                break;
            case 'f':
                token = match_identifier(lexer, "false", 5, TOKEN_FALSE);
                break;
            default:
                if (is_alpha(c)) {
                    token = match_identifier(lexer, "", 0, TOKEN_IDENTIFIER);
                }
                else {
                    token.type = TOKEN_ERROR;
                    token.start = &lexer->text[lexer->current];
                    token.len = strlen(token.start);
                }
                break;
        }
    }

    return token;
}

void tokenize(const char *text, TokenList *output)
{
    Lexer lexer = lexer_new(text); 
    Token token = scan_token(&lexer);

    while (token.type != TOKEN_END && token.type != TOKEN_ERROR) {
        list_push(output, token);
        token = scan_token(&lexer);
    }

    list_push(output, token);
}

void print_tokenlist(TokenList *list)
{
    static const char* token_names[TOKEN_COUNT] = {
        "NUM", 
        "STRING",
        "PLUS",
        "MINUS",
        "STAR",
        "SLASH",
        "CARET", 
        "LEFT_PAREN",
        "RIGHT_PAREN",
        "COMMA",
        "EQUAL",
        "DOLLAR",
        "ANS",
        "IDENTIFIER",
        "LET",
        "EXIT",
        "OR",
        "AND",
        "EQEQ",
        "NOT",
        "NOTEQ",
        "LESS",
        "LESSEQ",
        "GREATER",
        "GREATEREQ",
        "TRUE",
        "FALSE",
        "END",
        "ERROR",
    };

    for (size_t i = 0; i < list->count; i++) {
        Token token = list->items[i];
        printf("{\n");
        printf("  type: %s\n", token_names[token.type]);
        printf("  text: %.*s\n", token.len, token.start);
        printf("}\n");
    }
}
