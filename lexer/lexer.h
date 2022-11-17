/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:03:31 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/17 03:43:57 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include <stdlib.h>
# include <stdio.h>
# include <ctype.h> //TODO: replace with libft for isspace
# include "vector.h"
# include "environement_variable.h"
# include "libft.h"

enum e_token_type
{
	TOKEN_STRING,
	TOKEN_PIPE,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_ASTERISK,
	TOKEN_REDIRECT_INPUT,
	TOKEN_REDIRECT_OUTPUT,
	TOKEN_REDIRECT_OUTPUT_APPEND,
	TOKEN_HERE_DOCUMENT,
	TOKEN_HERE_DOCUMENT_OPEN,
	TOKEN_DOLLAR,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_END,
	TOKEN_SPACE,
	TOKEN_UNKNOW,
};

typedef struct s_token
{
	enum e_token_type	type;
	char				*word;
}	t_token;

typedef struct s_token_parse_helper
{
	char				*value;
	enum e_token_type	type;
}	t_token_parse_helper;

typedef struct s_token_info
{
	enum e_token_type	type;
	size_t				len;
}	t_token_info;

t_vec			lexer(char *str, t_env env);
void			lexer_debug(t_vec	tokens);
t_token_info	is_token(char const *str);
void			tokens_append(t_vec *tokens, t_token *current);
void			token_free(t_token *token);

#endif