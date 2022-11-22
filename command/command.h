/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 18:03:48 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/20 14:25:43 by tbousque         ###   ########.fr       */
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
# include "signaling.h"
# include "libft.h"
# include "heredoc.h"

// constant to use for accessing pipe
# define READ_END 0
# define WRITE_END 1

enum e_redirection_type
{
	REDIRECTION_OUTPUT,
	REDIRECTION_OUTPUT_APPEND,
	REDIRECTION_INPUT,
	REDIRECTION_INVALID
};

typedef struct s_redirection
{
	enum e_redirection_type	type;
	char					*filename;
}	t_redirection;

typedef struct s_command
{
	char			*path;
	char			**arguments;
	t_redirection	*redirections;
	size_t			redirections_len;
	int				fdin;
	int				fdout;
}	t_command;

typedef struct s_pidpes
{
	pid_t		*pids;
	int			pipes[2];
	size_t		i;
	int			fdin;
	int			exit_status;
}	t_pidpes;

int						execute_line(char *line, t_env *env);

//return 0 on error 1 on success
int						command_init(t_command *cmd, t_ast_command ast_command);
void					command_free(t_command *command);

enum e_redirection_type	token_to_redirection_type(enum e_token_type type);
int						command_set_stdin(t_command *cmd);
int						command_set_stdout(t_command *cmd);
int						ast_run_command(t_ast *ast, t_env *env);
int						builtin_no_pipe(t_ast_command *ast_cmd, t_env *env);
int						ft_is_builtin(t_ast_command *ast_cmd);
int						ft_which_command(t_ast *ast, t_env *env);
int						preparing_fd_pipe(t_pidpes *pidpes, t_ast *ast);
int						ft_exec_command(t_command *cmd, t_env *env);
int						execute_command(t_ast *ast, t_env *env);
int						waiting_childs(t_env *env, t_ast *ast, \
t_pidpes *pidpes);
int						ft_analyse_fd(t_command *cmd);
void					ft_open_input(int *fdin, char *filename);
void					ft_open_output(int *fdout, t_redirection redir);
int						ft_open_redirection(t_command *cmd);
void					close_stdfd(void);
void					fd_close_reset(int *fd);

#endif
