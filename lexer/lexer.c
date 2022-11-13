/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:02:16 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/03 17:18:13 by tbousque         ###   ########.fr       */
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

t_token_info	is_token(char const *str)
{
	size_t								i;
	t_token_info						next;
	static const t_token_parse_helper	parse_helper[] = {
	{.value = ">>", .type = TOKEN_REDIRECT_OUTPUT_APPEND},
	{.value = ">", .type = TOKEN_REDIRECT_OUTPUT},
	{.value = "<<", .type = TOKEN_HERE_DOCUMENT},
	{.value = "<", .type = TOKEN_REDIRECT_INPUT},
	{.value = "$", .type = TOKEN_DOLLAR},
	{.value = "|", .type = TOKEN_PIPE},
	{.value = "\'", .type = TOKEN_SINGLE_QUOTE},
	{.value = "\"", .type = TOKEN_DOUBLE_QUOTE},
	};

	i = 0;
	if (*str == '\0')
		return ((t_token_info){.type = TOKEN_END, .len = 1});
	while (i < (sizeof(parse_helper) / sizeof(parse_helper[0])))
	{
		if (strncmp(parse_helper[i].value, str, strlen(parse_helper[i].value)) \
		== 0)
			return ((t_token_info){parse_helper[i].type, \
			strlen(parse_helper[i].value)});
		i++;
	}
	//check TOKEN_SPACE && TOKEN_STRING
	i = 0;
	while (isspace(str[i]))
			i++;
	if (i > 0)
		return ((t_token_info){.type = TOKEN_SPACE, .len = i});
	next = is_token(&str[1]);
	if (next.type == TOKEN_STRING)
		return ((t_token_info){.type = TOKEN_STRING, .len = 1 + next.len});
	return ((t_token_info){.type = TOKEN_STRING, .len = 1});
}

void	token_free(t_token *token)
{
	token->type = TOKEN_UNKNOW;
	if (token->word != NULL)
		free(token->word);
}

void	lexer_debug(t_vec	tokens)
{
	const char	*lex_token[] = {
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
	size_t		i;
	t_token		current;

	i = 0;
	while (i < tokens.len)
	{
		current = ((t_token *)tokens.data)[i];
		printf("[%lu] %s", i, lex_token[current.type]);
		if (current.type == TOKEN_STRING)
			printf(" = \"%s\"", current.word);
		printf("\n");
		i++;
	}
}

void	tokens_append(t_vec *tokens, t_token *current)
{
	t_token	*last;
	char	*tmp;

	if (tokens->len > 0)
	{
		last = vec_get(tokens, tokens->len - 1);
		if (last->type == TOKEN_SPACE)
		{
			last->type = current->type;
			last->word = current->word;
			return ;
		}
		else if (last->type == TOKEN_STRING && current->type == TOKEN_STRING)
		{
			tmp = malloc(sizeof(char *) * (strlen(last->word) + \
			strlen(current->word) + 1));
			tmp[0] = '\0';
			strcpy(tmp, last->word);
			strcat(tmp, current->word);
			free(last->word);
			last->word = tmp;
			// if (strcat(last->word, current->word) == NULL)
			// 	exit(EXIT_FAILURE); //TODO error handling
			free(current->word);
			return ;
		}
	}
	vec_append(tokens, current);
}

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
