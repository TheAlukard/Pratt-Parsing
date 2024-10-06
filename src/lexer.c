#include "lexer.h"
#include "string.h"
#include <ctype.h>
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

static void trim_left(Lexer *lexer)
{
    while (isspace(peek(lexer))) {
        consume(lexer);
    }
}

static Token expected(Lexer *lexer, const char *text, int len, TokenType type)
{
    int start = lexer->current;

    Token token;

    while (isalnum(peek(lexer))) {
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

static Token match(Lexer *lexer, const char *text, int len, TokenType type)
{
    int start = lexer->current;

    Token token;

    while (!isspace(peek(lexer))) {
        consume(lexer);
    }

    if (lexer->current - start == len) {
        if (memcmp(&lexer->text[start], text, len) == 0) {
            token.type = type;
        }
    }
    else {
		fprintf(stderr, "Unexpected Token: %.*s\n", token.len, token.start);
		exit(1);
    }

    token.start = &lexer->text[start];
    token.len = lexer->current - start;

    return token;
}

static Token this_or_that(Lexer *lexer, const char *first, TokenType first_type, const char *second, TokenType second_type)
{
	int start = lexer->current;

	Token token;

	while (!isspace(peek(lexer))) {
		consume(lexer);
	}

	int first_len = strlen(first);
	int second_len = strlen(second);

	if (lexer->current - start == first_len) {
		if (memcmp(&lexer->text[start], first, first_len) == 0) {
			token.type = first_type;
		}
	}
	else if (lexer->current - start == second_len) {
		if (memcmp(&lexer->text[start], second, second_len) == 0) {
			token.type = second_type;
		}
	}
	else {
		fprintf(stderr, "Unexpected Token: %.*s\n", token.len, token.start);
		exit(1);
	}

	token.start = &lexer->text[start];
	token.len = lexer->current - start;

	return token;
}

static Token parse_string_literal(Lexer *lexer)
{
	char quote = consume(lexer);

	Token token;

	token.start = lexer->text;
	
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

    if (isdigit(c)) {
        token.type = TOKEN_NUM;
        token.start = &lexer->text[lexer->current];
        while (isdigit(peek(lexer))) {
            consume(lexer);
        }
        if (peek(lexer) == '.') {
            consume(lexer);
            while (isdigit(peek(lexer))) {
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
            case '=':
				token = this_or_that(lexer, "=", TOKEN_EQUAL, "==", TOKEN_EQEQ);	
				break;
			case '&':
				token = match(lexer, "&&", 2, TOKEN_AND);
				break;
			case '|':
				token = match(lexer, "||", 2, TOKEN_OR);
				break;
			case '!':
				token = this_or_that(lexer, "!", TOKEN_NOT, "!=", TOKEN_NOTEQ);
				break;
			case '<':
				token = this_or_that(lexer, "<", TOKEN_LESS, "<=", TOKEN_LESSEQ);
				break;
			case '>':
				token = this_or_that(lexer, ">", TOKEN_GREATER, ">=", TOKEN_GREATEREQ);
				break;
            case 'a':
                token = expected(lexer, "ans", 3, TOKEN_ANS);
                break;
            case 'e':
                token = expected(lexer, "exit", 4, TOKEN_EXIT);
                break;
            case 'l':
                token = expected(lexer, "let", 3, TOKEN_LET);
                break;
			case 't':
				token = expected(lexer, "true", 4, TOKEN_TRUE);
				break;
			case 'f':
				token = expected(lexer, "false", 5, TOKEN_FALSE);
				break;
            default:
                if (isalpha(c)) {
                    token = expected(lexer, "", 0, TOKEN_IDENTIFIER);
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
