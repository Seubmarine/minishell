/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:02:16 by tbousque          #+#    #+#             */
/*   Updated: 2022/08/14 19:14:15 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	ft_isalpha(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

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

t_vec	lexer(char const *str)
{
	t_vec			v;
	size_t			i;
	static size_t	j = 0;

	v = vec_new(sizeof(t_token), 16, NULL);
	i = 0;
	while (str[i])
	{
		while (str[i] && str[i] == ' ')
			i++;
		if (!strncmp(str + i, "*", 1))
			put_token(&v, TOKEN_TYPE_ASTERISK, &i, 1);
		else if (!strncmp(str + i, "||", 2))
			put_token(&v, TOKEN_TYPE_OR, &i, 2);
		else if (!strncmp(str + i, "|", 1))
			put_token(&v, TOKEN_TYPE_PIPE, &i, 1);
		else
		{
			j = 0;
			while (str[i + j] && ft_isalpha(str[i + j]))
				j++;
			put_token(&v, TOKEN_TYPE_STRING, &i, j);
		}
		i++;
	}
	put_token(&v, TOKEN_TYPE_END, &i, 1);
	return (v);
}
