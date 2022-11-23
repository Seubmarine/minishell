/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_double_quote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:59:30 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/22 16:03:34 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	lexer_double_quote_expand(t_lexer_context *lctx, t_vec *word, \
	size_t *j, t_env env)
{
	size_t	k;

	lctx->next = is_token(&lctx->str[*j + 1]);
	lctx->env_value = NULL;
	lctx->env_value = lexh_env(env, \
		ft_strndup(&lctx->str[*j + 1], lctx->next.len));
	*j += lctx->next.len;
	if (lctx->env_value)
	{
		k = 0;
		while (lctx->env_value[k])
		{
			if (vec_append(word, &lctx->env_value[k]) == 0)
				return (0);
			k++;
		}
		lctx->env_value = NULL;
	}
	return (1);
}

int	lexer_token_double_quote_loop(t_lexer_context *lctx, \
	size_t *j, t_env env)
{
	t_vec	word;

	word = vec_new(sizeof(char), *j + 1, NULL);
	if (word.data == NULL)
		return (0);
	*j = 0;
	while (lctx->str[*j] != '\"')
	{
		if (lctx->should_expand == 1 && lctx->str[*j] == '$' && \
			is_token(&lctx->str[*j + 1]).type == TOKEN_STRING)
		{
			if (lexer_double_quote_expand(lctx, &word, j, env) == 0)
				return (vec_free(&word), 0);
		}
		else
		{
			if (vec_append(&word, (void *)&lctx->str[*j]) == 0)
				return (vec_free(&word), 0);
		}
		*j += 1;
	}
	if (vec_append(&word, "\0") == 0)
		return (0);
	lctx->final = (t_token){TOKEN_STRING, word.data};
	return (1);
}

int	lexer_case_token_double_quote(t_lexer_context *lctx, t_env env)
{
	size_t			j;

	j = 0;
	while (lctx->str[j] != '\0' && lctx->str[j] != '\"')
		j++;
	if (lctx->str[j] == '\0')
	{
		ft_putstr_fd("Minishell: error unclosed double quote\n", STDERR_FILENO);
		return (0);
	}
	if (lexer_token_double_quote_loop(lctx, &j, env) == 0)
		return (0);
	if (tokens_append(lctx->tokens, &lctx->final) == 0)
	{
		token_free(&lctx->final);
		return (0);
	}
	lctx->info.len = j + 1;
	return (1);
}
