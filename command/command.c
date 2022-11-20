/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 18:03:32 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/19 22:29:59 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

int	command_set_redirection(t_command *cmd, t_ast_command ast, \
t_ast_redirection *ast_redirec)
{
	size_t	i;

	i = 0;
	while (i < ast.redirection.len)
	{
		ast_redirec = vec_get(&ast.redirection, i);
		if (ast_redirec->rhs.type != TOKEN_STRING || \
		ast_redirec->rhs.word == NULL)
		{
			ft_putstr_fd("Minishell: redirection filename is NULL\n", \
			STDERR_FILENO);
			return (0);
		}
		cmd->redirections[i].filename = ast_redirec->rhs.word;
		cmd->redirections[i].type = \
		token_to_redirection_type(ast_redirec->token.type);
		if (cmd->redirections[i].type == REDIRECTION_INVALID)
		{
			ft_putstr_fd("Minishell: redirection type invalid\n", STDERR_FILENO);
			return (0);
		}
		i++;
	}
	return (1);
}

//return 1 on success 0 on error
int	command_redirection_init(t_command *cmd, t_ast_command ast)
{
	t_ast_redirection	*ast_redirec;

	ast_redirec = NULL;
	cmd->redirections = malloc(\
	sizeof(*cmd->redirections) * ast.redirection.len);
	if (cmd->redirections == NULL)
	{	
		ft_putstr_fd("Minishell: malloc error: command_redirection_init\n", \
		STDERR_FILENO);
		return (0);
	}
	cmd->redirections_len = ast.redirection.len;
	if (command_set_redirection(cmd, ast, ast_redirec) == 0)
		return (0);
	return (1);
}

int	command_init_value(t_command *cmd, t_ast_command ast_command)
{
	cmd->arguments = NULL;
	cmd->path = NULL;
	cmd->redirections = NULL;
	cmd->fdin = STDIN_FILENO;
	cmd->fdout = STDOUT_FILENO;
	cmd->arguments = malloc(sizeof(*cmd->arguments) * \
	(ast_command.args.len + 1));
	if (cmd->arguments == NULL)
	{
		ft_putstr_fd("Minishell : error malloc: cmd->arguments\n", 2);
		return (0);
	}
	return (1);
}

//transform an ast command to a command type
int	command_init(t_command *cmd, t_ast_command ast_command)
{
	size_t		i;

	i = 0;
	if (command_init_value(cmd, ast_command) == 0)
		return (0);
	while (i < ast_command.args.len)
	{
		cmd->arguments[i] = ((t_token *)vec_get(&ast_command.args, i))->word;
		i++;
	}
	cmd->arguments[i] = NULL;
	if (!command_redirection_init(cmd, ast_command))
		return (0);
	cmd->path = NULL;
	if (cmd->arguments[0] != NULL)
	{
		cmd->path = ft_strdup(cmd->arguments[0]);
		if (cmd->path == NULL)
			return (0);
	}
	return (1);
}

void	command_free(t_command *command)
{
	free(command->redirections);
	free(command->arguments);
	free(command->path);
	command->redirections_len = 0;
	command->fdin = -1;
	command->fdout = -1;
}
