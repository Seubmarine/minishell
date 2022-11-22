/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 01:39:38 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/22 21:47:34 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"

//try to put character c in the buffer and augment iterator
//return 0 if the iterator exceed buffer size
int	put_in_buffer(char *buffer, char c, size_t *iterator, size_t buffer_size)
{
	if (*iterator > buffer_size)
		return (0);
	buffer[*iterator] = c;
	*iterator += 1;
	return (1);
}

void	heredoc_env_variable(int fd, char *line, size_t *line_pos, t_env *env)
{
	size_t	var_size;
	char	*key;
	char	*value;

	var_size = 0;
	*line_pos += 1;
	while (line[*line_pos + var_size] && \
	!ft_isspace(line[*line_pos + var_size]) && \
	line[*line_pos + var_size] != '$')
		var_size++;
	if (var_size == 0)
		write(fd, "$", 1);
	else
	{
		key = ft_strndup(&line[*line_pos], var_size);
		if (key)
		{
			value = env_get_var(*env, key);
			if (value)
				write(fd, value, ft_strlen(value));
		}
		free(key);
	}
	*line_pos += var_size;
}

void	heredoc_write(t_env *env, int fd, char *line)
{
	size_t	line_pos;
	size_t	line_to_write;

	line_pos = 0;
	while (line[line_pos] != '\0')
	{
		line_to_write = 0;
		while (line[line_pos + line_to_write] && \
		line[line_pos + line_to_write] != '$')
			line_to_write++;
		write(fd, &line[line_pos], line_to_write);
		line_pos += line_to_write;
		if (line[line_pos] == '$')
			heredoc_env_variable(fd, line, &line_pos, env);
	}
}

int	heredoc_readline(t_hd_fd *hd_fds, char *eof, char *filename, t_env *env)
{
	char	*line;
	size_t	line_size;

	line = NULL;
	line_size = 0;
	g_heredoc_ctrl_c = 0;
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			if (heredoc_null_line(hd_fds, filename) == 1)
				return (1);
			break ;
		}
		line_size = ft_strlen(line);
		if (line_size > 0 && ft_strncmp(line, eof, line_size) == 0)
			break ;
		heredoc_write(env, hd_fds->heredoc_fd, line);
		write(hd_fds->heredoc_fd, "\n", 1);
		free(line);
	}
	return (0);
}

char	*heredoc_open_routine(t_env *env, size_t heredoc_number, char *eof)
{
	char	*filename;
	t_hd_fd	hd_fds;

	filename = heredoc_naming(heredoc_number, env->random_str);
	if (filename == NULL)
		return (NULL);
	if (heredoc_open_fd(&hd_fds, filename) == 1)
		return (NULL);
	signal_handling_heredoc();
	if (heredoc_readline(&hd_fds, eof, filename, env) == 1)
		return (NULL);
	close(hd_fds.heredoc_fd);
	close(hd_fds.fdin_dup);
	signal_handling();
	return (filename);
}
