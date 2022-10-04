/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 18:03:32 by tbousque          #+#    #+#             */
/*   Updated: 2022/10/04 01:32:11 by tbousque         ###   ########.fr       */
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

/*
first we create all the command and connect them with pipe one after an other
the command 'ls -R .. > file_out' is represented as:
command.arguments = ["ls", "-R", ".."];
command.path = arguments[0];
command.stdin = STDIN_FILENO;
command.stdout = open("file_out");
*/

t_command	command_init(t_token *tokens, size_t token_size)
{
	t_command 	cmd;
	size_t		i;

	cmd.arguments = malloc(sizeof(*cmd.arguments) * (token_size + 1));
	if (!cmd.arguments)
		exit(EXIT_FAILURE); //TODO: Catch error
	i = 0;
	while (i < token_size)
	{
		cmd.arguments[i] = tokens[i].word;
		i++;
	}
	cmd.arguments[i] = NULL;
	cmd.path = cmd.arguments[0];
	cmd.stdin = STDIN_FILENO;
	cmd.stdout = STDOUT_FILENO;
	return (cmd);
}

void command_free(t_command *command)
{
	size_t i = 0;
	while (command->arguments[i])
	{
		if (command->stdin != STDIN_FILENO)
			close(command->stdin);
		if (command->stdout != STDOUT_FILENO)
			close(command->stdout);
		i++;
	}
	free(command->arguments);
}

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
		}
		i++;
	}
}

//TODO: stderr of command need to be redirected to the stdin of the minishell

/*
Create the commands from the ast tree and pipes them, then set the redirection, fork the program
open the filedescriptor in command.stdin and command.stdout and replace the real stdin and stout by them
in the forked process then execve.
*/
void	ast_run_command(t_ast *ast, t_env *env)
{
	size_t			i;
	t_ast_command	*ast_command;
	t_command		*commands = malloc(sizeof(*commands) * ast->pipeline.len);

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
	i = 0;
	while (i < ast->pipeline.len)
	{	
		t_command child_command = commands[i];
		pid_t pid = fork();
		if (pid == 0) //is child
		{
			printf("Process %i.\nCommand %s\nstdin: %i\nstdout: %i\n\n", getpid(), child_command.path, child_command.stdin, child_command.stdout);
			dup2(child_command.stdin, STDIN_FILENO);
			dup2(child_command.stdout, STDOUT_FILENO);
			if (child_command.stdin != STDIN_FILENO)
				close(child_command.stdin);
			if (child_command.stdout != STDOUT_FILENO)
				close(child_command.stdout);
			char *real_path = find_exec(child_command.path, env_get_var(*env, "PATH"));
			struct stat info;
			if (real_path)
			{
				free(child_command.path);
				child_command.path = real_path;
			}
			child_command.arguments[0] = child_command.path;
			if (stat(child_command.path, &info) != 0)
				perror("command doesn't exist");
			else
			{
				char **envp_child = env_to_envp(*env);
				execve(child_command.path, child_command.arguments, envp_child);
				perror("execve");
				envp_free(envp_child);
			}
			i = 0;
			while (i < ast->pipeline.len)
			{
				command_free(&commands[i]);
				i++;
			}
			free(commands);
			return ;
		}
		if (child_command.stdout != STDOUT_FILENO)
			close(child_command.stdout);
		i++;
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
		printf("Process %d terminated.\n\n", wpid);
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
}
