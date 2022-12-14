/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 19:43:00 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/22 21:57:02 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"

void	handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		close(STDIN_FILENO);
		g_heredoc_ctrl_c = 1;
	}
}

void	signal_handling_heredoc(void)
{
	struct sigaction	sa_child;

	sigemptyset(&sa_child.sa_mask);
	sa_child.sa_flags = SA_RESTART;
	sa_child.sa_handler = handler_heredoc;
	sigaction(SIGINT, &sa_child, NULL);
	sigaction(SIGQUIT, &sa_child, NULL);
}

char	*heredoc_naming(int heredoc_number, char *random_str)
{
	char		filename[HEREDOC_FILENAME_MAX_LEN];
	char		number_str[HEREDOC_NUMBER_LEN];
	char		*new_filename;

	filename[0] = '\0';
	if (ft_strlcat(filename, HEREDOC_NAMING, \
	sizeof(filename)) >= sizeof(filename))
		return (NULL);
	if (!ft_ullto_buf(heredoc_number, number_str, HEREDOC_NUMBER_LEN))
		return (NULL);
	if (ft_strlcat(filename, number_str, sizeof(filename)) >= sizeof(filename))
		return (NULL);
	if (ft_strlcat(filename, ".", sizeof(filename)) >= sizeof(filename))
		return (NULL);
	if (ft_strlcat(filename, random_str, sizeof(filename)) >= sizeof(filename))
		return (NULL);
	new_filename = ft_strdup(filename);
	return (new_filename);
}

int	heredoc_open_fd(t_hd_fd *hd_fds, char *filename)
{
	hd_fds->fdin_dup = dup(STDIN_FILENO);
	if (hd_fds->fdin_dup == -1)
	{
		perror("Minishell: dup");
		free(filename);
		return (1);
	}
	hd_fds->heredoc_fd = open(filename, O_WRONLY | O_EXCL | O_CREAT, \
	S_IWUSR | S_IROTH | S_IRUSR | S_IRGRP);
	if (hd_fds->heredoc_fd == -1)
	{
		perror("Minishell: heredoc open");
		free(filename);
		return (1);
	}
	return (0);
}

int	heredoc_null_line(t_hd_fd *hd_fds, char *filename)
{
	if (g_heredoc_ctrl_c == 1)
	{
		g_heredoc_ctrl_c = 0;
		printf("\n");
		if (dup2(hd_fds->fdin_dup, STDIN_FILENO) == -1)
			perror("Minishell: dup2 copy of stding for heredoc");
		if (close(hd_fds->heredoc_fd) == -1)
			perror("Minishell: close heredoc fd");
		if (unlink(filename) == -1)
			perror("Minishell: remove heredoc file");
		free(filename);
		return (1);
	}
	write(STDERR_FILENO, "Warning heredoc delimited by end of file\n", \
	41);
	return (0);
}
