/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_single_quote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:06:09 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/22 16:06:21 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
	if (lctx->final.word == NULL || \
	tokens_append(lctx->tokens, &lctx->final) == 0)
	{
		token_free(&lctx->final);
		return (0);
	}
	return (1);
}
