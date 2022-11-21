/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 19:39:24 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/15 18:19:52 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

t_ast_command	ast_command_init(void)
{
	t_ast_command	command;

	command.args = vec_new(sizeof(t_token), 2, NULL);
	command.redirection = vec_new(sizeof(t_ast_redirection), 2, NULL);
	// if arg ou redir == NULL
	return (command);
}

t_ast_redirection	redirection_init(t_token type, t_token rhs)
{
	t_ast_redirection	redirection;

	redirection.token = type;
	redirection.rhs = rhs;
	return (redirection);
}

int	is_redirection_token(t_token *tok, size_t i)
{
	if (tok[i].type == TOKEN_REDIRECT_INPUT || tok[i].type == \
		TOKEN_REDIRECT_OUTPUT || tok[i].type == TOKEN_REDIRECT_OUTPUT_APPEND \
		|| tok[i].type == TOKEN_HERE_DOCUMENT)
		return (0);
	return (1);
}

void	token_string(t_ast_command *command, t_token *tok, t_ast *ast, size_t i)
{
	command = vec_get(&ast->pipeline, ast->pipeline.len - 1);
	vec_append(&command->args, &tok[i]);
}

int	ast_push(t_ast *ast, t_token *tok)
{
	size_t				i;
	t_ast_command		*command;
	t_ast_redirection	new_redirection;
	t_ast_command		cmd;

	i = 0;
	command = NULL;
	while (tok[i].type != TOKEN_END)
	{
		if (is_redirection_token(tok, i) == 0)
		{
			new_redirection = redirection_init(tok[i], tok[i + 1]);
			command = vec_get(&ast->pipeline, ast->pipeline.len - 1);
			if (vec_append(&command->redirection, &new_redirection) == 0)
				return (0);
			i += 1;
		}
		else if (tok[i].type == TOKEN_STRING)
			token_string(command, tok, ast, i);
		else if (tok[i].type == TOKEN_PIPE)
		{
			cmd = ast_command_init(); // TODO error malloc
			if (cmd.args.data == NULL || cmd.redirection.data == NULL)
			{
				ast_command_free(&cmd);
				return (0);
			}
			if (vec_append(&ast->pipeline, &cmd) == 0)
				return (0);
		}
		i++;
	}
	return (1);
}
