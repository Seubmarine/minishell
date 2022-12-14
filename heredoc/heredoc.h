/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 02:52:43 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/22 22:03:44 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <ctype.h>
# include <signaling.h>
# include "ast.h"
# include "libft.h"

# define HEREDOC_NAMING "/tmp/.minishell.heredoc."
# define HEREDOC_FILENAME_MAX_LEN 1024
# define HEREDOC_RANDOM_NUMBER_LEN 32
# define HEREDOC_NUMBER_LEN 32

static volatile sig_atomic_t	g_heredoc_ctrl_c = 0;

typedef struct s_hd_fd
{
	int	fdin_dup;
	int	heredoc_fd;
}	t_hd_fd;

void	signal_handling_heredoc(void);
char	*heredoc_open_routine(t_env *env, size_t heredoc_number, char *eof);
char	*ft_strndup(const char *s, size_t n);
int		heredoc_null_line(t_hd_fd *hd_fds, char *filename);
int		heredoc_open_fd(t_hd_fd *hd_fds, char *filename);
char	*heredoc_naming(int heredoc_number, char *random_str);

#endif