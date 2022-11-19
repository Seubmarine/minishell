/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 01:39:38 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/19 06:49:53 by tbousque         ###   ########.fr       */
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

// int	main(int argc, char const *argv[])
// {
// 	long long unsigned	seed;	
// 	int					fd;
// 	int					i;
// 	char				*filename;

// 	fd = open("/dev/urandom", O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror("Minishell: /dev/urandom");
// 		return (1);
// 	}
// 	i = 0;
// 	while (i < 1000)
// 	{
// 		read(fd, &seed, sizeof(seed));
// 		filename = heredoc_naming(i, seed);
// 		if (filename == NULL)
// 			break ;
// 		printf("%s\n", filename);
// 		free(filename);
// 		i++;
// 	}
// 	close(fd);
// 	return (0);
// }

void	heredoc_env_variable(int fd, char *line, size_t *line_pos, t_env *env)
{
	size_t	var_size;
	char	*key;
	char	*value;

	var_size = 0;
	*line_pos += 1;
	while (line[*line_pos + var_size] && !isspace(line[*line_pos + var_size]) \
	&& line[*line_pos + var_size] != '$')
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
		if (line_size > 0 && ft_strncmp(line, eof, line_size) == 0) //TODO: use ft
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

// int	ast_command_heredoc_unlink(t_ast_command *cmd)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i < cmd->redirection.len)
// 	{
// 		t_ast_redirection *redir = vec_get(&cmd->redirection, i);
// 		if (redir->token.type == TOKEN_HERE_DOCUMENT)
// 		{

// 		}
// 		i++;
// 	}
// }

// int ast_heredoc_unlink(t_ast *ast)
// {
// 	size_t	i;
// 	size_t	j;
// 	t_ast_command		*cmds;
// 	t_ast_redirection	*redir;

// 	cmds = vec_get(&ast->pipeline, 0);
// 	while (i < ast->pipeline.len)
// 	{
// 		ast
// 	}

// }

// int ast_command_heredoc_open(t_ast_command *cmd, t_env *env, size_t *heredoc_number)
// {
// 	size_t i;
// 	char	*filename;

// 	i = 0;
// 	while (i < cmd->redirection.len)
// 	{
// 		t_ast_redirection *redir = vec_get(&cmd->redirection, i);
// 		if (redir->token.type == TOKEN_HERE_DOCUMENT)
// 		{
// 			filename = naming_heredoc(*heredoc_number);
// 			if (filename == NULL)
// 				return (0); // error trying to create heredoc name
// 			if (read_in_heredoc(filename, redir->rhs.word) != 1) //opening error or ctrl+C happened
// 			{
// 				free(filename);
// 				return (0);
// 			}
// 			free(redir->rhs.word);
// 			redir->rhs.word = filename;
// 			*heredoc_number += 1;
// 		}
// 		i++;
// 	}
// 	return (1);
// }

// int ast_open_heredoc(t_ast *ast, t_env *env)
// {
// 	size_t heredoc_number;
// 	size_t i_cmd;
// 	t_ast_command *cmd;

// 	heredoc_number = 0;
// 	i_cmd = 0;
// 	while (i_cmd < ast->pipeline.len)
// 	{
// 		cmd = vec_get(&ast->pipeline, i_cmd);
// 		ast_command_open_heredoc(cmd, env, &heredoc_number);
// 		i_cmd++;
// 	}
// }