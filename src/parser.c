#include "parser.h"
#include "lexer.h"
#include "value.h"
#include <math.h> 
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

Value unary(Parser *parser);
Value binary(Parser *parser);
Value number(Parser *parser);
Value grouping(Parser *parser);
Value ans(Parser *parser);
Value identifier(Parser *parser);
Value exit_prog(Parser *parser);
Value declare(Parser *parser);
Value get_var(Parser *parser);
Value string(Parser *parser);
Value boolean(Parser *parser);

ParseRule rules[TOKEN_COUNT] = {
    {number, NULL, PREC_NONE},
    {string, NULL, PREC_NONE},
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
    {NULL, binary, PREC_OR},
    {NULL, binary, PREC_AND},
    {NULL, binary, PREC_EQUALITY},
    {unary, NULL, PREC_UNARY},
    {NULL, binary, PREC_EQUALITY},
    {NULL, binary, PREC_COMP},
    {NULL, binary, PREC_COMP},
    {NULL, binary, PREC_COMP},
    {NULL, binary, PREC_COMP},
    {boolean, NULL, PREC_NONE},
    {boolean, NULL, PREC_NONE},
    {NULL, NULL, PREC_NONE},
    {NULL, NULL, PREC_NONE},
};

Value exit_prog(Parser *parser)
{
    (void)(parser);
    exit(parser->error);
}

ParseRule* get_rule(Token token)
{
    return &rules[token.type];
}

Parser parser_create()
{
    Parser parser;
    parser.current = 0;
    parser.error = false;
    parser.ans = VAL_NUM(0);
    parser.map = map_new();
    parser.arena = arena_init(1024);
    
    return parser;
}

void parser_reset(Parser *parser, TokenList *list)
{
    parser->error = false;
    parser->current = 0;
    parser->tokens = list;
}

void parser_destroy(Parser *parser)
{
    parser->current = 0;
    arena_deinit(&parser->arena);
    map_delete(&parser->map);
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
        fprintf(stderr, "Error: Invalid Token '%.*s'\n", token.len, token.start);
        parser->error = true;
        return (Token){.type = TOKEN_ERROR};
    }
}

Value do_operation(Parser *parser, Value left, Value right, Token oper)
{
    if (left.type != right.type) {
        char buffer1[100];
        value_to_str(buffer1, &left);
        char buffer2[100];
        value_to_str(buffer2, &right);

        fprintf(stderr, "Error: Value: %s of type: %s has a different type than Value: %s of type: %s\n",
                buffer1, value_type_to_str(left.type), buffer2, value_type_to_str(right.type));

        parser->error = true;
    }

    Value result;

    #define invalid_op()\
        do { \
            fprintf(stderr, "Error: Can't perform this operation: %.*s on a value of this type: %s\n",\
                    oper.len, oper.start, value_type_to_str(left.type));\
            parser->error = true;\
        } while (0)

    switch (oper.type) {
        case TOKEN_PLUS: 
            switch (left.type) {
                case VALUE_BOOL: invalid_op(); break;
                case VALUE_NUM: result = VAL_NUM(AS_NUM(left) + AS_NUM(right)); break;
                case VALUE_STR: result = VAL_STR(string_add(&parser->arena, &AS_STR(left), &AS_STR(right))); break;
            }
            break;
        case TOKEN_MINUS: 
            if (left.type != VALUE_NUM) invalid_op();
            else result = VAL_NUM(AS_NUM(left) - AS_NUM(right));
            break;
        case TOKEN_STAR:  
            if (left.type != VALUE_NUM) invalid_op();
            else result = VAL_NUM(AS_NUM(left) * AS_NUM(right));
            break;
        case TOKEN_SLASH: 
            if (left.type != VALUE_NUM) invalid_op();
            else result = VAL_NUM(AS_NUM(left) / AS_NUM(right));
            break;
        case TOKEN_CARET: 
            if (left.type != VALUE_NUM) invalid_op();
            else result = VAL_NUM(pow(AS_NUM(left), AS_NUM(right)));
            break;
        case TOKEN_EQEQ:
            switch (left.type) {
                case VALUE_NUM:  result = VAL_BOOL(AS_NUM(left) == AS_NUM(right)); break;
                case VALUE_STR:  result = VAL_BOOL(string_compare(&AS_STR(left), &AS_STR(right))); break;
                case VALUE_BOOL: result = VAL_BOOL(AS_BOOL(left) == AS_BOOL(right)); break;
            }
            break;
        case TOKEN_NOTEQ:
            switch (left.type) {
                case VALUE_NUM:  result = VAL_BOOL(AS_NUM(left) != AS_NUM(right)); break;
                case VALUE_STR:  result = VAL_BOOL(!string_compare(&AS_STR(left), &AS_STR(right))); break;
                case VALUE_BOOL: result = VAL_BOOL(AS_BOOL(left) != AS_BOOL(right)); break;
            }
            break;
        case TOKEN_LESS: 
            if (left.type != VALUE_NUM) invalid_op();
            else result = VAL_BOOL(AS_NUM(left) < AS_NUM(right));
            break;
        case TOKEN_LESSEQ:
            if (left.type != VALUE_NUM) invalid_op();
            else result = VAL_BOOL(AS_NUM(left) <= AS_NUM(right));
            break;
        case TOKEN_GREATER:
            if (left.type != VALUE_NUM) invalid_op();
            else result = VAL_BOOL(AS_NUM(left) > AS_NUM(right));
            break;
        case TOKEN_GREATEREQ:
            if (left.type != VALUE_NUM) invalid_op();
            else result = VAL_BOOL(AS_NUM(left) >= AS_NUM(right));
            break;
        case TOKEN_OR: 
            if (left.type != VALUE_BOOL) invalid_op();
            else result = VAL_BOOL(AS_BOOL(left) || AS_BOOL(right));
            break;
        case TOKEN_AND: 
            if (left.type != VALUE_BOOL) invalid_op();
            else result = VAL_BOOL(AS_BOOL(left) && AS_BOOL(right));
            break;
        default: break; // Unreachable
    }

    #undef invalid_op

    return result;
}

