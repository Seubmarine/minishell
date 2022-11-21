/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:02:16 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/21 19:42:53 by tbousque         ###   ########.fr       */
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

typedef struct s_lexer_context
{
	const char		*str;
	t_vec			*tokens;
	t_token_info	info;
	t_token			final;
	int				should_expand;
}	t_lexer_context;


int	lexer_case_token_single_quote(t_lexer_context *lctx)
{
	size_t	j;

	j = 0;
	while (lctx->str[j] != '\0' && lctx->str[j] != '\'')
		j++;
	if (lctx->str[j] == '\0')
	{
		write(STDERR_FILENO, "Minishell: error unclosed single quote\n", 39);
		return (0);
	}
	lctx->info.len = j + 1;
	lctx->final.type = TOKEN_STRING;
	lctx->final.word = ft_strndup(lctx->str, j);
	tokens_append(lctx->tokens, &lctx->final);
	return (1);
}

int	lexer_case_token_dollar(t_lexer_context *lctx, t_env env)
{
	t_token_info	next;
	size_t			reminder;
	char			*env_key;
	char			*env_value;

	next = is_token(&lctx->str[1]);
	lctx->final.type = TOKEN_STRING;
	if (lctx->should_expand == 1 && next.type == TOKEN_STRING)
	{
		env_key = ft_strndup((const char *)&lctx->str[1], next.len);
		env_value = env_get_var(env, env_key);
		free(env_key);
		env_key = NULL;
		if (env_value != NULL)
		{
			size_t j = 0;
			while (env_value[j])
			{
				lctx->final.type = TOKEN_SPACE;
				lctx->final.word = NULL;
				while (ft_isspace(env_value[j]))
					j++;
				if (j > 0)
				{
					if (tokens_append(lctx->tokens, &lctx->final) == 0)
						return (0);
				}
				if (env_value[j] == '\0')
					break ;
				reminder = j;
				while (env_value[j] != '\0' && !ft_isspace(env_value[j]))
					j++;
				lctx->final.type = TOKEN_STRING;
				lctx->final.word = ft_strndup((const char *)&env_value[reminder], j - reminder);
				if (lctx->final.word == NULL || tokens_append(lctx->tokens, &lctx->final) == 0)
					return (0);
			}
		}
		lctx->info.len = next.len + 1;
	}
	else if (next.type == TOKEN_DOUBLE_QUOTE || next.type == TOKEN_SINGLE_QUOTE)
		;
	else
	{
		lctx->final.type = TOKEN_STRING;
		lctx->final.word = ft_strndup("$", 1);
		tokens_append(lctx->tokens, &lctx->final);
	}
	return (1);
}

int lexer_case_token_double_quote(t_lexer_context *lctx, t_env env)
{
	size_t			j;
	t_vec			word;
	t_token_info	next;
	char			*env_key;
	char			*env_value;
	size_t			k;

	j = 0;
	while (lctx->str[j] != '\0' && lctx->str[j] != '\"')
		j++;
	if (lctx->str[j] == '\0')
	{
		ft_putstr_fd("Minishell: error unclosed double quote\n", STDERR_FILENO);
		return (0);
	}
	word = vec_new(sizeof(char), j + 1, NULL);
	j = 0;
	while (lctx->str[j] != '\"')
	{
		if (lctx->should_expand == 1 && lctx->str[j] == '$' && \
			is_token(&lctx->str[j + 1]).type == TOKEN_STRING)
		{
			next = is_token(&lctx->str[j + 1]);
			env_key = ft_strndup((const char *)&lctx->str[j + 1], next.len);
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
			vec_append(&word, (void *)&lctx->str[j]);
		j++;
	}
	vec_append(&word, "\0");
	lctx->final.type = TOKEN_STRING;
	lctx->final.word = word.data;
	tokens_append(lctx->tokens, &lctx->final);
	lctx->info.len = j + 1;
	return (1);
}

//return 0 on failure, 1 on success
int	lexer(char *str, t_env env, t_vec *tokens)
{
	t_lexer_context lctx;

	*tokens = vec_new(sizeof(t_token), 10, (void (*)(void *))token_free);
	if (tokens->data == NULL)
	{
		perror("Minishell: vec_new");
		return (0);
	}
	lctx.str = str;
	lctx.tokens = tokens;
	lctx.should_expand = 1;
	while (lctx.str[0])
	{
		lctx.info = is_token(lctx.str);
		t_token	tok = {.type = lctx.info.type, .word = NULL};
		if (lctx.info.type == TOKEN_SPACE)
			tokens_append(tokens, &tok);
		else if (lctx.info.type == TOKEN_STRING)
		{
			tok.word = ft_strndup((const char *)lctx.str, lctx.info.len);
			tokens_append(tokens, &tok);
		}	
		else if (lctx.info.type == TOKEN_DOLLAR)
		{
			if (lexer_case_token_dollar(&lctx, env) == 0)
				return (0);
		}
		else if (lctx.info.type == TOKEN_DOUBLE_QUOTE)
		{
			lctx.str = &lctx.str[1];
			if (lexer_case_token_double_quote(&lctx, env) == 0)
				return (0);
		}
		else if (lctx.info.type == TOKEN_SINGLE_QUOTE)
		{
			lctx.str = &lctx.str[1];
			if (lexer_case_token_single_quote(&lctx) == 0)
				return (0);
		}
		else
		{
			lctx.should_expand = 1;
			if (lctx.info.type == TOKEN_HERE_DOCUMENT)
				lctx.should_expand = 0;
			lctx.final = (t_token){.type = lctx.info.type, .word = NULL};
			tokens_append(tokens, &lctx.final);
		}
		lctx.str = &lctx.str[lctx.info.len];
	}
	t_token	tok = {.type = TOKEN_END, .word = NULL};
	tokens_append(tokens, &tok);
	lexer_debug(*tokens);
	return (1);
}
