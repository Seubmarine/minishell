/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:02:16 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/23 17:09:03 by tbousque         ###   ########.fr       */
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

//pass the value of a key and free it's key
char	*lexh_env(t_env env, char *env_key)
{
	char	*env_value;

	if (env_key == NULL)
		return (NULL);
	env_value = env_get_var(env, env_key);
	free(env_key);
	return (env_value);
}

int	lexer_case_token_heredocument(t_lexer_context *lctx)
{
	lctx->should_expand = 0;
	lctx->next = is_token(&lctx->str[2]);
	if (lctx->next.type == TOKEN_SPACE)
		lctx->info.len += lctx->next.len;
	return (1);
}

int	lexer_case_next(t_lexer_context *lctx)
{
	if (lctx->info.type == TOKEN_SINGLE_QUOTE)
	{
		lctx->str = &lctx->str[1];
		if (lexer_case_token_single_quote(lctx) == 0)
			return (0);
	}
	else if (lctx->info.type == TOKEN_HERE_DOCUMENT)
	{
		if (lexer_case_token_heredocument(lctx) == 0)
			return (0);
	}
	if (lctx->info.type != TOKEN_SPACE && lctx->info.type != TOKEN_STRING \
		&& lctx->info.type != TOKEN_DOLLAR && lctx->info.type != TOKEN_SPACE \
		&& lctx->info.type != TOKEN_DOUBLE_QUOTE \
		&& lctx->info.type != TOKEN_SINGLE_QUOTE)
	{
		lctx->should_expand = 1;
		if (lctx->info.type == TOKEN_HERE_DOCUMENT)
			lctx->should_expand = 0;
		lctx->final = (t_token){.type = lctx->info.type, .word = NULL};
		if (tokens_append(lctx->tokens, &lctx->final) == 0)
			return (token_free(&lctx->final), 0);
	}
	return (1);
}

int	lexer_case(t_lexer_context *lctx, t_env env)
{
	if (lctx->info.type == TOKEN_SPACE)
	{
		lctx->should_expand = 1;
		if (tokens_append(lctx->tokens, &lctx->final) == 0)
			return (0);
	}
	else if (lctx->info.type == TOKEN_STRING)
	{
		lctx->final.word = ft_strndup((const char *)lctx->str, lctx->info.len);
		if (lctx->final.word == NULL || \
			tokens_append(lctx->tokens, &lctx->final) == 0)
			return (token_free(&lctx->final), 0);
	}	
	else if (lctx->info.type == TOKEN_DOLLAR)
	{
		if (lexer_case_token_dollar(lctx, env) == 0)
			return (0);
	}
	else if (lctx->info.type == TOKEN_DOUBLE_QUOTE)
	{
		lctx->str = &lctx->str[1];
		if (lexer_case_token_double_quote(lctx, env) == 0)
			return (0);
	}
	return (lexer_case_next(lctx));
}

//return 0 on failure, 1 on success
int	lexer(char *str, t_env env, t_vec *tokens)
{
	t_lexer_context	lctx;

	*tokens = vec_new(sizeof(t_token), 10, (void (*)(void *))token_free);
	if (tokens->data == NULL)
		return (0);
	lctx.str = str;
	lctx.tokens = tokens;
	lctx.should_expand = 1;
	lctx.env_value = NULL;
	while (lctx.str[0])
	{
		lctx.info = is_token(lctx.str);
		lctx.final = (t_token){.type = lctx.info.type, .word = NULL};
		if (lexer_case(&lctx, env) == 0)
			return (0);
		lctx.str = &lctx.str[lctx.info.len];
	}
	lctx.final = (t_token){.type = TOKEN_END, .word = NULL};
	if (tokens_append(tokens, &lctx.final) == 0)
		return (token_free(&lctx.final), 0);
	lexer_debug(*tokens);
	return (1);
}
