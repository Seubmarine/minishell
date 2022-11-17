/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 11:14:40 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/12 11:14:41 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

void	ft_init_pid(t_pidpes *pidpes, t_ast *ast)
{
	int	i;

	i = -1;
	pidpes->i = 0;
	pidpes->pipes[0][0] = -2;
	pidpes->pipes[0][1] = -2;
	pidpes->pipes[1][0] = -2;
	pidpes->pipes[1][1] = -2;
	pidpes->pid = malloc(sizeof(int) * ast->pipeline.len);
	// if pipe->pid == NULL
	while (++i < ast->pipeline.len)
		pidpes->pid[0] = 0;
}

int	ft_final_cmd(t_pidpes *pidpes, t_ast *ast, t_env *env)
{
	int	status;

	status = 0;
	ft_close_pipes(pidpes);
	pidpes->pid[pidpes->i] = fork();
	// if (pidpes->pid[pidpes->i] == -1)
	// 	return (ft_error_fork());
	if (pidpes->pid[pidpes->i] == 0)
	{
		env->is_child = 1;
		status = ft_last_exec(pidpes, ast, env);
		return (status);
	}
	pidpes->i++;
	ft_close_pipes(pidpes);
	pidpes->i = 0;
	return (status);
}

int	ft_middle_cmd(t_pidpes *pidpes, t_ast *ast, t_env *env)
{
	int	status;

	pidpes->i++;
	while (pidpes->i < (ast->pipeline.len -1))
	{
		if (ft_open_close_pipes(pidpes) == 1)
			return (1);
		pidpes->pid[pidpes->i] = fork();
		// if (pidpes->pid[pidpes->i] == -1)
		// 	return (ft_error_fork(pidpes));
		if (pidpes->pid[pidpes->i] == 0)
		{
			env->is_child = 1;
			status = ft_middle_exec(pidpes, ast, env);
			return (status);
		}
		pidpes->i++;
	}
	status = ft_final_cmd(pidpes, ast, env);
	return (status);
}

int	ft_multi_command(t_ast *ast, t_env *env)
{
	int			status;
	t_pidpes	pidpes;

	status = 0;
	ft_init_pid(&pidpes, ast);
	if (pipe(pidpes.pipes[pidpes.i]) == -1)
		return (ft_error_pipe(&pidpes));
	pidpes.pid[pidpes.i] = fork();
	if (pidpes.pid[pidpes.i] == -1)
		return (ft_error_fork(&pidpes));
	if (pidpes.pid[pidpes.i] == 0)
	{
		env->is_child = 1;
		status = ft_first_exec(&pidpes, ast, env);
	}
	else
		status = ft_middle_cmd(&pidpes, ast, env);
	if (env->is_child == 0)
	{
		while (pidpes.i < ast->pipeline.len)
		{
			waitpid(pidpes.pid[pidpes.i], &status, 0);
			pidpes.i++;
		}
	}
	return (status);
}
