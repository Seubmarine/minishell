/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 15:11:28 by mportrai          #+#    #+#             */
/*   Updated: 2022/06/13 15:59:46 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	ft_error_d_cmd(char *s1, char **s2, t_fderr *fd_errnum)
{
	int	i;

	i = 0;
	ft_putstr_fd(s2[0], 2);
	ft_putstr_fd(" : No such file or directory\n", 2);
	close(fd_errnum->c_fd[0]);
	close(fd_errnum->c_fd[1]);
	free(s1);
	while (s2[i])
	{
		free(s2[i]);
		i++;
	}
	free(s2);
	exit(127);
}

void	ft_error_cmd(char *s1, char **s2, t_fderr *fd_errnum)
{
	int	i;

	i = 0;
	ft_putstr_fd(s2[0], 2);
	ft_putstr_fd(" : command not found\n", 2);
	close(fd_errnum->c_fd[0]);
	close(fd_errnum->c_fd[1]);
	free(s1);
	while (s2[i])
	{
		free(s2[i]);
		i++;
	}
	free(s2);
	exit(127);
}

void	ft_error_exec(char *s1, char **s2, t_fderr *fd_errnum)
{
	int	i;

	i = 0;
	ft_putstr_fd("Error : Execve dysfunction\n", 2);
	close(fd_errnum->c_fd[0]);
	close(fd_errnum->c_fd[1]);
	free(s1);
	while (s2[i])
	{
		free(s2[i]);
		i++;
	}
	free(s2);
	exit(1);
}

void	ft_error(char **cmd, t_fderr *fd_errnum)
{
	int	i;

	i = 0;
	ft_putstr_fd("Error during command preparation\n", 2);
	close(fd_errnum->c_fd[0]);
	close(fd_errnum->c_fd[1]);
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
	exit(1);
}

void	ft_error_path(t_fderr *fd_errnum)
{
	ft_putstr_fd("Error during command split\n", 2);
	close(fd_errnum->c_fd[0]);
	close(fd_errnum->c_fd[1]);
}
