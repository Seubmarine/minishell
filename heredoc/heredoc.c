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

char	*heredoc_naming(int heredoc_number, char *random_str)
{
	char	filename[HEREDOC_FILENAME_MAX_LEN] = HEREDOC_NAMING;
	char	number_str[HEREDOC_NUMBER_LEN];
	char	*new_filename;

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

char	*ft_strndup(const char *s, size_t n)
{
	char	*dupped;
	size_t	i;

	i = 0;
	while (s[i] && i < n)
		i++;
	dupped = malloc(sizeof(*dupped) * (i + 1));
	if (dupped == NULL)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		dupped[i] = s[i];
		i++;
	}
	dupped[i] = '\0';
	return (dupped);
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
		{
			line_pos++;
			size_t	var_size = 0;
			while (line[line_pos + var_size] && !isspace(line[line_pos + \
			var_size]) && line[line_pos + var_size] != '$')
				var_size++;
			if (var_size == 0)
				write(fd, "$", 1);
			else
			{
				char	*key = ft_strndup(&line[line_pos], var_size);
				if (key)
				{
					char	*value = env_get_var(*env, key);
					if (value)
						write(fd, value, ft_strlen(value));
				}
				free(key);
			}
			line_pos += var_size;
		}
	}
}

char	*heredoc_open_routine(t_env *env, size_t heredoc_number, char *eof)
{
	char	*filename;
	char	*line;
	size_t	line_size;
	int		fdin_dup;
	int		heredoc_fd;

	filename = heredoc_naming(heredoc_number, env->random_str);
	if (filename == NULL)
		return (NULL);
	fdin_dup = dup(STDIN_FILENO);
	// dup = -1
	heredoc_fd = open(filename, O_WRONLY | O_EXCL | O_CREAT, \
	S_IWUSR | S_IROTH | S_IRUSR | S_IRGRP);
	if (heredoc_fd == -1)
	{
		perror("Minishell: heredoc open");
		free(filename);
		return (NULL);
	}
	signal_handling_heredoc();
	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			if (fcntl(STDIN_FILENO, F_GETFD) == -1)
			{
				printf("\n");
				dup2(fdin_dup, STDIN_FILENO);//TODO check error
				close(heredoc_fd);
				unlink(filename);
				free(filename);
				return (NULL);
			}
			write(STDERR_FILENO, "Warning heredoc delimited by end of file\n", \
			41);
			break ;
		}
		line_size = ft_strlen(line);
		if (line_size > 0 && ft_strncmp(line, eof, line_size) == 0) //TODO: use ft
			break ;
		heredoc_write(env, heredoc_fd, line);
		write(heredoc_fd, "\n", 1);
		free(line);
	}
	close(heredoc_fd);
	close(fdin_dup);
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