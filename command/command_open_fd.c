/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_open_fd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 14:22:45 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/20 14:25:51 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

//return 1 on success
int	preparing_fd_pipe(t_pidpes *pidpes, t_ast *ast)
{
	pidpes->pipes[READ_END] = STDIN_FILENO;
	pidpes->pipes[WRITE_END] = STDOUT_FILENO;
	if (pidpes->i < ast->pipeline.len - 1)
	{
		if (pipe(pidpes->pipes) == -1)
		{
			return (perror("Minishell: pipe: "), 1);
			pidpes->exit_status = 1;
			return (0);
		}
	}
	return (1);
}

void	ft_dup2close(int *fd, int fd_to_replace)
{
	if (dup2(*fd, fd_to_replace) == -1)
		perror("Minishell: dup2");
	if (close(*fd) == -1)
		perror("Minishell: close");
	*fd = -2;
}


void	ft_open_input(int *fdin, char *filename)
{
	*fdin = open(filename, O_RDONLY);
	if (*fdin >= 0)
		ft_dup2close(fdin, STDIN_FILENO);
}

void	ft_open_output(int *fdout, t_redirection redir)
{
	if (redir.type == REDIRECTION_OUTPUT)
		*fdout = open(redir.filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (redir.type == REDIRECTION_OUTPUT_APPEND)
		*fdout = open(redir.filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (*fdout >= 0)
		ft_dup2close(fdout, STDOUT_FILENO);
}

//return 0 on error 1 on success
int	ft_open_redirection(t_command *cmd)
{
	size_t			i;
	t_redirection	redir;

	i = 0;
	if (cmd->fdin != STDIN_FILENO)
		ft_dup2close(&cmd->fdin, STDIN_FILENO);
	if (cmd->fdout != STDOUT_FILENO)
		ft_dup2close(&cmd->fdout, STDOUT_FILENO);
	while (i < cmd->redirections_len)
	{
		redir = cmd->redirections[i];
		if (redir.type == REDIRECTION_INPUT)
			ft_open_input(&(cmd->fdin), redir.filename);
		else if (redir.type == REDIRECTION_OUTPUT || \
		redir.type == REDIRECTION_OUTPUT_APPEND)
			ft_open_output(&(cmd->fdout), redir);
		if (cmd->fdin == -1 || cmd->fdout == -1)
		{
			ft_putstr_fd("Minishell: ", STDERR_FILENO);
			return (perror(redir.filename), 0);
		}
		i++;
	}
	return (1);
}
