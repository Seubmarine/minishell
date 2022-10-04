/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:02:16 by tbousque          #+#    #+#             */
/*   Updated: 2022/10/04 07:35:59 by tbousque         ###   ########.fr       */
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

// void	put_token(t_vec *v, enum e_token_type type,
// 			size_t *current, size_t token_size)
// {
// 	const t_token	token = {
// 		.span = {
// 		.begin = *current,
// 		.end = *current + token_size - 1},
// 		.type = type,
// 	};

// 	*current += token_size - 1;
// 	vec_append(v, (void *)&token);
// }

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


#include <ctype.h>
t_token_info is_token(char const *str)
{
	static const t_token_parse_helper parse_helper[] = {
		{.value = ">>", .type = TOKEN_REDIRECT_OUTPUT_APPEND},
		{.value = ">", .type = TOKEN_REDIRECT_OUTPUT},
		{.value = "<<", .type = TOKEN_HERE_DOCUMENT},
		{.value = "<", .type = TOKEN_REDIRECT_INPUT},
		{.value = "$", .type = TOKEN_DOLLAR},
		{.value = "|", .type = TOKEN_PIPE},
		{.value = "\'", .type = TOKEN_SINGLE_QUOTE},
		{.value = "\"", .type = TOKEN_DOUBLE_QUOTE},
	};
	size_t i;

	i = 0;
	if (*str == '\0')
		return ((t_token_info){.type = TOKEN_END, .len = 1});
	while (i < (sizeof(parse_helper) / sizeof(parse_helper[0])))
	{
		if (strncmp(parse_helper[i].value, str, strlen(parse_helper[i].value)) == 0)
			return ((t_token_info){parse_helper[i].type, strlen(parse_helper[i].value)});
		i++;
	}
	//check TOKEN_SPACE && TOKEN_STRING
	i = 0;
	while (isspace(str[i]))
			i++;
	if (i > 0)	
		return ((t_token_info){.type = TOKEN_SPACE, .len = i});
	t_token_info next = is_token(&str[1]);
	if (next.type == TOKEN_STRING)
		return ((t_token_info){.type = TOKEN_STRING, .len =  1 + next.len});
	return ((t_token_info){.type = TOKEN_STRING, .len = 1});
}

void	token_free(t_token *token)
{
	token->type = TOKEN_UNKNOW;
	if (token->word != NULL)
		free(token->word);
}

#include <stdio.h>
void	lexer_debug(t_vec	tokens)
{
	const char *lex_token[] = {
        "TOKEN_STRING",
        "TOKEN_PIPE",
        "TOKEN_OR",
        "TOKEN_AND",
        "TOKEN_ASTERISK",
        "TOKEN_REDIRECT_INPUT",
        "TOKEN_REDIRECT_OUTPUT",
        "TOKEN_REDIRECT_OUTPUT_APPEND",
        "TOKEN_HERE_DOC",
        "TOKEN_DOLLAR",
        "TOKEN_SINGLE_QUOTE",
	    "TOKEN_DOUBLE_QUOTE",
        "TOKEN_END",
        "TOKEN_SPACE"
        "TOKEN_TYPE_UNKNOW",
    };

	size_t	i = 0;
	while (i < tokens.len)
	{
		t_token current = ((t_token *)tokens.data)[i];
		printf("[%lu] %s", i, lex_token[current.type]);
		if (current.type == TOKEN_STRING)
			printf(" = \"%s\"", current.word);
		printf("\n");
		i++;
	}
}

t_vec	lexer(char *str, t_env env)
{
	t_vec	tokens;
	size_t	i;

	(void) env;
	tokens = vec_new(sizeof(t_token), 10, (void(*)(void *))token_free);
	i = 0;
	while (str[i])
	{
		t_token_info info = is_token(&str[i]);
		t_token tok = {.type = info.type, .word = NULL};
		
		if (info.type == TOKEN_SPACE)
			;
		else if (info.type == TOKEN_STRING)
		{
			tok.word = strndup(&str[i], info.len);
			vec_append(&tokens, &tok);
		}
		else if (info.type == TOKEN_DOLLAR )
		{
			t_token_info next = is_token(&str[i + 1]);
			tok.type = TOKEN_STRING;
			if (next.type == TOKEN_STRING)
			{
				char *env_key = strndup(&str[i + 1], next.len);
				char *env_value = env_get_var(env, env_key);
				free(env_key);
				if (env_value != NULL)
				{
					size_t j = 0;
					while (env_value[j])
					{
						while (isspace(env_value[j]))
							j++;
						if (env_value[j] == '\0')
							break;
						size_t reminder = j;
						while (env_value[j] != '\0' && !isspace(env_value[j]))
							j++;
						tok.type = TOKEN_STRING;
						tok.word = strndup(&env_value[reminder], j - reminder);
						vec_append(&tokens, &tok);
					}
				}
				info.len = next.len + 1;
			}
			else
			{
				tok.type = TOKEN_STRING;
				tok.word = strndup("$", 1);
				vec_append(&tokens, &tok);

			}
		}
		else if (info.type == TOKEN_DOUBLE_QUOTE)
		{
			size_t j = 0;
			t_vec word;
			
			i++;
			while (str[i + j] != '\"')
				j++;
			word = vec_new(sizeof(char), j + 1, NULL);
			j = 0;
			while (str[i + j] != '\"')
			{
				if (str[i + j] == '$' && is_token(&str[i + j + 1]).type == TOKEN_STRING)
				{
					t_token_info next = is_token(&str[i + j + 1]);
					char	*env_key = strndup(&str[i + j + 1], next.len);
					j += next.len;
					char	*env_value = env_get_var(env, env_key);
					free(env_key);
					if (env_value)
					{
						size_t k = 0;
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
			vec_append(&tokens, &tok);
			info.len = j + 1;
		}
		else if (info.type == TOKEN_SINGLE_QUOTE)
		{
			i++;
			size_t j = 0;
			while (str[i + j] != '\'')
				j++;
			info.len = j + 1;
			tok.type = TOKEN_STRING;
			tok.word = strndup(&str[i], j);
			vec_append(&tokens, &tok);
		}
		else
		{
			t_token tok = {.type = info.type, .word = NULL};
			vec_append(&tokens, &tok);
		}
		i += info.len;
	}
	t_token tok = {.type = TOKEN_END, .word = NULL};
	vec_append(&tokens, &tok);
	lexer_debug(tokens);
	return (tokens);
}