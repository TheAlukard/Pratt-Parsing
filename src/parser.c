#include "parser.h"
#include <math.h> 
#include <string.h>

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
