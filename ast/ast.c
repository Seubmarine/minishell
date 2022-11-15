/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 19:39:24 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/15 08:06:30 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

t_ast_command	ast_command_init(void)
{
	t_ast_command	command;

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
	size_t				i;
	t_ast_command		*command;
	t_ast_redirection	new_redirection;
	t_ast_command		cmd;

	i = 0;
	while (tok[i].type != TOKEN_END)
	{
		if (tok[i].type == TOKEN_REDIRECT_INPUT || tok[i].type == \
		TOKEN_REDIRECT_OUTPUT || tok[i].type == TOKEN_REDIRECT_OUTPUT_APPEND \
		|| tok[i].type == TOKEN_HERE_DOCUMENT)
		{
			new_redirection = redirection_init(tok[i], tok[i + 1]);
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
			cmd = ast_command_init();
			vec_append(&ast->pipeline, &cmd);
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
	t_ast			*ast;
	t_ast_command	command;

	(void) tok_size;
	ast = malloc(sizeof(*ast));
	ast->pipeline = vec_new(sizeof(t_ast_command), 2, NULL);
	command = ast_command_init();
	vec_append(&ast->pipeline, &command);
	ast_push(ast, tok);
	return (ast);
}

/*
open all the heredoc in a given ast_command and replace it's
return 1 on success 0 on error
*/
int	ast_command_open_heredoc(t_ast_command *command, t_env	*env, size_t *heredoc_counter)
{
	size_t	i;
	t_ast_redirection *redirections;

	if (command->redirection.len == 0)
		return (1);
	i = 0;
	while (i < command->redirection.len)
	{
		redirections = vec_get(&command->redirection, i);
		char	*heredoc_filename;
		heredoc_filename = heredoc_open_routine(env, *heredoc_counter, redirections->rhs.word);
		if (heredoc_filename == NULL)
			return (0);
		redirections->rhs.word = heredoc_filename;
		redirections->token.type = TOKEN_HERE_DOCUMENT_OPEN;
		*heredoc_counter += 1;
		i++;
	}
	return (1);
}

//return 1 one success 0 on error
int	ast_open_heredocs(t_ast *ast, t_env *env)
{
	size_t			i;
	t_ast_command	*commands;
	size_t			heredoc_counter;

	if (ast->pipeline.len == 0)
		return (0);
	i = 0;
	heredoc_counter = 0;
	commands = vec_get(&ast->pipeline, 0);
	while (i < ast->pipeline.len)
	{
		if (ast_command_open_heredoc(&commands[i], env, &heredoc_counter) == 0)
			return (0);
		i++;
	}
	return (1);
}

void	ast_command_close_heredoc(t_ast_command *cmd)
{
	size_t				i;
	t_ast_redirection	*redirection;

	i = 0;
	while (i < cmd->redirection.len)
	{
		redirection = vec_get(&cmd->redirection, i);
		if (redirection->token.type == TOKEN_HERE_DOCUMENT_OPEN)
		{
			unlink(redirection->rhs.word);
			free(redirection->rhs.word);
			redirection->rhs.word = NULL;
			redirection->token.type = TOKEN_HERE_DOCUMENT;
		}
		i++;
	}
}

void	ast_close_heredocs(t_ast *ast)
{
	t_ast_command	*commands;
	size_t			i;

	if (ast->pipeline.len == 0)
		return ;
	commands = vec_get(&ast->pipeline, 0);
	i = 0;
	while (i < ast->pipeline.len)
	{
		ast_command_close_heredoc(&commands[i]);
		i++;
	}
}