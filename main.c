#include "lexer.h"
#include "vector.h"
#include <stdio.h>
#include "ast.h"
#include "command.h"
#include "environement_variable.h"
#include "signaling.h"
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>

// void	lexer_debug_print(const char *original_line, t_vec v)
// {
// 	const char *lex_token[] = {
//	     "TOKEN_STRING",
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

//return exit status
int	execute_line(char *line, t_env *env)
{
	t_vec	v;
	t_ast	*ast;
	int		exit_status;

	ast = NULL;
	exit_status = 0;
	v = lexer(line, *env);
	if (v.len <= 1 || (((t_token *)v.data)[v.len - 1].type) != TOKEN_END)
	{
		vec_free(&v);
		return (exit_status);
	}
	ast = ast_init(v.data, v.len);
	if (ast_open_heredocs(ast, env) == 1)
		exit_status = ft_which_command(ast, env);
	else
		exit_status = 130;
	ast_close_heredocs(ast);
	ast_free(ast);
	vec_free(&v);
	return (exit_status);
}

void	remove_echo_ctrl(void)
{
	struct termios	state;

	if (!isatty(STDIN_FILENO))
	{
		perror("Not a tty");
		return ;
	}
	tcgetattr(STDIN_FILENO, &state);
	state.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &state);
}

t_env	ft_prepare_env(const char **envp, char *argv)
{
	t_env	env;

	if (*envp == NULL)
		env = env_init_null(argv);
	else
		env = env_init_from_envp(envp, argv);
	return (env);
}

int	prompt(t_env *env)
{
	char	*line;
	int		is_running;
	int		exit_status;

	exit_status = 0;
	is_running = 1;
	while (is_running)
	{
		line = readline("Minishell$ ");
		if (line == NULL)
			is_running = 0;
		else
		{
			add_history(line);
			exit_status = execute_line(line, env);
			if (env->is_child)
				is_running = 0;
		}
		free(line);
	}
	return (exit_status);
}

int	main(int argc, char const *argv[], char const *envp[])
{
	t_env	env;
	int		exit_status;

	(void) argc;
	exit_status = 0;
	remove_echo_ctrl();
	signal_handling();
	env = ft_prepare_env(envp, (char *)argv[0]);
	exit_status = prompt(&env);
	clear_history();
	env_free(&env);
	return (exit_status);
}
