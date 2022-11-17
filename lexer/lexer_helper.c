/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 03:36:34 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/17 03:43:02 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
		if (ft_strncmp(parse_helper[i].value, str, strlen(parse_helper[i].value)) \
		== 0)
			return ((t_token_info){parse_helper[i].type, \
			ft_strlen(parse_helper[i].value)});
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
		"TOKEN_HERE_DOCUMENT",
		"TOKEN_HERE_DOCUMENT_OPEN",
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
			//if NULL
			tmp[0] = '\0';
			strcpy(tmp, last->word); // creer strcpy
			strcat(tmp, current->word); // creer strcat
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