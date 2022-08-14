/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:03:31 by tbousque          #+#    #+#             */
/*   Updated: 2022/08/14 18:41:33 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include <stdlib.h>
# include "vector.h"

enum e_token_type
{
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_PIPE,
	TOKEN_TYPE_OR,
	TOKEN_TYPE_ASTERISK,
	TOKEN_TYPE_END,
	TOKEN_TYPE_UNKNOW,
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