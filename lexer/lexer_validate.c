/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_validate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 22:08:16 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/22 22:40:19 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	is_token_redirection(t_token tok)
{
	if (tok.type == TOKEN_REDIRECT_INPUT || tok.type == \
		TOKEN_REDIRECT_OUTPUT || tok.type == TOKEN_REDIRECT_OUTPUT_APPEND \
		|| tok.type == TOKEN_HERE_DOCUMENT)
		return (1);
	return (0);
}

int	lexer_validate(t_token *tokens, size_t tokens_len)
{
	size_t	i;
	t_token	current;
	t_token	next;

	if (tokens == NULL)
		return (0);
	i = 0;
	if (tokens[0].type == TOKEN_PIPE)
		return (0);
	while (tokens[i].type != TOKEN_END && i < tokens_len)
	{
		current = tokens[i];
		next = tokens[i + 1];
		if (current.type == TOKEN_STRING && current.word == NULL)
			return (0);
		else if (is_token_redirection(current) && next.type != TOKEN_STRING)
			return (0);
		else if (current.type == TOKEN_PIPE && next.type == TOKEN_END)
			return (0);
		else if (current.type == TOKEN_PIPE && next.type == TOKEN_PIPE)
			return (0);
		i++;
	}
	return (1);
}
