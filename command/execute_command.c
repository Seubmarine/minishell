/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 21:24:28 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/22 22:23:54 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "command.h"

int	prepare_arguments(char *line, t_env *env, t_vec *tokens)
{
	if (lexer(line, *env, tokens) == 0)
	{
		vec_free(tokens);
		return (0);
	}
	if (tokens->len <= 1 || \
	(((t_token *)tokens->data)[tokens->len - 1].type) != TOKEN_END)
	{
		vec_free(tokens);
		return (0);
	}
	if (lexer_validate(tokens->data, tokens->len) == 0)
	{
		ft_putstr_fd("Minishell: invalid syntax\n", STDERR_FILENO);
		env_set_last_status(env, 2);
		return (0);
	}
	return (1);
}

int	execute_line(char *line, t_env *env)
{
	t_vec	tokens;
	t_ast	ast;
	int		exit_status;

	exit_status = 1;
	if (prepare_arguments(line, env, &tokens) == 0)
		return (exit_status);
	if (ast_init(tokens.data, tokens.len, &ast) == 0)
	{
		ast_free(&ast);
		vec_free(&tokens);
		return (exit_status);
	}
	if (ast_open_heredocs(&ast, env) == 1)
		exit_status = ft_which_command(&ast, env);
	else
		exit_status = 130;
	ast_close_heredocs(&ast);
	ast_free(&ast);
	vec_free(&tokens);
	return (exit_status);
}
