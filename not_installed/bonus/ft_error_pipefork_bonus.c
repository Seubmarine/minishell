/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_pipefork.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 14:46:52 by mportrai          #+#    #+#             */
/*   Updated: 2022/06/13 15:19:40 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	ft_error_pipe(t_fderr *fd_errnum)
{
	int	i;

	i = 0;
	ft_putstr_fd(strerror(1), 2);
	perror("Pipe");
	while (i < fd_errnum->nbpipe)
	{
		if (fd_errnum->p_fd[i][0] > 0)
		{
			close(fd_errnum->p_fd[i][0]);
			close(fd_errnum->p_fd[i][0]);
		}
		i++;
	}
	free(fd_errnum->p_fd);
	free(fd_errnum->c_pid);
	exit(1);
}

void	ft_error_fork(t_fderr *fd_errnum)
{
	int	i;

	i = 0;
	ft_putstr_fd(strerror(1), 2);
	perror("Fork");
	free(fd_errnum->p_fd);
	free(fd_errnum->c_pid);
	exit(1);
}

// void	ft_error_fd(t_fderr *fd_errnum)

void	ft_error_c_fd(t_fderr *fd_errnum)
{
	ft_putstr_fd("Error during fds duplication\n", 2);
	if (fd_errnum->c_fd[0] != -1)
		close(fd_errnum->c_fd[0]);
	if (fd_errnum->c_fd[1] != -1)
		close(fd_errnum->c_fd[1]);
	exit(1);
}

void	ft_error_no_argv(t_fderr *fd_errnum)
{
	ft_putstr_fd("Error : no argument\n", 2);
	exit(1);
}
