/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 18:03:32 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/12 16:28:03 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

// char *token_to_str(const char *src, t_token token)
// {
// 	size_t str_size = token.span.end - token.span.begin + 1;
// 	char *str = malloc((str_size + 1) * sizeof(*str));
// 	memcpy(str, src + token.span.begin, str_size);
// 	str[str_size] = '\0';
	
// 	return (str);
// }

enum e_redirection_type token_to_redirection_type(enum e_token_type type)
{
	if (type == TOKEN_REDIRECT_INPUT)
		return (REDIRECTION_INPUT);
	else if (type == TOKEN_REDIRECT_OUTPUT)
		return (REDIRECTION_OUTPUT);
	else if (type == TOKEN_REDIRECT_OUTPUT_APPEND)
		return (REDIRECTION_OUTPUT_APPEND);
	else
		return (REDIRECTION_INVALID);
}

//return 0 on success 1 on error
int	command_redirection_init(t_command *cmd, t_ast_command ast)
{
	size_t	i;
	t_ast_redirection *ast_redirec;

	cmd->redirections = malloc(sizeof(*cmd->redirections) * ast.redirection.len);
	if (cmd->redirections == NULL)
		return (1);
	cmd->redirections_len = ast.redirection.len;
	i = 0;
	while (i < ast.redirection.len)
	{
		ast_redirec = vec_get(&ast.redirection, i);
		if (ast_redirec->rhs.type != TOKEN_STRING)
			return (1);
		cmd->redirections[i].filename = ast_redirec->rhs.word;
		cmd->redirections[i].type = token_to_redirection_type(ast_redirec->token.type);
		i++;
	}
	return (0);
}

t_command	command_init(t_ast_command ast_command)
{
	t_command 	cmd;
	size_t		i;

	i = 0;
	cmd.arguments = malloc(sizeof(*cmd.arguments) * (ast_command.args.len + 1)); //TODO check error
	while (i < ast_command.args.len)
	{
		cmd.arguments[i] = ((t_token *)vec_get(&ast_command.args, i))->word;
		i++;
	}
	cmd.arguments[i] = NULL;
	cmd.redirections = malloc(sizeof(*cmd.redirections) * ast_command.redirection.len); //TODO check error
	command_redirection_init(&cmd, ast_command);
	cmd.path = strdup(cmd.arguments[0]); //TODO: check error + use ft_strndup
	cmd.pid = -1;
	return (cmd);
}

void command_free(t_command *command)
{
	free(command->arguments);
	free(command->path);
}

/*
//set the redirection of a command accordingly to the the redirection array in order and it's size
#include <fcntl.h>
void command_set_redirection(t_command *command, t_ast_redirection *redirection, size_t redirection_size)
{
	size_t	i;

	i = 0;
	while (i < redirection_size)
	{
		if (redirection[i].token.type == TOKEN_REDIRECT_OUTPUT)
		{
			char *filename = redirection->rhs.word;
			if (command->stdout != STDOUT_FILENO) //if a redirection existed before close the previous file, not sure if it's the correct behavior
				close(command->stdout);
			command->stdout = open(filename, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
			if (command->stdout == -1)
				perror("minishell: "); // TODO: check error with name of file
		}
		else if (redirection[i].token.type == TOKEN_REDIRECT_INPUT)
		{
			char *filename = redirection->rhs.word;
			if (command->stdin != STDIN_FILENO)
				close(command->stdin);
			command->stdin = open(filename, O_RDONLY);
			if (command->stdin == -1)
				perror("minishell: "); // TODO: check error with name of file
		}
		i++;
	}
}

int command_run(t_env *env, t_command command)
{
	int	exit_status = 42;

	printf("Process %i.\nCommand %s\nstdin: %i\nstdout: %i\n\n", getpid(), command.path, command.stdin, command.stdout);
	dup2(command.stdin, STDIN_FILENO);
	dup2(command.stdout, STDOUT_FILENO);
	if (command.stdin != STDIN_FILENO)
		close(command.stdin);
	if (command.stdout != STDOUT_FILENO)
		close(command.stdout);
	if (builtin(command.arguments, env, &exit_status))
		return (exit_status);
	char *real_path = find_exec(command.path, env_get_var(*env, "PATH"));
	if (real_path)
		command.path = real_path;
	command.arguments[0] = command.path;
	struct stat info;
	if (stat(command.path, &info) != 0)
	{
		perror("command doesn't exist");
		exit_status = 127;
	}
	else
	{
		char **envp_child = env_to_envp(*env);
		execve(command.path, command.arguments, envp_child);
		perror("execve");
		envp_free(envp_child);
		exit_status = 126;
	}
	if (real_path)
		free(real_path);
	return (exit_status);
}
*/
//TODO: stderr of command need to be redirected to the stdin of the minishell

