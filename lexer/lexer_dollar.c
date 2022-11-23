/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:02:57 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/22 16:03:50 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	lexer_token_dollar_expand_word(t_lexer_context *lctx, \
	char *env_value, size_t *current)
{
	size_t	i;

	i = 0;
	while (env_value[i] != '\0' && !ft_isspace(env_value[i]))
		i++;
	*current += i;
	lctx->final = (t_token){TOKEN_STRING, \
		ft_strndup((const char *)env_value, i)};
	if (lctx->final.word == NULL || \
	tokens_append(lctx->tokens, &lctx->final) == 0)
	{
		token_free(&lctx->final);
		return (0);
	}
	return (1);
}

int	lexer_token_dollar_expand(t_lexer_context *lctx, char *env_value)
{
	size_t	i;

	i = 0;
	while (env_value && env_value[i])
	{
		lctx->final = (t_token){.type = TOKEN_SPACE, .word = NULL};
		while (ft_isspace(env_value[i]))
			i++;
		if (i > 0)
		{
			if (tokens_append(lctx->tokens, &lctx->final) == 0)
				return (0);
		}
		if (env_value[i] == '\0')
			break ;
		if (lexer_token_dollar_expand_word(lctx, &env_value[i], &i) == 0)
			return (0);
	}
	return (1);
}

int	lexer_case_token_dollar(t_lexer_context *lctx, t_env env)
{
	lctx->next = is_token(&lctx->str[1]);
	lctx->final.type = TOKEN_STRING;
	if (lctx->should_expand == 1 && lctx->next.type == TOKEN_STRING)
	{
		lctx->env_value = lexh_env(env, \
			ft_strndup((const char *)&lctx->str[1], lctx->next.len));
		if (lctx->env_value != NULL)
		{
			if (lexer_token_dollar_expand(lctx, lctx->env_value) == 0)
				return (0);
		}
		lctx->info.len = lctx->next.len + 1;
	}
	else if (lctx->next.type == TOKEN_DOUBLE_QUOTE || \
		lctx->next.type == TOKEN_SINGLE_QUOTE)
		;
	else
	{
		lctx->final = (t_token){TOKEN_STRING, ft_strndup("$", 1)};
		if (lctx->final.word == NULL || \
		tokens_append(lctx->tokens, &lctx->final) == 0)
			return (0);
	}
	return (1);
}
