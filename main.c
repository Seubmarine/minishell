#include "lexer.h"
#include "vector.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    const char *lex_token[] = {
        "TOKEN_TYPE_STRING",
	    "TOKEN_TYPE_PIPE",
	    "TOKEN_TYPE_OR",
	    "TOKEN_TYPE_ASTERISK",
	    "TOKEN_TYPE_END",
	    "TOKEN_TYPE_UNKNOW"
    };
    const char *str = " Hello WOrld *|*||";
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
    printf("%s\n", (char *)space.data);
    vec_free(&space);
    vec_free(&v);
    return 0;
}
