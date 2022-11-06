#include "lexer.h"
#include "vector.h"
#include <stdio.h>
#include "ast.h"
#include "command.h"
#include "environement_variable.h"

// void	lexer_debug_print(const char *original_line, t_vec v)
// {
// 	const char *lex_token[] = {
//         "TOKEN_STRING",
//         "TOKEN_PIPE",
//         "TOKEN_OR",
//         "TOKEN_AND",
//         "TOKEN_ASTERISK",
//         "TOKEN_REDIRECT_INPUT",
//         "TOKEN_REDIRECT_OUTPUT",
//         "TOKEN_REDIRECT_OUTPUT_APPEND",
//         "TOKEN_HERE_DOC",
//         "TOKEN_DOLLAR",
//         "TOKEN_SINGLE_QUOTE",
// 	    "TOKEN_DOUBLE_QUOTE",
//         "TOKEN_END",
//         "TOKEN_SPACE"
//         "TOKEN_TYPE_UNKNOW",
//     };

// 	size_t i = 0;
//     while (i < v.len)
//     {
//         t_token t = *(t_token *)vec_get(&v, i);
//         printf("Token: (type: %s) ", lex_token[t.type]);
//         printf("Span : (begin: %li, end: %li)\n", t.span.begin, t.span.end);
//         i++;
//     }
//     printf("%s\n", original_line);
//     i = 0;
//     t_token last = *(t_token *)vec_get(&v, v.len - 1);
//     t_vec space = vec_new(sizeof(char), last.span.end + 1, NULL);
//     size_t space_index = 0;
//     while (space_index < space.capacity)
//     {
//         vec_append(&space, " ");
//         space_index++;
//     }
//     vec_append(&v, "\0");
//     while (i < v.len - 1)
//     {
//         t_token t = *(t_token *)vec_get(&v, i);
//         char *c = vec_get(&space, t.span.begin);
//         *c = '<';
//         c = vec_get(&space, t.span.end);
//         if (*c == '<')
//             *c = '^';
//         else
//             *c = '>';
//         i++;
//     }
//     vec_append(&space, "\0");
//     printf("%s\n", (char *)space.data);
//     vec_free(&space);
// }

void execute_line(char *line, t_env *env)
{
    t_vec v = lexer(line, *env);
	t_ast *ast = ast_init(v.data, v.len);
    ast_run_command(ast, env);
    ast_free(ast);
    vec_free(&v);
}

#include <termios.h>
void remove_echo_ctrl(void)
{
    struct termios state;
    if(!isatty(STDIN_FILENO))
    {
        perror("Not a tty");
        return ;
    }
    tcgetattr(STDIN_FILENO, &state);
    state.c_lflag ^= ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &state);
}

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
int main(int argc, char const *argv[], char const *envp[])
{
	(void) argc;
    char *line;
    int is_running = 1;
    t_env env;

    remove_echo_ctrl();
    if (*envp == NULL)
    {
       env = env_init_null((char *)argv[0]);
    }
    else
    {
        (void) argv;
        env = env_init_from_envp(envp);
    }
    while (is_running)
	{
		line = readline("Minishell$ ");
		add_history(line);
		if (strncmp("__end", line, 5) == 0)
            is_running = 0;
        else
            execute_line(line, &env);
        free(line);
	}
    clear_history();
    env_free(&env);
	return (0);
}