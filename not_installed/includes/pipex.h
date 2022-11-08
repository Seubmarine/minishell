/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 12:37:45 by mportrai          #+#    #+#             */
/*   Updated: 2022/06/13 18:15:04 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include <stdio.h>
# include "../libft/libft.h"

typedef struct s_fderr
{
	pid_t	c_pid[2];
	int		fd[2];
	int		p_fd[2];
	int		c_fd[2];
	int		errin;
	int		errnum;
}	t_fderr;

void	ft_init_value(t_fderr *fd_errnum);
void	ft_open_fd(t_fderr *fd_errnum, char **argv);
void	ft_pipex(t_fderr *fd_errnum, char **argv, char **envp);
void	ft_init_pipefork(t_fderr *fd_errnum);
void	ft_init_pipefork(t_fderr *fd_errnum);
void	ft_error_pipe(t_fderr *fd_errnum);
void	ft_error_fork(t_fderr *fd_errnum);
void	ft_error_c_fd(t_fderr *fd_errnum);
void	ft_error_no_argv(t_fderr *fd_errnum);
void	ft_first_cmd(t_fderr *fd_errnum, char *argv, char **envp);
void	ft_last_cmd(t_fderr *fd_errnum, char *argv, char **envp);
void	ft_close_pipe(t_fderr *fd_errnum);
void	ft_close_fd(t_fderr *fd_errnum);
char	**ft_prepare_cmd(char *argv, t_fderr *fd_errnum);
char	*ft_prepare_path(char **cmd, char **envp, t_fderr *fd_errnum);
char	*ft_check_d_path(char **cmd, t_fderr *fd_errnum);
char	*ft_prepare_no_path(char **cmd, t_fderr *fd_errnum);
char	*ft_prepare_e_path(char **envp);
char	*ft_prepare_w_path(char **cmd, char *e_path);
char	**ft_prepare_a_path(char *e_path);
char	**ft_path_join(char **a_path);
char	*ft_correct_path(char *cmd, char **a_path);
void	ft_freetab(char **tab);
void	ft_error_exec(char *s1, char **s2, t_fderr *fd_errnum);
void	ft_error_path(t_fderr *fd_errnum);
void	ft_error(char **cmd, t_fderr *fd_errnum);
void	ft_error_cmd(char *s1, char **s2, t_fderr *fd_errnum);
void	ft_error_d_cmd(char *s1, char **s2, t_fderr *fd_errnum);

#endif
