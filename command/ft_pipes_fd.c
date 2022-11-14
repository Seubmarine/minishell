/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipes_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 11:58:17 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/14 01:12:24 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

int	ft_open_close_pipes(t_pidpes *pidpes)
{
	if ((pidpes->i - 2) >= 0)
	{
		close(pidpes->pipes[pidpes->i % 2][0]);
		close(pidpes->pipes[pidpes->i % 2][1]);
	}
	pipe(pidpes->pipes[pidpes->i % 2]);
	// if (pipe(pidpes->pipes[pidpes->i % 2]) == -1)
	// 	return (ft_error_pipes(pidpes));
	return (0);
}

void	ft_close_pipes(t_pidpes *pidpes)
{
	if (pidpes->pipes[pidpes->i % 2][0] <= -1)
		close(pidpes->pipes[pidpes->i % 2][0]);
	if (pidpes->pipes[pidpes->i % 2][1] <= -1)
		close(pidpes->pipes[pidpes->i % 2][1]);
}

void	ft_close_correct_pipes(t_pidpes *pidpes)
{
	if ((pidpes->i % 2) == 0)
	{
		close(pidpes->pipes[0][0]);
		close(pidpes->pipes[1][1]);
	}
	else
	{
		close(pidpes->pipes[0][1]);
		close(pidpes->pipes[1][0]);
	}
}

void	ft_close_correct_pipes_last(t_pidpes *pidpes)
{
	if ((pidpes->i % 2) == 0)
	{
		close(pidpes->pipes[1][1]);
		pidpes->pipes[1][1] = -2;
	}
	else
	{
		close(pidpes->pipes[0][1]);
		pidpes->pipes[0][1] = -2;
	}
}

int	ft_open_input_pipe(int *fd, int i, t_ast *ast, t_env *env)
{
	*fd = open(/*arg*/, O_RDONLY);
}

int	ft_open_output_pipes(int *fd, char *filename, enum e_redirection_type redirection)
{
	if (redirection == REDIRECTION_OUTPUT)
		*fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (redirection == REDIRECTION_OUTPUT_APPEND)
		*fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		perror("Minishell: open_output_pipes given invalid redirection type");
}

int	ft_analyse_file(t_pidpes *pidpes, t_command *cmd)
{
	int	i;
	t_redirection current;

	i = 0;
	while (i < cmd->redirections_len)
	{
		current = cmd->redirections[i];
		if (current.type == REDIRECTION_INPUT)
		{
			if ((pidpes->i % 2) == 0)
				ft_open_input_pipes(pidpes->pipes[1][0], current.filename, current.type);
			else
				ft_open_input_pipes(pidpes->pipes[0][0], current.type);
		}
		else if (current.type == REDIRECTION_OUTPUT || current.type == REDIRECTION_OUTPUT_APPEND)
		{
			if ((pidpes->i % 2) == 0)
				ft_open_output_pipes(pidpes->pipes[0][1], current.filename, current.type);
			else
				ft_open_output_pipes(pidpes->pipes[1][1], current.filename, current.type);
		}
		i++;
	}
}

int	ft_open_fds_last(int *c_fd, t_pidpes *pidpes, t_ast *ast, t_env *env)
{
	ft_close_correct_pipes_last(pidpes);
	ft_analyse_file(pidpes, ast, env);
	if ((pidpes->i % 2) == 0)
	{
		if (pidpes->pipes[1][1] == -2)
			pidpes->pipes[1][1] = open(1, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		c_fd[0] = dup2(pidpes->pipes[1][0], STDIN_FILENO);
		c_fd[1] = dup2(pidpes->pipes[1][1], STDOUT_FILENO);
		close(pidpes->pipes[1][0]);
		close(pidpes->pipes[1][1]);
	}
	else
	{
		if (pidpes->pipes[0][1] == -2)
			pidpes->pipes[0][1] = open(1, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		c_fd[0] = dup2(pidpes->pipes[0][0], STDIN_FILENO);
		c_fd[1] = dup2(pidpes->pipes[0][1], STDOUT_FILENO);
		close(pidpes->pipes[0][0]);
		close(pidpes->pipes[0][1]);
	}
}

int	ft_open_fds_middle(int *c_fd, t_pidpes *pidpes, t_ast *ast, t_env *env);
{
	ft_close_correct_pipes(pidpes);
	ft_analyse_file(pidpes, ast, env);
	if ((pidpes->i % 2) == 0)
	{
		c_fd[0] = dup2(pidpes->pipes[1][0], STDIN_FILENO);
		c_fd[1] = dup2(pidpes->pipes[0][1], STDOUT_FILENO);
		close(pidpes->pipes[1][0]);
		close(pidpes->pipes[0][1]);
	}
	else
	{
		c_fd[0] = dup2(pidpes->pipes[0][0], STDIN_FILENO);
		c_fd[1] = dup2(pidpes->pipes[1][1], STDOUT_FILENO);
		close(pidpes->pipes[0][0]);
		close(pidpes->pipes[1][1]);
	}
	// if ((c_fd[0] == -1) || (c_fd[1] == -1))
	// 	ft_error_open_c_fd();
	return (0);
}

int	ft_open_fds_first(int *c_fd, t_pidpes *pidpes, t_ast *ast, t_env *env)
{
	int	i;

	i = 0;
	close(pidpes->pipes[0][0]);
	pidpes->pipes[0][0] = -2;
	while (/* parcours fd*/)
	{
		if (/* token < || << */)
			ft_open_input_pipe(pidpes->pipes[0][0], i, ast, env);
		if (/* token > || >>*/)
			ft_open_output_pipe(pidpes->pipes[0][1], i, ast, env);
		i++;
	}
	if (pidpes->pipes[0][0] == -2)
		pidpes->pipes[0][0] = open(0, O_RDONLY);
	// if (pidpes->pipes[0][0] == -1)
	// 	ft_error_open();
	c_fd[0] = dup2(pidpes->pipes[0][0], STDIN_FILENO);
	c_fd[1] = dup2(pidpes->pipes[0][1], STDOUT_FILENO);
	close(pidpes->pipes[0][0]);
	close(pidpes->pipes[0][1]);
	// if ((c_fd[0] == -1) || (c_fd[1] == -1))
	// 	ft_error_open_c_fd();
	return (0);
}
