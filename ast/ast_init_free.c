#include "ast.h"

void	ast_command_free(t_ast_command *command)
{
	vec_free(&command->args);
	vec_free(&command->redirection);
}

void	ast_free(t_ast *ast)
{
	size_t			i;
	t_ast_command	*command;

	i = 0;
	while (i < ast->pipeline.len)
	{
		command = vec_get(&ast->pipeline, i);
		ast_command_free(command);
		i++;
	}
	vec_free(&ast->pipeline);
}
/*
Create an array of token by placing them in a command and their redirection
for example : "echo hello world | wc > out"
		command[0].args = tok[0] tok[1] tok[3]
		detect a pipe so create a new command
		command[1].args = tok[4]
		detect a tok of redirection so create a redirection
		command[1].redirection = [.type = tok[5], .name = tok[6]]
*/

int		ast_init(t_token *tok, size_t tok_size, t_ast *ast)
{
	t_ast_command	command;

	(void) tok_size;
	ast->pipeline = vec_new(sizeof(t_ast_command), 2, NULL);
	if (ast->pipeline.data == NULL)
	{
		ft_putstr_fd("Minishell: vec_new: malloc error\n", 2);
		return (0);
	}
	command = ast_command_init();
	if (command.args.data == NULL || command.redirection.data == NULL)
	{
		ft_putstr_fd("Minishell: ast_command_init: malloc error\n", 2);
		return (0);
	}
	if (vec_append(&ast->pipeline, &command) == 0)
		return (0);
	if (ast_push(ast, tok) == 0)
		return (0);
	return (1);
}
