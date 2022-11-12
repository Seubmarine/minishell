/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 18:03:48 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/11 21:33:53 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "lexer.h"
# include "ast.h"
# include "environement_variable.h"
# include "path_finder.h"
# include "built_in.h"

enum e_redirection_type
{
	REDIRECTION_OUTPUT,
	REDIRECTION_OUTPUT_APPEND,
	REDIRECTION_INPUT,
	REDIRECTION_INVALID
};

typedef struct s_redirection
{
	enum e_redirection_type type;
	char					*filename;
}	t_redirection;

typedef struct s_command
{
	char			*path;
	char			**arguments;
	t_redirection	*redirections;
	size_t			redirections_len;	
	pid_t			pid;
}	t_command;

t_command	command_init(t_ast_command ast_command);
void		command_free(t_command *command);
int			command_set_stdin(t_command *cmd);
int			command_set_stdout(t_command *cmd);

int			ast_run_command(t_ast *ast,  t_env *env);
int			ft_which_command(t_ast *ast, t_env *env);
#endif