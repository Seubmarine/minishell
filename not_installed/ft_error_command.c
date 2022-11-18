/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 19:24:05 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/11 19:24:07 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

void	ft_error_open(int *fd, int i, t_ast *ast, t_env *env)
{
	if (i == -1)
	{
		if (fd[0] == -1)
			ft_putstr_fd("/dev/stdin: Message erreur\n", 2);
		else
			ft_putstr_fd("/dev/stdout: Message erreur\n", 2);
	}
	else
		perror(/*arg[i]*/);
	if ((fd[0] != -2) || (fd[0] != -1))
		close(fd[0]);
	if ((fd[1] != -2) || (fd[1] != -1))
		close(fd[1]);
	ast_free(ast);
	envp_free(env);
}

void	ft_error_open_c_fd(int *c_fd, t_ast *ast, t_env *env)
{
	perror("dup2: ");
	if (c_fd[0] == -1)
		close(c_fd[0]);
	else
		close(c_fd[1]);
	ast_free(ast);
	envp_free(env);
	exit(1);
}

void	ft_error_cmd(int fd, int c_fd, t_ast *ast, t_env *env)
{
	ft_putstr_fd("Error during command preparation\n", 2);
	close(c_fd[0]);
	close(c_fd[1]);
	ast_free(ast);
	envp_free(env);
	exit (1);
}

void	ft_error_path(int c_fd, char **cmd, t_ast *ast, t_env *env)
{
	int	i;

	i = -1;
	ft_putstr_fd("Error during path preparation\n", 2);
	while (cmd[++i])
		free(cmd[i]);
	free(cmd);
	close(c_fd[0]);
	close(c_fd[1]);
	ast_free(ast);
	envp_free(env);
	exit (1);
}

void	ft_error_execve(int c_fd, char **cmd, t_ast *ast, t_env *env)
{
	int	i;

	i = -1;
	perror("execve: ");
	while (cmd[++i])
		free(cmd[i]);
	free(cmd);
	close(c_fd[0]);
	close(c_fd[1]);
	ast_free(ast);
	envp_free(env);
	exit (1);
}
