#include "lexer.h"
#include "vector.h"
#include <stdio.h>
#include "ast.h"
#include "command.h"

int main(int argc, char const *argv[])
{
    if (argc < 1)
        exit(EXIT_FAILURE);
    const char *lex_token[] = {
        "TOKEN_STRING",
        "TOKEN_PIPE",
        "TOKEN_OR",
        "TOKEN_AND",
        "TOKEN_ASTERISK",
        "TOKEN_REDIRECT_INPUT",
        "TOKEN_REDIRECT_OUTPUT",
        "TOKEN_REDIRECT_OUTPUT_APPEND",
        "TOKEN_HERE_DOC",
        "TOKEN_DOLLAR",
        "TOKEN_SINGLE_QUOTE",
	    "TOKEN_DOUBLE_QUOTE",
        "TOKEN_END",
        "TOKEN_TYPE_UNKNOW",
    };
    // const char *str = "echo -n HI";
    const char *str = argv[1];
    t_vec v = lexer(str);
    size_t i = 0;
    while (i < v.len)
    {
        t_token t = *(t_token *)vec_get(&v, i);
        printf("Token: (type: %s) ", lex_token[t.type]);
        printf("Span : (begin: %li, end: %li)\n", t.span.begin, t.span.end);
        i++;
    }
    printf("%s\n", str);
    i = 0;
    t_token last = *(t_token *)vec_get(&v, v.len - 1);
    t_vec space = vec_new(sizeof(char), last.span.end + 1, NULL);
    size_t space_index = 0;
    while (space_index < space.capacity)
    {
        vec_append(&space, " ");
        space_index++;
    }
    vec_append(&v, "\0");
    while (i < v.len - 1)
    {
        t_token t = *(t_token *)vec_get(&v, i);
        char *c = vec_get(&space, t.span.begin);
        *c = '<';
        c = vec_get(&space, t.span.end);
        if (*c == '<')
            *c = '^';
        else
            *c = '>';
        i++;
    }
    vec_append(&space, "\0");
    printf("%s\n", (char *)space.data);
    t_ast *ast = ast_init(v.data, v.len);
    ast_run_command(ast, str);
    ast_free(ast);
    vec_free(&space);
    vec_free(&v);
    return 0;
}
