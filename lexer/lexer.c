/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:02:16 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/17 03:38:17 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <ctype.h>
#include <stdio.h>

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

t_vec	lexer(char *str, t_env env)
{
	t_vec			tokens;
	size_t			i;
	t_token_info	info;
	t_token_info	next;
	char			*env_key;
	char			*env_value;
	size_t			j;
	size_t			reminder;

	(void) env;
	tokens = vec_new(sizeof(t_token), 10, (void (*)(void *))token_free);
	if (tokens.data == NULL)
	{
		perror("Minishell: vec_new");
		return (tokens);
	}
	i = 0;
	while (str[i])
	{
		info = is_token(&str[i]);
		t_token	tok = {.type = info.type, .word = NULL};
		if (info.type == TOKEN_SPACE)
			tokens_append(&tokens, &tok);
		else if (info.type == TOKEN_STRING)
		{
			tok.word = strndup(&str[i], info.len);
			tokens_append(&tokens, &tok);
		}
		else if (info.type == TOKEN_DOLLAR)
		{
			next = is_token(&str[i + 1]);
			tok.type = TOKEN_STRING;
			if (next.type == TOKEN_STRING)
			{
				env_key = strndup(&str[i + 1], next.len);
				env_value = env_get_var(env, env_key);
				free(env_key);
				env_key = NULL;
				if (env_value != NULL)
				{
					j = 0;
					while (env_value[j])
					{
						tok.type = TOKEN_SPACE;
						tok.word = NULL;
						while (isspace(env_value[j]))
							j++;
						if (j > 0)
							tokens_append(&tokens, &tok);
						if (env_value[j] == '\0')
							break ;
						reminder = j;
						while (env_value[j] != '\0' && !isspace(env_value[j]))
							j++;
						tok.type = TOKEN_STRING;
						tok.word = strndup(&env_value[reminder], j - reminder);
						tokens_append(&tokens, &tok);
					}
				}
				info.len = next.len + 1;
			}
			else if (next.type == TOKEN_DOUBLE_QUOTE || next.type == TOKEN_SINGLE_QUOTE)
				;
			else
			{
				tok.type = TOKEN_STRING;
				tok.word = strndup("$", 1);
				tokens_append(&tokens, &tok);
			}
		}
		else if (info.type == TOKEN_DOUBLE_QUOTE)
		{
			size_t			j;
			t_vec			word;
			t_token_info	next;
			char			*env_key;
			char			*env_value;
			size_t			k;

			j = 0;
			i++;
			while (str[i + j] != '\"')
				j++;
			word = vec_new(sizeof(char), j + 1, NULL);
			j = 0;
			while (str[i + j] != '\"')
			{
				if (str[i + j] == '$' && is_token(&str[i + j + 1]).type == \
				TOKEN_STRING)
				{
					next = is_token(&str[i + j + 1]);
					env_key = strndup(&str[i + j + 1], next.len);
					j += next.len;
					env_value = env_get_var(env, env_key);
					free(env_key);
					if (env_value)
					{
						k = 0;
						while (env_value[k])
						{
							vec_append(&word, &env_value[k]);
							k++;
						}
					}
					env_value = NULL;
				}
				else
					vec_append(&word, &str[i + j]);
				j++;
			}
			vec_append(&word, "\0");
			tok.type = TOKEN_STRING;
			tok.word = word.data;
			tokens_append(&tokens, &tok);
			info.len = j + 1;
		}
		else if (info.type == TOKEN_SINGLE_QUOTE)
		{
			i++;
			size_t	j;

			j = 0;
			while (str[i + j] != '\'')
				j++;
			info.len = j + 1;
			tok.type = TOKEN_STRING;
			tok.word = strndup(&str[i], j);
			tokens_append(&tokens, &tok);
		}
		else
		{
			t_token	tok = {.type = info.type, .word = NULL};
			tokens_append(&tokens, &tok);
		}
		i += info.len;
	}
	t_token	tok = {.type = TOKEN_END, .word = NULL};
	tokens_append(&tokens, &tok);
	lexer_debug(tokens);
	return (tokens);
}
