/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:03:31 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/22 22:20:14 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include <stdlib.h>
# include <stdio.h>
# include "vector.h"
# include "environement_variable.h"
# include "libft.h"

enum e_token_type
{
	TOKEN_STRING,
	TOKEN_PIPE,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_ASTERISK,
	TOKEN_REDIRECT_INPUT,
	TOKEN_REDIRECT_OUTPUT,
	TOKEN_REDIRECT_OUTPUT_APPEND,
	TOKEN_HERE_DOCUMENT,
	TOKEN_HERE_DOCUMENT_OPEN,
	TOKEN_DOLLAR,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_END,
	TOKEN_SPACE,
	TOKEN_UNKNOW,
};

typedef struct s_token
{
	enum e_token_type	type;
	char				*word;
}	t_token;

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

typedef struct s_lexer_context
{
	const char		*str;
	char			*env_value;
	t_vec			*tokens;
	t_token_info	info;
	t_token_info	next;
	t_token			final;
	int				should_expand;
}	t_lexer_context;

int				lexer_case_token_single_quote(t_lexer_context *lctx);
int				lexer_case_token_double_quote(t_lexer_context *lctx, t_env env);
int				lexer_case_token_dollar(t_lexer_context *lctx, t_env env);
char			*lexh_env(t_env env, char *env_key);
int				lexer(char *str, t_env env, t_vec *tokens);
void			lexer_debug(t_vec	tokens);
t_token_info	is_token(char const *str);
int				tokens_append(t_vec *tokens, t_token *current);
void			token_free(t_token *token);
int				lexer_validate(t_token *tokens, size_t tokens_len);

#endif