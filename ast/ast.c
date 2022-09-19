/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 19:39:24 by tbousque          #+#    #+#             */
/*   Updated: 2022/09/18 22:22:12 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

t_ast_command ast_command_init(void)
{
	t_ast_command command;

	command.args = vec_new(sizeof(t_token), 2, NULL);
	command.redirection = vec_new(sizeof(t_ast_redirection), 2, NULL);
	return (command);
}

t_ast_redirection	redirection_init(t_token type, t_token rhs)
{
	t_ast_redirection	redirection;

	redirection.token = type;
	redirection.rhs = rhs;
	return (redirection);
}

void	ast_push(t_ast *ast, t_token *tok)
{
	size_t	i;
	t_ast_command *command;

	i = 0;
	while (tok[i].type != TOKEN_END)
	{
		if (tok[i].type == TOKEN_REDIRECT_INPUT || tok[i].type == TOKEN_REDIRECT_OUTPUT)
		{
			t_ast_redirection new_redirection = redirection_init(tok[i], tok[i + 1]);
			command = vec_get(&ast->pipeline, ast->pipeline.len - 1);
			vec_append(&command->redirection, &new_redirection);
			i += 1;
		}
		else if (tok[i].type == TOKEN_STRING)
		{
			command = vec_get(&ast->pipeline, ast->pipeline.len - 1);
			vec_append(&command->args, &tok[i]);
		}
		else if (tok[i].type == TOKEN_PIPE)
		{
			t_ast_command command = ast_command_init();
			vec_append(&ast->pipeline, &command);
		}
		i++;
	}
}

void	ast_command_free(t_ast_command *command)
{
	vec_free(&command->args);
	vec_free(&command->redirection);
}

void	ast_free(t_ast *ast)
{
	size_t	i;

	i = 0;
	while (i < ast->pipeline.len)
	{
		t_ast_command *command = vec_get(&ast->pipeline, i);
		ast_command_free(command);
		i++;
	}
	vec_free(&ast->pipeline);
	free(ast);
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
t_ast	*ast_init(t_token *tok, size_t tok_size)
{
	(void) tok_size;
	t_ast *ast = malloc(sizeof(*ast));
	ast->pipeline = vec_new(sizeof(t_ast_command), 2, NULL);
	t_ast_command command = ast_command_init();
	vec_append(&ast->pipeline, &command);
	ast_push(ast, tok);
	return (ast);
}
