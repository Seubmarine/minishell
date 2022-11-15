/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 17:07:41 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/15 19:20:23 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

void	ft_open_input(int *fdin, char *filename)
{
	if (*fdin != STDIN_FILENO)
		close(*fdin);
	*fdin = open(filename, O_RDONLY);
	// ft_error_open(fd, NULL, NULL);
}

void	ft_open_output(int *fdout, t_redirection redir)
{
	if (*fdout != STDOUT_FILENO)
		close(*fdout);
	if (redir.type == REDIRECTION_OUTPUT)
		*fdout = open(redir.filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (redir.type == REDIRECTION_OUTPUT_APPEND)
		*fdout = open(redir.filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	// if (fd[1] == -1)
	// 	ft_error_open(fd, NULL, NULL);
}

//return 0 on error 1 on success
int	ft_analyse_fd(t_command *cmd)
{
	size_t			i;
	t_redirection	redir;

	i = 0;
	while (i < cmd->redirections_len)
	{
		redir = cmd->redirections[i];
		if (redir.type == REDIRECTION_INPUT)
			ft_open_input(&(cmd->fdin), redir.filename);
		else if (redir.type == REDIRECTION_OUTPUT || redir.type == REDIRECTION_OUTPUT_APPEND)
			ft_open_output(&(cmd->fdout), redir);
		if (cmd->fdin == -1 || cmd->fdout == -1)
			return(perror("Minishell"), 0);
		i++;
	}
	return (1);
}

//return 0 on error 1 on success
int	ft_open_fd_child(t_command *cmd)
{
	if (ft_analyse_fd(cmd) == 0)
		return (0);
	// if ((fd[0] == -1) || (fd[1] == -1))
	// {
	// 	perror("minishell child open: /dev/stdout or /dev/stdin");
	// 	//ft_error_open
	// 	exit(EXIT_FAILURE);
	// }
	if (cmd->fdin != STDIN_FILENO)
	{
		printf("dup2 and close fdin %i\n", cmd->fdin);
		dup2(cmd->fdin, STDIN_FILENO); //check error
		close(cmd->fdin); // check error
	}
	if (cmd->fdout != STDOUT_FILENO)
	{
		printf("dup2 and close fdout %i\n", cmd->fdout);
		dup2(cmd->fdout, STDOUT_FILENO); //check error
		close(cmd->fdout); // check error
	}
	return (1);
}

const char	*REDIRECTION_DEFUG[] = {
	"REDIRECTION_OUTPUT",
	"REDIRECTION_OUTPUT_APPEND",
	"REDIRECTION_INPUT",
	"REDIRECTION_INVALID"
};

void	ft_command_debug(t_command cmd)
{
	size_t			i;
	t_redirection	r;

	i = 0;
	if (cmd.path == NULL)
		printf("cmd : NULL\n");
	else
		printf("cmd : %s\n", cmd.path);
	printf("fdin = %i, fdout = %i\n", cmd.fdin, cmd.fdout);
	printf("args = {\n");
	while (cmd.arguments[i])
	{
		printf("\targs[%zu] = \"%s\"\n", i, cmd.arguments[i]);
		i++;
	}
	printf("}\n");
	printf("redirection = {\n");
	i = 0;
	while (i < cmd.redirections_len)
	{
		r = cmd.redirections[i];
		printf("\t[%zu] type = %s, filename = \"%s\"\n", i, REDIRECTION_DEFUG[r.type], r.filename);
		i++;
	}
	printf("}\n");
}

#define READ_END 0
#define WRITE_END 1

void	fd_close_reset(int *fd)
{
	if (close(*fd) != 0)
		printf("error closing fd %i\n", *fd);
	*fd = -2;
}

void close_stdfd(void)
{
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}

int	ft_exec_command(t_command *cmd, t_env *env)
{
	char	*bin_path;
	char	**envp;
	int		exit_status;

	ft_command_debug(*cmd);
	if (ft_open_fd_child(cmd) == 0)
		return (1);
	if (builtin(cmd->arguments, env, &exit_status))
		return (exit_status);
	bin_path = find_exec(cmd->path, env_get_var(*env, "PATH"));
	if (bin_path != NULL)
	{
		free(cmd->path);
		cmd->path = bin_path;
	}
	if (cmd->path == NULL)
		return (0);
	envp = env_to_envp(*env);
	execve(cmd->path, cmd->arguments, envp);
	envp_free(envp);
	return (127);
}

int execute_command(t_ast *ast, t_env *env)
{
	int pipes[2];
	int fdin = STDIN_FILENO;
	int	exit_status = 0;
	t_ast_command *ast_command = vec_get(&ast->pipeline, 0);
	t_command cmd;
	pid_t	*pids = malloc(sizeof(*pids) * ast->pipeline.len);
	size_t	i = 0;

	while (i < ast->pipeline.len)
	{
		pipes[READ_END] = STDIN_FILENO;
		pipes[WRITE_END] = STDOUT_FILENO;
		if (i < ast->pipeline.len - 1)
			pipe(pipes);
		pids[i] = fork();
		if (pids[i] == 0) //is child
		{
			env->is_child = 1;
			if (pipes[READ_END] != STDIN_FILENO)
				close(pipes[READ_END]);
			command_init(&cmd, ast_command[i]);
			cmd.fdin = fdin;
			cmd.fdout = pipes[WRITE_END];
			exit_status = ft_exec_command(&cmd, env);
			command_free(&cmd);
			break ;
		}
		// is parent
		if (fdin != STDIN_FILENO)
			fd_close_reset(&fdin);
		if (pipes[WRITE_END] != STDOUT_FILENO)
			fd_close_reset(&pipes[WRITE_END]);
		fdin = pipes[READ_END];
		i++;
	}
	i = 0;
	while (env->is_child == 0 && i < ast->pipeline.len)
	{
		int wstatus;
		waitpid(pids[i], &wstatus, 0);
		exit_status = WEXITSTATUS(wstatus);
		i++;
	}
	free(pids);
	return (exit_status);
}

int	ft_is_buitin(t_ast_command *ast_cmd)
{
	char *cmd;

	if (ast_cmd->args.len == 0)
		return (0);
	cmd = ((t_token *)vec_get(&ast_cmd->args, 0))->word;	
	if (cmd == NULL)
		return (0);
	const char *builtins_str[] = {"env", "export", "cd", "echo", "exit", "pwd", "unset"};
	size_t	i = 0;
	while (i < sizeof(builtins_str) / sizeof(builtins_str[0]))
	{
		if (ft_strncmp(cmd, builtins_str[i], ft_strlen(cmd)) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	builtin_no_pipe(t_ast_command *ast_cmd, t_env *env)
{
	int	exit_status;

	t_command cmd;
	command_init(&cmd, *ast_cmd);
	int fd_stdin = dup(STDIN_FILENO); 
	int fd_stdout = dup(STDIN_FILENO); 
	exit_status = ft_exec_command(&cmd, env);
	if (fd_stdin != STDIN_FILENO)
	{
		dup2(fd_stdin, STDIN_FILENO);
		close(fd_stdin);
	}
	if (fd_stdout != STDOUT_FILENO)
	{
		dup2(fd_stdout, STDOUT_FILENO);
		close(fd_stdout);
	}
	command_free(&cmd);
	return (exit_status);
}

int	ft_which_command(t_ast *ast, t_env *env)
{
	int				exit_status;
	t_ast_command	*ast_cmd;
	t_ast_command	*last_cmd;
	char			*last_args;

	if (ast->pipeline.len == 0)
		return (0);
	ast_cmd = vec_get(&ast->pipeline, 0);
	if (ast->pipeline.len == 1 && ft_is_buitin(ast_cmd))
		exit_status = builtin_no_pipe(ast_cmd, env);
	else
		exit_status = execute_command(ast, env);
	env_set_last_status(env, exit_status);
	last_cmd = vec_get(&ast->pipeline, ast->pipeline.len - 1);
	last_args = "";
	if (last_cmd->args.len > 0)
	{
		last_args = ((t_token *)vec_get(&last_cmd->args, \
		last_cmd->args.len - 1))->word;
	}
	env_set_var(env, "_", last_args);
	return (exit_status);
}
