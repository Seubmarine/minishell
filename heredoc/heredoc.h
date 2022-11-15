/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 02:52:43 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/15 08:27:39 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <bsd/string.h>
# include <sys/stat.h>
# include "ast.h"

// # include "lexer.h"
// # include "ast.h"
# define HEREDOC_NAMING "/tmp/.minishell.heredoc."
# define HEREDOC_FILENAME_MAX_LEN 1024
# define HEREDOC_RANDOM_NUMBER_LEN 32
# define HEREDOC_NUMBER_LEN 32

int	itoa_buf(unsigned long long x, char *buffer, size_t buffer_size);
char	*heredoc_open_routine(t_env *env, size_t heredoc_number, char *eof);
#endif