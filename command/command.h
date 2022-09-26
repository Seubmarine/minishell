/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 18:03:48 by tbousque          #+#    #+#             */
/*   Updated: 2022/09/26 03:31:46 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include "lexer.h"
# include "ast.h"
# include "environement_variable.h"
# include "path_finder.h"

typedef struct s_command
{
	char	*path;
	char	**arguments;
	int		stdin;
	int		stdout;
}	t_command;

t_command	command_init(const char *src_token, t_token *tokens, size_t token_size);
int			command_set_stdin(t_command *cmd);
int			command_set_stdout(t_command *cmd);

void		ast_run_command(t_ast *ast, const char *src_token, t_env *env);
#endif