/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 16:36:24 by tbousque          #+#    #+#             */
/*   Updated: 2022/09/11 23:26:13 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H
# include "lexer.h"

typedef struct s_ast
{
	t_vec	pipeline;
}	t_ast;

typedef struct s_ast_redirection
{
	t_token	token;
	t_token	rhs;
}	t_ast_redirection;

typedef struct s_ast_command
{
	t_vec	args;
	t_vec	redirection;
}	t_ast_command;

t_ast	*ast_init(t_token *tok, size_t tok_size);
void	ast_free(t_ast *ast);

#endif