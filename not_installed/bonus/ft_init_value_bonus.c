/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:55:23 by mportrai          #+#    #+#             */
/*   Updated: 2022/06/14 14:55:25 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	ft_init_pipe(t_fderr *fd_errnum, int argc)
{
	int	i;

	i = 0;
	fd_errnum->p_fd = malloc(sizeof(int *) * (argc - 4));
	if (fd_errnum->p_fd == NULL)
	{
		ft_putstr_fd("Error : malloc int *.\n", 2);
		exit (1);
	}
	while (i < argc - 4)
	{
		fd_errnum->p_fd[i] = malloc(sizeof(int) * 2);
		if (fd_errnum->p_fd[i] == NULL)
		{
			ft_putstr_fd("Error : malloc int.\n", 2);
			exit (1);
		}
		fd_errnum->p_fd[i][0] = 0;
		fd_errnum->p_fd[i][1] = 0;
		i++;
	}
	fd_errnum->nbpipe = i;
}

void	ft_init_value(t_fderr *fd_errnum, int argc)
{
	int	i;

	i = 0;
	fd_errnum->c_pid = malloc(sizeof(pid_t) * (argc - 3));
	if (fd_errnum->c_pid == NULL)
	{
		ft_putstr_fd("Error : malloc pid_t.\n", 2);
		exit (1);
	}
	while (i < argc - 3)
	{
		fd_errnum->c_pid[i] = 0;
		i++;
	}
	ft_init_pipe(fd_errnum, argc);
	fd_errnum->fd[0] = 0;
	fd_errnum->fd[1] = 0;
	fd_errnum->c_fd[0] = 0;
	fd_errnum->c_fd[1] = 0;
	fd_errnum->errin = 0;
	fd_errnum->errnum = 0;
	fd_errnum->id = 0;
	fd_errnum->nbfork = i;
}

void	ft_close_pipe(t_fderr *fd_errnum, int i)
{
	if (i >= 0)
	{
		close(fd_errnum->p_fd[i][0]);
		close(fd_errnum->p_fd[i][1]);
	}
}

void	ft_close_all(t_fderr *fd_errnum)
{
	int	i;

	i = -1;
	close(0);
	close(1);
	close(2);
	free(fd_errnum->c_pid);
	while (++i < fd_errnum->nbpipe)
		free(fd_errnum->p_fd[i]);
	free(fd_errnum->p_fd);
}
