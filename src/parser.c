#include "parser.h"
#include "lexer.h"
#include <math.h> 
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

double unary(Parser *parser);
double binary(Parser *parser);
double number(Parser *parser);
double grouping(Parser *parser);
double ans(Parser *parser);
double identifier(Parser *parser);
double exit_prog(Parser *parser);
double declare(Parser *parser);
double get_var(Parser *parser);

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
    {get_var, NULL, PREC_NONE},
    {ans, NULL, PREC_NONE},
    {identifier, NULL, PREC_NONE},
    {declare, NULL, PREC_NONE},
    {exit_prog, NULL, PREC_NONE},
    {NULL, NULL, PREC_NONE},
    {NULL, NULL, PREC_NONE},
};

double exit_prog(Parser *parser)
{
    exit(0);
}

ParseRule* get_rule(Token token)
{
    return &rules[token.type];
}

Parser parser_create()
{
    Parser parser;
    parser.current = 0;
    parser.ans = 0;
    parser.map = map_new();
    
    return parser;
}

void parser_reset(Parser *parser, TokenList *list)
{
    parser->current = 0;
    parser->tokens = list;
}

Token prev(Parser *parser)
{
    return parser->tokens->items[parser->current - 1];
}

Token peek(Parser *parser)
{
    return parser->tokens->items[parser->current];
}

Token consume(Parser *parser)
{
    return parser->tokens->items[parser->current++];
}

Token expect(Parser *parser, TokenType expected)
{
    Token token = peek(parser);

    if (token.type == expected) {
        return consume(parser);
    }
    else {
        fprintf(stderr, "Error: Invalid Token '%.*s'", token.len, token.start);
        exit(1);
    }
}

