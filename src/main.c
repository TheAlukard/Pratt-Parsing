#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "list.h"
#include "lexer.h"
#include "parser.h"

int main(void)
{
    const size_t buffer_len = 1000;
    char buffer[buffer_len]; 
    Parser parser = {0};
    
    while (true) {
        printf("> ");
        fgets(buffer, sizeof(char) * buffer_len, stdin);
        TokenList list = tokenize(buffer);
        parser_new(&parser, &list);
        printf("> %0.15lf\n", parse_expr(&parser));
        list_free(&list);
    }
}
