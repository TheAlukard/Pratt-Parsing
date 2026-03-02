#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include "lexer.h"
#include "parser.h"
#include "value.h"
#include "test.h"

void print_value(Value value)
{
    printf(">> ");
    switch (value.type) {
        case VALUE_NUM:
            printf("%0.25Lf\n", AS_NUM(value));
            break;
        case VALUE_STR:
            printf("%.*s\n", (int)AS_STR(value).len, AS_STR(value).data);
            break;
        case VALUE_BOOL:
            printf ("%s\n", AS_BOOL(value) ? "true" : "false");
            break;
    }
}

Value get_result(Parser *parser, TokenList *tl, char *buffer)
{
    list_clear(tl);
    if (tokenize(buffer, tl)) {
        // print_tokenlist(&tl);
        parser_reset(parser, tl);
        Value result = parse_expr(parser);
        if (parser->error) {
            const char *err = "ERROR: Parsing Failed!";
            String s = string_create(err, strlen(err));
            return VAL_STR(s);
        }
        else {
            return result;
        }
    }
    else {
        const char *err = "ERROR: Tokenization Failed!";
        String s = string_create(err, strlen(err));
        return VAL_STR(s);
    }
}

int main(void)
{
    enum { buffer_len = 1000 };
    char buffer[buffer_len]; 
    TokenList list = {0};
    Parser parser = parser_create();

    while (true) {
        printf(">> ");
        fgets(buffer, sizeof(char) * buffer_len, stdin);
        Value result = get_result(&parser, &list, buffer);
        print_value(result);
    }

    parser_destroy(&parser);
    list_free(&list);
}
