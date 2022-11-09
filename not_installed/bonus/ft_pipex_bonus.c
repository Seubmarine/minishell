/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 17:37:16 by mportrai          #+#    #+#             */
/*   Updated: 2022/06/14 18:11:44 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

int	ft_strlen_l(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		i++;
	return (i);
}

void	ft_first_cmd(t_fderr *fd_errnum, char **argv, char **envp)
{
	char	*path;
	char	**cmd;

	path = NULL;
	cmd = NULL;
	fd_errnum->fd[0] = open(argv[1], O_RDONLY);
	if (fd_errnum->fd[0] == -1)
	{
		perror(argv[1]);
		// ft_error_fd(fd_errnum);
		exit(2);
	}
	close(fd_errnum->p_fd[0][0]);
	fd_errnum->c_fd[0] = dup2(fd_errnum->fd[0], STDIN_FILENO);
	fd_errnum->c_fd[1] = dup2(fd_errnum->p_fd[0][1], STDOUT_FILENO);
	close(fd_errnum->p_fd[0][1]);
	close(fd_errnum->fd[0]);
	if ((fd_errnum->c_fd[0] == -1) || (fd_errnum->c_fd[1] == -1))
		ft_error_c_fd(fd_errnum);
	cmd = ft_prepare_cmd(argv[2], fd_errnum);
	path = ft_prepare_path(cmd, envp, fd_errnum);
	if (execve(path, cmd, envp) == -1)
		ft_error_exec(path, cmd, fd_errnum);
}

void	ft_middle_cmd(t_fderr *fd_errnum, char **argv, char **envp, int i)
{
	char	*path;
	char	**cmd;

	path = NULL;
	cmd = NULL;
	close(fd_errnum->p_fd[i - 1][1]);
	close(fd_errnum->p_fd[i][0]);
	fd_errnum->c_fd[0] = dup2(fd_errnum->p_fd[i - 1][0], STDIN_FILENO);
	fd_errnum->c_fd[1] = dup2(fd_errnum->p_fd[i][1], STDOUT_FILENO);
	close(fd_errnum->p_fd[i - 1][0]);
	close(fd_errnum->p_fd[i][1]);
	if ((fd_errnum->c_fd[0] == -1) || (fd_errnum->c_fd[1] == -1))
		ft_error_c_fd(fd_errnum);
	cmd = ft_prepare_cmd(argv[i + 2], fd_errnum);
	path = ft_prepare_path(cmd, envp, fd_errnum);
	if (execve(path, cmd, envp) == -1)
		ft_error_exec(path, cmd, fd_errnum);
}

void	ft_last_cmd(t_fderr *fd_errnum, char **argv, char **envp, int i)
{
	char	*path;
	char	**cmd;

	path = NULL;
	cmd = NULL;
	close(fd_errnum->p_fd[i - 1][1]);
	fd_errnum->fd[1] = open(\
	argv[ft_strlen_l(argv) - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd_errnum->fd[1] == -1)
	{
		perror(argv[ft_strlen_l(argv) - 1]);
		// ft_error_fd(fd_errnum);
		exit (1);
	}
	fd_errnum->c_fd[0] = dup2(fd_errnum->p_fd[i - 1][0], STDIN_FILENO);
	fd_errnum->c_fd[1] = dup2(fd_errnum->fd[1], STDOUT_FILENO);
	close(fd_errnum->p_fd[i - 1][0]);
	close(fd_errnum->fd[1]);
	if ((fd_errnum->c_fd[0] == -1) || (fd_errnum->c_fd[1] == -1))
		ft_error_c_fd(fd_errnum);
	cmd = ft_prepare_cmd(argv[i + 2], fd_errnum);
	path = ft_prepare_path(cmd, envp, fd_errnum);
	if (execve(path, cmd, envp) == -1)
		ft_error_exec(path, cmd, fd_errnum);
}

void	ft_open_close_pipe(t_fderr *fd_errnum, int i)
{
	if ((i - 2) >= 0)
	{
		close(fd_errnum->p_fd[i - 2][0]);
		close(fd_errnum->p_fd[i - 2][1]);
	}
	if (pipe(fd_errnum->p_fd[i]) == -1)
		ft_error_pipe(fd_errnum);
}

void	ft_final(t_fderr *fd_errnum, char **argv, char **envp, int i)
{
	ft_close_pipe(fd_errnum, i - 2);
	fd_errnum->c_pid[i] = fork();
	if (fd_errnum->c_pid[i] == -1)
		ft_error_fork(fd_errnum);
	if (fd_errnum->c_pid[i] == 0)
	{
		ft_last_cmd(fd_errnum, argv, envp, i);
		return ;
	}
	ft_close_pipe(fd_errnum, i - 1);
	i = 0;
}

void	ft_middle(t_fderr *fd_errnum, char **argv, char **envp, int i)
{
	i++;
	while (i < (fd_errnum->nbfork - 1))
	{
		ft_open_close_pipe(fd_errnum, i);
		fd_errnum->c_pid[i] = fork();
		if (fd_errnum->c_pid[i] == -1)
			ft_error_fork(fd_errnum);
		if (fd_errnum->c_pid[i] == 0)
		{
			ft_middle_cmd(fd_errnum, argv, envp, i);
			return ;
		}
		i++;
	}
	ft_final(fd_errnum, argv, envp, i);
}

void	ft_pipex(t_fderr *fd_errnum, char **argv, char **envp)
{
	int	status;
	int	i;

	i = 0;
	if (pipe(fd_errnum->p_fd[i]) == -1)
		ft_error_pipe(fd_errnum);
	fd_errnum->c_pid[0] = fork();
	if (fd_errnum->c_pid[0] == -1)
		ft_error_fork(fd_errnum);
	if (fd_errnum->c_pid[0] == 0)
		ft_first_cmd(fd_errnum, argv, envp);
	else
		ft_middle(fd_errnum, argv, envp, i);
	while (i < fd_errnum->nbfork)
	{	
		waitpid(fd_errnum->c_pid[i], &status, 0);
		if (WIFEXITED(status))
			fd_errnum->errnum = WEXITSTATUS(status);
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_fderr	fd_errnum;

	ft_init_value(&fd_errnum, argc);
	if ((argc >= 5) && (envp[0] != NULL))
	{
		if (fd_errnum.errin == 1)
			return (fd_errnum.errin);
		ft_pipex(&fd_errnum, argv, envp);
		ft_close_all(&fd_errnum);
	}
	else
	{
		if (envp[0] == NULL)
			ft_putstr_fd("Error : no environment\n", 2);
		else
			ft_putstr_fd("Error : Incorrect number of arguments\n", 2);
		return (1);
	}
	return (fd_errnum.errnum);
}
