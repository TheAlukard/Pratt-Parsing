#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include "lexer.h"
#include "parser.h"
#include "value.h"

void print_value(Value value)
{
    switch (value.type) {
        case VALUE_NUM:
            printf("> %0.25Lf\n", AS_NUM(value));
            break;
        case VALUE_STR:
            printf("> %.*s\n", (int)AS_STR(value).len, AS_STR(value).data);
            break;
        case VALUE_BOOL:
            printf ("> %s\n", AS_BOOL(value) ? "true" : "false");
            break;
    }
}

int main(void)
{
    #define buffer_len 1000
    char buffer[buffer_len]; 
    TokenList list = {0};
    Parser parser = parser_create();
    
    while (true) {
        printf(">> ");
        fgets(buffer, sizeof(char) * buffer_len, stdin);
        list_clear(&list);
        if (tokenize(buffer, &list)) {
            // print_tokenlist(&list);
            parser_reset(&parser, &list);
            Value result = parse_expr(&parser);
            if (parser.error) {
                fprintf(stderr, "ERROR: Parsing Failed!\n");
            }
            else {
                print_value(result);
            }
        }
    }

    parser_destroy(&parser);
    list_free(&list);
}