Value expression(Parser *parser, precedence rbp)
{
    if (parser->error) return VAL_NUM(0.0);
    Token token = consume(parser);
    Value left = get_rule(token)->prefix(parser);

    while ((int)rbp < get_rule(peek(parser))->lbp) {
        if (parser->error) return VAL_NUM(0.0);
        token = consume(parser);
        Value right = get_rule(token)->infix(parser); 
        left = do_operation(parser, left, right, token);
    }

    return left;
}

Value parse_expr(Parser *parser)
{
    if (parser->tokens->count <= 0 ||
        parser->tokens->items[0].type == TOKEN_END ||
        parser->tokens->items[0].type == TOKEN_ERROR) {
        parser->error = true;
        return VAL_NUM(0.0);
    }
    Value result = expression(parser, PREC_NONE);
    parser->ans = result;

    return result;
}

Value ans(Parser *parser)
{
    return parser->ans;
}

Value grouping(Parser *parser)
{
    Value result = expression(parser, PREC_NONE);
    expect(parser, TOKEN_RIGHT_PAREN);

    return result;
}

Value unary(Parser *parser)
{
    Token token = prev(parser);
    Value result = expression(parser, PREC_UNARY);

    switch (token.type) {
        case TOKEN_NOT:   result = VAL_BOOL(!AS_BOOL(result));   break;
        case TOKEN_MINUS: result = VAL_NUM(AS_NUM(result) * -1); break;
        default: break;
    }

    return result;
}

Value binary(Parser *parser)
{
    Value result = expression(parser, get_rule(prev(parser))->lbp);

    return result;
}

bool expected_str(const char *str, const char *expected, size_t len)
{
    return strlen(expected) != len ? false : memcmp(str, expected, sizeof(char) * len) == 0;
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
    EULER,
    MATHFUNC_COUNT,
} MathFunc;

