/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:02:16 by tbousque          #+#    #+#             */
/*   Updated: 2022/09/18 20:09:29 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/*
The lexer as the purpose of transforming a string in a series of Token
It's to simplify what the shell have to do for each series of token

Token is composed of a span with a beginning and an end in the string where the
token in located and a type for the token like string or pipe

In this given example we know that :
	the first 3 token will compose a command and it's arguments
	
	after the token of type REDIRECT_OUTPUT should be a token of type string
	to be valid and will use this token to open a file

Token: (type: TOKEN_STRING) Span : (begin: 0, end: 10)
Token: (type: TOKEN_STRING) Span : (begin: 12, end: 13)
Token: (type: TOKEN_STRING) Span : (begin: 15, end: 16)
Token: (type: TOKEN_REDIRECT_OUTPUT) Span : (begin: 18, end: 18)
Token: (type: TOKEN_STRING) Span : (begin: 20, end: 23)
Token: (type: TOKEN_END) Span : (begin: 24, end: 24)
/usr/bin/ls -R .. > out1
<         > <> <> ^ <  >^

It helps to see if an input is valid and ease the program job for later
*/

int	ft_isalpha(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// ft_isatoken(char c)
// {
// 	return (c == '<' || c == '>' || c == ' ' || c == '*' || c == ''
// 	|| c == '&' ||)
// }

void	put_token(t_vec *v, enum e_token_type type,
			size_t *current, size_t token_size)
{
	const t_token	token = {
		.span = {
		.begin = *current,
		.end = *current + token_size - 1},
		.type = type,
	};

	*current += token_size - 1;
	vec_append(v, (void *)&token);
}

t_vec	lexer(char const *str)
{
	t_vec			v;
	size_t			i;

	v = vec_new(sizeof(t_token), 16, NULL);
	i = 0;
	while (str[i])
	{
		while (str[i] && str[i] == ' ')
			i++;
		if (!strncmp(str + i, "*", 1))
			put_token(&v, TOKEN_ASTERISK, &i, 1);
		else if (!strncmp(str + i, "||", 2))
			put_token(&v, TOKEN_OR, &i, 2);
		else if (!strncmp(str + i, "&&", 2))
			put_token(&v, TOKEN_AND, &i, 2);
		else if (!strncmp(str + i, "|", 1))
			put_token(&v, TOKEN_PIPE, &i, 1);
		else if (!strncmp(str + i, ">>", 2))
			put_token(&v, TOKEN_REDIRECT_OUTPUT_APPEND, &i, 2);
		else if (!strncmp(str + i, ">", 1))
			put_token(&v, TOKEN_REDIRECT_OUTPUT, &i, 1);
		else if (!strncmp(str + i, "<<", 2))
			put_token(&v, TOKEN_HERE_DOCUMENT, &i, 2);
		else if (!strncmp(str + i, "<", 1))
			put_token(&v, TOKEN_REDIRECT_INPUT, &i, 1);
		else if (!strncmp(str + i, "$", 1))
			put_token(&v, TOKEN_DOLLAR, &i, 1);
		else if (!strncmp(str + i, "'", 1))
			put_token(&v, TOKEN_SINGLE_QUOTE, &i, 1);
		else if (!strncmp(str + i, "\"", 1))
			put_token(&v, TOKEN_DOUBLE_QUOTE, &i, 1);
		else
		{
			t_token *tok;
			if (v.len >= 1)
			{
				tok = (t_token *)vec_get(&v, v.len - 1);
				if (tok->type == TOKEN_STRING && tok->span.end == i - 1)
					tok->span.end += 1;
				else
					put_token(&v, TOKEN_STRING, &i, 1);

			}
			else
				put_token(&v, TOKEN_STRING, &i, 1);
		}
		i++;
	}
	put_token(&v, TOKEN_END, &i, 1);
	return (v);
}