double expression(Parser *parser, precedence rbp)
{
    Token token = consume(parser);
    double result = get_rule(token)->prefix(parser);

    while (rbp < get_rule(peek(parser))->lbp) {
        token = consume(parser);
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

double parse_expr(Parser *parser)
{
    double result = expression(parser, PREC_NONE);
    parser->ans = result;

    return result;
}

double ans(Parser *parser)
{
    return parser->ans;
}

double grouping(Parser *parser)
{
    double result = expression(parser, PREC_NONE);
    expect(parser, TOKEN_RIGHT_PAREN);

    return result;
}

double unary(Parser *parser)
{
    double result = expression(parser, PREC_UNARY);

    return -result;
}

double binary(Parser *parser)
{
    double result = expression(parser, get_rule(prev(parser))->lbp);

    return result;
}

bool expected_str(const char *str, const char *expected, int len)
{
    return memcmp(str, expected, sizeof(char) * len) == 0;
}

typedef enum {
    SIN,
    COS,
    TAN,
    ASIN,
    ACOS,
    ATAN,
    ATAN2,
    SINH,
    COSH,
    TANH,
    ASINH,
    ACOSH,
    ATANH,
    EXP,
    LOG,
    LOG10,
    LOG2,
    CEIL,
    FLOOR,
    ROUND,
    SQRT,
    PI,
} MathFunc;

double math_func(Parser *parser, MathFunc func)
{
    double r1, r2;

    switch (func) {
        case SIN:
            expect(parser, TOKEN_LEFT_PAREN);
            return sin(grouping(parser)); 
        case COS:
            expect(parser, TOKEN_LEFT_PAREN);
            return cos(grouping(parser));
        case TAN:
            expect(parser, TOKEN_LEFT_PAREN);
            return tan(grouping(parser));
        case ASIN:
            expect(parser, TOKEN_LEFT_PAREN);
            return asin(grouping(parser));
        case ACOS:
            expect(parser, TOKEN_LEFT_PAREN);
            return acos(grouping(parser));
        case ATAN:
            expect(parser, TOKEN_LEFT_PAREN);
            return atan(grouping(parser));
        case ATAN2:
            expect(parser, TOKEN_LEFT_PAREN);
            r1 = expression(parser, PREC_NONE);
            expect(parser, TOKEN_COMMA);
            r2 = grouping(parser);
            return atan2(r1, r2);
        case SINH:
            expect(parser, TOKEN_LEFT_PAREN);
            return sinh(grouping(parser));
        case COSH:
            expect(parser, TOKEN_LEFT_PAREN);
            return cosh(grouping(parser));
        case TANH:
            expect(parser, TOKEN_LEFT_PAREN);
            return tanh(grouping(parser));
        case ASINH:
            expect(parser, TOKEN_LEFT_PAREN);
            return asinh(grouping(parser));
        case ACOSH:
            expect(parser, TOKEN_LEFT_PAREN);
            return acosh(grouping(parser));
        case ATANH:
            expect(parser, TOKEN_LEFT_PAREN);
            return atanh(grouping(parser));
        case EXP:
            expect(parser, TOKEN_LEFT_PAREN);
            return exp(grouping(parser));
        case LOG:
            expect(parser, TOKEN_LEFT_PAREN);
            return log(grouping(parser));
        case LOG10:
            expect(parser, TOKEN_LEFT_PAREN);
            return log10(grouping(parser));
        case LOG2:
            expect(parser, TOKEN_LEFT_PAREN);
            return log2(grouping(parser));
        case CEIL:
            expect(parser, TOKEN_LEFT_PAREN);
            return ceil(grouping(parser));
        case FLOOR:
            expect(parser, TOKEN_LEFT_PAREN);
            return floor(grouping(parser));
        case ROUND:
            expect(parser, TOKEN_LEFT_PAREN);
            return round(grouping(parser));
        case SQRT:
            expect(parser, TOKEN_LEFT_PAREN);
            return sqrt(grouping(parser));
        case PI:
            return 3.14159265358979323846f;
    }
}

double identifier(Parser *parser)
{
    // math funcs 
    // sin
    // cos
    // tan
    //

    const char* funcs[] = {
        "sin",
        "cos",
        "tan",
        "asin",
        "acos",
        "atan",
        "atan2",
        "sinh",
        "cosh",
        "tanh",
        "asinh",
        "acosh",
        "atanh",
        "exp",
        "log",
        "log10",
        "log2",
        "ceil",
        "floor",
        "round",
        "sqrt",
        "pi",
    };

    Token ident = prev(parser);

    for (int i = 0; i < array_len(funcs); i++) {
        if (expected_str(ident.start, funcs[i], ident.len)) {
            return math_func(parser, (MathFunc)i);
        }
    }

    fprintf(stderr, "Error: Unkown identifier '%.*s'", ident.len, ident.start);
    exit(1);
}

double declare(Parser *parser)
{
    Token ident = expect(parser, TOKEN_IDENTIFIER); 
    expect(parser, TOKEN_EQUAL); 
    double result = expression(parser, PREC_NONE);

    if (! map_has(&parser->map, ident)) {
        char *name = (char*)malloc(sizeof(char) * ident.len);
        Token key = {
            .start = name,
            .len = ident.len,
            .type = ident.type,
        };
        memcpy(name, ident.start, sizeof(char) * ident.len);
        map_set(&parser->map, key, result);
    }
    else {
        map_set(&parser->map, ident, result);
    }

    return result;
}

double get_var(Parser *parser)
{
    Token ident = expect(parser, TOKEN_IDENTIFIER); 
    
    if (! map_has(&parser->map, ident)) {
        fprintf(stderr, "Error: Variable '%.*s' doesn't exist\n", ident.len, ident.start);
        for (int i = 0; i < parser->map.capacity; i++) {
            if (parser->map.items[i].valid) {
                printf("Key: %.*s, Value: %lf\n", parser->map.items[i].key.len, parser->map.items[i].key.start, parser->map.items[i].value);
            }
        }
        exit(1);
    }
    double result = map_get(&parser->map, ident);

    return result;
}

double number(Parser *parser)
{
    Token num = prev(parser);
    char temp[num.len + 1];
    memcpy(temp, num.start, num.len * sizeof(char));
    temp[num.len] = '\0';

    return strtod(temp, NULL); 
}
