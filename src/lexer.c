#include "lexer.h"
#include "string.h"
#include <ctype.h>

Lexer lexer_new(const char *text)
{
    Lexer lexer = {
        .text = text,
        .current = 0,
    };

    return lexer;
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
