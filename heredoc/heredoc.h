/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 02:52:43 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/12 06:46:58 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <bsd/string.h>

// # include "lexer.h"
// # include "ast.h"
# define HEREDOC_NAMING "/tmp/.minishell.heredoc."
# define HEREDOC_FILENAME_MAX_LEN 1024
# define HEREDOC_RANDOM_NUMBER_LEN 32
# define HEREDOC_NUMBER_LEN 32
#endif