/*
Create the commands from the ast tree and pipes them, then set the redirection, fork the program
open the filedescriptor in command.stdin and command.stdout and replace the real stdin and stout by them
in the forked process then execve.

Return 1 if it's a child
*/
/*
int	ast_run_command(t_ast *ast, t_env *env)
{
	size_t			i;
	t_ast_command	*ast_command;
	t_command		*commands = malloc(sizeof(*commands) * ast->pipeline.len);
	int				exit_status = 0;

	//Create all commands with redirection
	i = 0;
	while (i < ast->pipeline.len)
	{
		ast_command = vec_get(&ast->pipeline, i);
		commands[i] = command_init(ast_command->args.data, ast_command->args.len);
		i++;
	}
	int pipes[2];
	i = 0;
	while ((ssize_t)i < (ssize_t)(ast->pipeline.len - 1))
	{
		pipe(pipes);
		commands[i].stdout = pipes[1];
		commands[i + 1].stdin = pipes[0];
		i++;
	}
	i = 0;
	while (i < ast->pipeline.len)
	{
		ast_command = vec_get(&ast->pipeline, i);
		size_t j = 0;
		while (j < ast_command->redirection.len)
		{
			t_ast_redirection *redir = vec_get(&ast_command->redirection, j);
			command_set_redirection(&commands[i], redir, ast_command->redirection.len);
			j++;	
		}
		i++;
	}
	if (ast->pipeline.len == 1 && builtin(commands[0].arguments, env, &exit_status))
	{
		env_set_last_status(env, exit_status);
		exit_status = 0;
	}
	else
	{
		i = 0;
		while (i < ast->pipeline.len)
		{	
			t_command child_command = commands[i];
			pid_t pid = fork();
			if (pid == 0) //is child
			{
				exit_status = command_run(env, child_command);
				break ;
			}
			else
				commands[i].pid = pid;
			if (child_command.stdout != STDOUT_FILENO)
				close(child_command.stdout);
			i++;
		}
	}
	i = 0;
	while (i < ast->pipeline.len)
	{
		if (commands[i].stdin != STDIN_FILENO)
			close(commands[i].stdin);
		if (commands[i].stdout != STDOUT_FILENO)
			close(commands[i].stdout);
		i++;
	}
	int status = 0;
	pid_t wpid;
	while ((wpid = waitpid(-1, &status, 0)) != -1) //TODO: Exit status of recent command is here $?
	{
		printf("Process %d terminated.\n\n", wpid);
		if (commands[ast->pipeline.len - 1].pid == wpid)
			env_set_last_status(env, (WEXITSTATUS(status)));
	}
	char **last_args = commands[ast->pipeline.len - 1].arguments;
	i = 0;
	while (last_args[i])
		i++;
	env_set_var(env, "_", last_args[i - 1]);
	i = 0;
	while (i < ast->pipeline.len)
	{
		free(commands[i].arguments);
		if (commands[i].stdin != STDIN_FILENO)
			close(commands[i].stdin);
		if (commands[i].stdout != STDOUT_FILENO)
			close(commands[i].stdout);
		i++;
	}
	free(commands);
	return (exit_status);
}
*/