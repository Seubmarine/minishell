/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 13:56:03 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/12 13:56:04 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

int	ft_last_exec(t_pidpes *pidpes, t_ast *ast, t_env *env)
{
	int	c_fd[2];

	c_fd[0];
	c_fd[1];
	ft_open_fds_last(c_fd, pidpes, ast, env);
	//prepare command;
	//prepare path;
	if (execve(path, cmd, env) == -1)
		ft_error_execve();
}

int	ft_middle_exec(t_pidpes *pidpes, t_ast *ast, t_env *env)
{
	int	c_fd[2];

	c_fd[0] = 0;
	c_fd[1] = 0;
	ft_open_fds_middle(c_fd, pidpes, ast, env);
	//prepare command;
	//prepare path;
	if (execve(path, cmd, env) == -1)
		ft_error_execve();
}

int	ft_first_exec(t_pidpes *pidpes, t_ast *ast, t_env *env)
{
	int	c_fd[2];

	c_fd[0] = 0;
	c_fd[1] = 0;
	ft_open_fds_first(c_fd, pidpes, ast, env);
	//prepare command;
	//prepare path;
	if (execve(path, cmd, env) == -1)
		ft_error_execve();
}
