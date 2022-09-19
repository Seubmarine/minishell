#include "lexer.h"
#include "vector.h"
#include <stdio.h>
#include "ast.h"
#include "command.h"

void	lexer_debug_print(const char *original_line, t_vec v)
{
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

	size_t i = 0;
    while (i < v.len)
    {
        t_token t = *(t_token *)vec_get(&v, i);
        printf("Token: (type: %s) ", lex_token[t.type]);
        printf("Span : (begin: %li, end: %li)\n", t.span.begin, t.span.end);
        i++;
    }
    printf("%s\n", original_line);
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
    vec_free(&space);
}

void execute_line(char *line)
{
    const char *str = line;
    t_vec v = lexer(str);
    lexer_debug_print(str, v);
	t_ast *ast = ast_init(v.data, v.len);
    ast_run_command(ast, str);
    ast_free(ast);
    vec_free(&v);
}

#include <readline/readline.h>
#include <readline/history.h>
int main(int argc, char const *argv[], char const *envp[])
{
	(void) argc;
	(void) argv;
	(void) envp;
    char *line;
    int is_running = 1;
	
	while (is_running)
	{
		line = readline("Minishell$ ");
		add_history(line);
        execute_line(line);
		if (strncmp("exit", line, 4) == 0)
            is_running = 0;
        free(line);
	}
    clear_history();
	return (0);
}