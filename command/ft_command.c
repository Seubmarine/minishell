/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 17:07:41 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/13 18:16:06 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

void	ft_open_input(int *fd, char *filename)
{
	if (fd[0] != -2)
		close(fd[0]);
	fd[0] = open(filename, O_RDONLY);
	// if (fd[0] == -1)
	// 	ft_error_open(fd, NULL, NULL);
}

void	ft_open_output(int *fd, t_redirection redir)
{
	if (fd[1] != -2)
		close(fd[1]);
	if (redir.type == REDIRECTION_OUTPUT)
		fd[1] = open(redir.filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (redir.type == REDIRECTION_OUTPUT_APPEND)
		fd[1] = open(redir.filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	// if (fd[1] == -1)
	// 	ft_error_open(fd, NULL, NULL);
}

void	ft_analyse_fd(int *fd, t_command cmd)
{
	size_t			i;
	t_redirection	redir;

	i = 0;
	while (i < cmd.redirections_len)
	{
		redir = cmd.redirections[i];
		if (redir.type == REDIRECTION_INPUT)
			ft_open_input(fd, redir.filename);
		else if (redir.type == REDIRECTION_OUTPUT || redir.type == REDIRECTION_OUTPUT_APPEND)
			ft_open_output(fd, redir);
		i++;
	}
}

void	ft_open_fd_child(int *fd, int *c_fd, t_command cmd)
{
	ft_analyse_fd(fd, cmd);
	if (fd[0] == -2)
		fd[0] = open("/dev/stdin", O_RDONLY);
	if (fd[1] == -2)
		fd[1] = open("/dev/stdout", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if ((fd[0] == -1) || (fd[1] == -1))
	{
		perror("minishell child open: /dev/stdout or /dev/stdin");
		//ft_error_open
		exit(EXIT_FAILURE);
	}
	c_fd[0] = dup2(fd[0], STDIN_FILENO);
	c_fd[1] = dup2(fd[1], STDOUT_FILENO);
	printf("fd[0] = %d fd[1] = %d\n", fd[0], fd[1]);
	close(fd[0]);
	close(fd[1]);
	// if ((c_fd[0] == -1) || (c_fd[1] == -1))
		//ft_error_open(fd, c_fd, NULL, NULL);
}

void	ft_init_exec_command(int *fd, int *c_fd)
{
	fd[0] = -2;
	fd[1] = -2;
	c_fd[0] = 0;
	c_fd[1] = 0;
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

int	ft_exec_command(t_command *cmd, t_env *env)
{
	int		fd[2];
	int		c_fd[2];
	char	*bin_path;
	char	**envp;

	bin_path = find_exec(cmd->path, env_get_var(*env, "PATH"));
	if (bin_path != NULL)
	{
		free(cmd->path);
		cmd->path = bin_path;
	}
	ft_command_debug(*cmd);
	ft_init_exec_command(fd, c_fd);
	ft_open_fd_child(fd, c_fd, *cmd);
	if (cmd->path == NULL)
		return (0);
	envp = env_to_envp(*env);
	execve(cmd->path, cmd->arguments, envp);
	envp_free(envp);
	return (127);
}

int	ft_simple_command(t_ast_command *ast_command, t_env *env)
{
	int			status;
	pid_t		pid;
	t_command	command;

	status = 0;
	signal_handling_child();
	command_init(&command, ast_command[0]);
	if (builtin(command.arguments, env, &status))
		return (status);
	pid = fork();
	if (pid == 0)
	{
		env->is_child = 1;
		// if (command_init(&command, ast_command[0]))
			status = ft_exec_command(&command, env);
		command_free(&command);
	}
	else
	{
		if (waitpid(pid, &status, 0) != -1)
			status = WEXITSTATUS(status);
	}
	signal_handling();
	return (status);
}

int	ft_which_command(t_ast *ast, t_env *env)
{
	int				exit_status;
	t_ast_command	*last_cmd;
	char			*last_args;

	exit_status = 0;
	if (ast->pipeline.len == 1)
		exit_status = ft_simple_command(vec_get(&ast->pipeline, 0), env);
	// else
	//  	exit_status = ft_multi_command(ast, env);
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
