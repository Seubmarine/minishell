/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:02:16 by tbousque          #+#    #+#             */
/*   Updated: 2022/09/25 17:25:18 by tbousque         ###   ########.fr       */
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

typedef struct token_parse_helper
{
	char				*value;
	enum e_token_type	type;
}	t_token_parse_helper;

#include <ctype.h>
t_token_parse_helper is_token(char const *str, size_t str_index)
{
	static const t_token_parse_helper parse_helper[] = {
		{.value = ">>", .type = TOKEN_REDIRECT_OUTPUT_APPEND},
		{.value = ">", .type = TOKEN_REDIRECT_OUTPUT},
		{.value = "<<", .type = TOKEN_HERE_DOCUMENT},
		{.value = "<", .type = TOKEN_REDIRECT_INPUT},
		{.value = "$", .type = TOKEN_DOLLAR},
		{.value = "|", .type = TOKEN_PIPE},
	};
	size_t i;

	i = 0;
	if (str[str_index] == '\0')
		return ((t_token_parse_helper){.type = TOKEN_END, .value = " "});
	while (i < (sizeof(parse_helper) / sizeof(parse_helper[0])))
	{
		if (strncmp(parse_helper[i].value, str + str_index, strlen(parse_helper[i].value)) == 0)
			return (parse_helper[i]);
		i++;
	}
	if (isspace(str[str_index]))
		return ((t_token_parse_helper){.type = TOKEN_SPACE, .value = " "});
	return ((t_token_parse_helper){.type = TOKEN_STRING, .value = " "});
}

t_vec	lexer(char const *str)
{
	t_vec			v;
	size_t			i;
	t_token tok;
	

	v = vec_new(sizeof(t_token), 16, NULL);
	i = 0;
	while (str[i])
	{
		t_token_parse_helper token_info = is_token(str, i);
		tok.span.begin = i;
		tok.type = token_info.type;
		i += strlen(token_info.value);
		if (tok.type == TOKEN_STRING)
		{
			while (is_token(str, i).type == TOKEN_STRING)
				i++;
		}
		tok.span.end = i - 1;
		if (tok.type != TOKEN_SPACE)
			vec_append(&v, &tok);
		if (token_info.type == TOKEN_END)
			break;
	}
	tok.span.begin = i;
	tok.span.end = i;
	tok.type = TOKEN_END;
	vec_append(&v, &tok);
	return (v);
}
