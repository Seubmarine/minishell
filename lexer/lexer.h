/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:03:31 by tbousque          #+#    #+#             */
/*   Updated: 2022/09/25 16:04:47 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include <stdlib.h>
# include "vector.h"

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
	TOKEN_DOLLAR,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_END,
	TOKEN_SPACE,
	TOKEN_UNKNOW,
};

typedef struct s_span
{
	size_t	begin;
	size_t	end;
}	t_span;

typedef struct s_token
{
	enum e_token_type	type;
	t_span				span;
}	t_token;

t_vec	lexer(char const *str);

#endif