Value math_func(Parser *parser, MathFunc func)
{
    Value r1, r2;

    switch (func) {
        case SIN:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(sinl(AS_NUM(grouping(parser)))); 
        case COS:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(cosl(AS_NUM(grouping(parser))));
        case TAN:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(tanl(AS_NUM(grouping(parser))));
        case ASIN:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(asinl(AS_NUM(grouping(parser))));
        case ACOS:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(acosl(AS_NUM(grouping(parser))));
        case ATAN:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(atanl(AS_NUM(grouping(parser))));
        case ATAN2:
            expect(parser, TOKEN_LEFT_PAREN);
            r1 = expression(parser, PREC_NONE);
            expect(parser, TOKEN_COMMA);
            r2 = grouping(parser);
            return VAL_NUM(atan2l(AS_NUM(r1), AS_NUM(r2)));
        case SINH:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(sinhl(AS_NUM(grouping(parser))));
        case COSH:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(coshl(AS_NUM(grouping(parser))));
        case TANH:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(tanhl(AS_NUM(grouping(parser))));
        case ASINH:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(asinhl(AS_NUM(grouping(parser))));
        case ACOSH:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(acoshl(AS_NUM(grouping(parser))));
        case ATANH:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(atanhl(AS_NUM(grouping(parser))));
        case EXP:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(expl(AS_NUM(grouping(parser))));
        case LOG:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(logl(AS_NUM(grouping(parser))));
        case LOG10:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(log10l(AS_NUM(grouping(parser))));
        case LOG2:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(log2l(AS_NUM(grouping(parser))));
        case CEIL:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(ceill(AS_NUM(grouping(parser))));
        case FLOOR:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(floorl(AS_NUM(grouping(parser))));
        case ROUND:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(roundl(AS_NUM(grouping(parser))));
        case SQRT:
            expect(parser, TOKEN_LEFT_PAREN);
            return VAL_NUM(sqrtl(AS_NUM(grouping(parser))));
        case PI:
            return VAL_NUM(3.14159265358979323846264338327950288419716939937510f);
        case EULER:
            return VAL_NUM(2.71828182845904523536028747135266249775724709369995f);
        default:
            return VAL_NUM(0.0f); // unreachable
    }
}

Value identifier(Parser *parser)
{
    // math funcs 
    // sin
    // cos
    // tan
    //

    static const char* funcs[MATHFUNC_COUNT] = {
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
        "e",
    };

    Token ident = prev(parser);

    for (size_t i = 0; i < array_len(funcs); i++) {
        if (expected_str(ident.start, funcs[i], ident.len)) {
            return math_func(parser, (MathFunc)i);
        }
    }

    fprintf(stderr, "Error: Unkown identifier '%.*s'\n", ident.len, ident.start);
    parser->error = true;

    return VAL_BOOL(false);
}

Value declare(Parser *parser)
{
    Token ident = consume(parser); 
    expect(parser, TOKEN_EQUAL); 
    Value result = expression(parser, PREC_NONE);

    if (result.type == VALUE_STR) {
        result = VAL_STR(string_create(AS_STR(result).data, AS_STR(result).len));
    }

    if (!map_has(&parser->map, ident)) {
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
        Value val = map_get(&parser->map, ident);
        if (val.type == VALUE_STR) {
            string_destroy(&AS_STR(val));
        }
        map_set(&parser->map, ident, result);
    }

    return result;
}

Value get_var(Parser *parser)
{
    Token ident = expect(parser, TOKEN_IDENTIFIER);
    
    if (!map_has(&parser->map, ident)) {
        fprintf(stderr, "Error: Variable '%.*s' doesn't exist\n", ident.len, ident.start);
        parser->error = true;
        for (size_t i = 0; i < parser->map.capacity; i++) {
            if (parser->map.items[i].valid) {
                Token key = parser->map.items[i].key;
                Value value = parser->map.items[i].value;
                static char buffer[100];
                value_to_str(buffer, &value);
                printf("Key: %.*s, Value: %s\n", key.len, key.start, buffer);
            }
        }
        parser->error = true;
    }
    Value result = map_get(&parser->map, ident);

    return result;
}

Value number(Parser *parser)
{
    Token num = prev(parser);
    static char temp[100];
    sprintf(temp, "%.*s", num.len, num.start);
    Value result = VAL_NUM(strtold(temp, NULL));

    return result;
}

Value string(Parser *parser)
{
    Token token = prev(parser);
    String str = string_create_arena(&parser->arena, token.start, token.len);

    return VAL_STR(str);
}

Value boolean(Parser *parser)
{
    Token token = prev(parser);

    return token.type == TOKEN_TRUE ? VAL_BOOL(true) : VAL_BOOL(false);
}
