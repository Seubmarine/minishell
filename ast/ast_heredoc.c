/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 19:44:37 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/21 19:44:38 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

/*
open all the heredoc in a given ast_command and replace it's
return 1 on success 0 on error
*/
int	ast_command_open_heredoc(t_ast_command *command, t_env	*env, \
size_t *heredoc_counter)
{
	size_t				i;
	t_ast_redirection	*redirections;
	char				*heredoc_filename;

	if (command->redirection.len == 0)
		return (1);
	i = 0;
	while (i < command->redirection.len)
	{
		redirections = vec_get(&command->redirection, i);
		if (redirections->token.type == TOKEN_HERE_DOCUMENT)
		{
			heredoc_filename = heredoc_open_routine(env, *heredoc_counter, \
			redirections->rhs.word);
			if (heredoc_filename == NULL)
				return (0);
			redirections->rhs.word = heredoc_filename;
			redirections->token.type = TOKEN_HERE_DOCUMENT_OPEN;
			*heredoc_counter += 1;
		}
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
