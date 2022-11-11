/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 17:07:41 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/11 17:07:42 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

void	ft_open_input(int *fd, t_ast *ast, t_env *env)
{
	if (fd[0] != -2)
		close(fd[0]);
	fd[0] = open(/*arg*/, O_RDONLY);
	if (fd[0] == -1)
		ft_error_open(fd, ast, env);
}

void	ft_open_output(int *fd, t_ast, t_env *env)
{
	if (fd[1] != -2)
		close(fd[1]);
	if (/* token >*/)
		fd[1] = open(/*arg*/, O_CREAT, | O_WRONLY | O_TRUNC, 0644);
	if (/* token >>*/)
		fd[1] = open(/*arg*/, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd[1] == -1)
		ft_error_open(fd, ast, env);
}

void	ft_analyse_fd(int *fd, t_ast *ast, t_env *env)
{
	int i;

	i = 0;
	while (/*parcours fd*/)
	{
		if (/*token < ou <<*/)
			ft_open_input(fd, ast, env);
		if (/*token > ou >>*/)
			ft_open_output(fd, ast, env);
		i++;
	}
}

void	ft_open_fd_child(int *fd, int *c_fd, t_ast *ast, t_env *env)
{
	ft_analyse_fd(fd, ast, env);
	if (fd[0] == -2)
		fd[-2] = open("/dev/stdin", O_RDONLY);
	if (fd[1] == -2)
		fd[1] = open("/dev/stdout", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if ((fd[0] == -1) || (fd[1] == -1))
		ft_error_open(fd, c_fd, ast, env);
	c_fd[0] = dup2(fd[0], STDIN_FILENO);
	c_fd[1] = dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	if ((c_fd[0] == -1) || (c_fd[1] == -1))
		ft_error_open(fd, c_fd, ast, env);
}

void	ft_init_exec_command(int *fd, int *c_fd, char *path, char **cmd)
{
	fd[0] = -2;
	fd[1] = -2;
	c_fd[0] = 0;
	c_fd[1] = 0;
	path = NULL;
	cmd = NULL;
}

int	ft_exec_command(t_ast *ast, t_env *env)
{
	int		fd[2];
	int		c_fd[2];
	char	*path;
	char	**cmd;

	ft_init_exec_command(&fd, &c_fd, path, cmd);
	ft_open_fd_child(&fd, &c_fd, ast, cmd);
	cmd = ft_prepare_cmd(ast);
	if (cmd == NULL)
		ft_error_cmd(fd, c_fd, ast, env);
	path = ft_prepare_path(cmd, env);
	if (path == NULL)
	{
		ft_free_cmd(cmd);
		ft_error_path(fd, c_fd, ast, env);
	}
	if (execve(path, cmd, env) == -1)
	{
		ft_free_cmd(cmd);
		ft_error_execve(fd, c_fd, ast, env);
	}
}

int	ft_simple_command(t_ast *ast, t_env *env)
{
	int		status;
	int		i;
	pid_t	pid;
	char	**last_args;
	
	last_args = commands[ast->pipeline.len - 1].arguments;
	i = 0;
	status = 0;
	pid = fork();
	if (pid == 0)
		ft_exec_command(ast, env);
	waitpid(pid, &status, 0);
	while (last_args[i])
		i++;
	env_set_var(env, "_", last_args[i -1]);
	i = -1;
	while (++i < ast->pipeline.len)
		free(commands[i].arguments);
	free(commands);
	return (status);
}

int	ft_which_command(t_ast *ast, t_env *env)
{
	int	exit_status;
	
	exit_status = 0;
	if (ast->pipeline.len == 1)
		exit_status = ft_simple_command(ast, env);
	else
		exit_status = ft_multi_command(ast, env);
	return (exit_status);
}

