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
	TokenList list = {0};
    /* Parser parser = parser_create(); */
    
    while (true) {
        printf("> ");
        fgets(buffer, sizeof(char) * buffer_len, stdin);
		list_clear(&list);
        tokenize(buffer, &list);
		print_tokenlist(&list);
        /* parser_reset(&parser, &list); */
        /* printf("> %0.15lf\n", parse_expr(&parser)); */
    }

	list_free(&list);
}
