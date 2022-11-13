/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 01:39:38 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/12 06:51:07 by tbousque         ###   ########.fr       */
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

void	ft_swap(unsigned char *a, unsigned char *b)
{
	unsigned char	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	ft_strrev(char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = strlen(str) - 1;
	while (i < j)
	{
		ft_swap(&str[i], &str[j]);
		i++;
		j--;
	}
}

int	_itoa_buf(unsigned long long x, char *buffer, size_t buffer_size)
{
	if (buffer_size == 0)
		return (0);
	if (x == 0)
	{
		buffer[0] = '\0';
		return (1);
	}
	buffer[0] = (x % 10 + '0');
	if (!_itoa_buf(x / 10, buffer + 1, buffer_size - 1))
		return (0);
	return (1);
}

//put an int in a string with a max size
//return 0 on failure 1 one success
int	itoa_buf(unsigned long long x, char *buffer, size_t buffer_size)
{
	size_t				i;
	unsigned long long	n;
	const int			is_neg = x < 0;

	i = 0;
	n = x;
	if (buffer_size == 0)
		return (0);
	if (n == 0)
	{
		if (buffer_size < 2)
			return (0);
		memcpy(buffer, "0", 2);
		return (1);
	}
	if (is_neg)
	{
		buffer[0] = '-';
		n = -n;
	}
	if (!_itoa_buf(n, buffer + (is_neg), buffer_size - (is_neg)))
		return (0);
	ft_strrev(buffer + (is_neg));
	return (1);
}

char	*heredoc_naming(int heredoc_number, long long unsigned random)
{
	char	filename[HEREDOC_FILENAME_MAX_LEN];
	char	random_str[HEREDOC_RANDOM_NUMBER_LEN];
	char	number_str[HEREDOC_NUMBER_LEN];
	char	*new_filename;

	filename[HEREDOC_FILENAME_MAX_LEN] = HEREDOC_NAMING;
	if (!itoa_buf(heredoc_number, number_str, HEREDOC_NUMBER_LEN))
		return (NULL);
	if (strlcat(filename, number_str, sizeof(filename)) >= sizeof(filename))
		return (NULL);
	if (strlcat(filename, ".", sizeof(filename)) >= sizeof(filename))
		return (NULL);
	if (!itoa_buf(random, random_str, HEREDOC_NUMBER_LEN))
		return (NULL);
	if (strlcat(filename, random_str, sizeof(filename)) >= sizeof(filename))
		return (NULL);
	new_filename = strdup(filename);
	return (new_filename);
}

int	main(int argc, char const *argv[])
{
	long long unsigned	seed;	
	int					fd;
	int					i;
	char				*filename;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		perror("Minishell: /dev/urandom");
		return (1);
	}
	i = 0;
	while (i < 1000)
	{
		read(fd, &seed, sizeof(seed));
		filename = heredoc_naming(i, seed);
		if (filename == NULL)
			break ;
		printf("%s\n", filename);
		free(filename);
		i++;
	}
	close(fd);
	return (0);
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