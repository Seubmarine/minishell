/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 18:03:32 by tbousque          #+#    #+#             */
/*   Updated: 2022/09/11 23:28:12 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

char *token_to_str(const char *src, t_token token)
{
	size_t str_size = token.span.end - token.span.begin + 1;
	char *str = malloc((str_size + 1) * sizeof(*str));
	memcpy(str, src + token.span.begin, str_size);
	str[str_size] = '\0';
	
	return (str);
}

t_command	command_init(const char *src_token, t_token *tokens, size_t token_size)
{
	t_command 	cmd;
	size_t		i;

	cmd.arguments = malloc(sizeof(*cmd.arguments) * (token_size + 1));
	if (!cmd.arguments)
		exit(EXIT_FAILURE); //TODO: Catch error
	i = 0;
	while (i < token_size)
	{
		cmd.arguments[i] = token_to_str(src_token, tokens[i]);
		i++;
	}
	cmd.arguments[i] = NULL;
	cmd.path = cmd.arguments[0];
	cmd.stdin = STDIN_FILENO;
	cmd.stdout = STDOUT_FILENO;
	return (cmd);
}

#include <fcntl.h>
void command_set_redirection(const char *src_token, t_command *command, t_ast_redirection *redirection, size_t redirection_size)
{
	size_t	i;

	i = 0;
	while (i < redirection_size)
	{
		if (redirection[i].token.type == TOKEN_REDIRECT_OUTPUT)
		{
			char *filename = token_to_str(src_token, redirection->rhs);
			command->stdout = open(filename, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
			free(filename);
		}
		i++;
	}
}

void	ast_run_command(t_ast *ast, const char *src_token)
{
	size_t			i;
	t_ast_command	*ast_command;
	t_command		*commands = malloc(sizeof(*commands) * ast->pipeline.len);

	//Create all commands with redirection
	i = 0;
	while (i < ast->pipeline.len)
	{
		ast_command = vec_get(&ast->pipeline, i);
		commands[i] = command_init(src_token, ast_command->args.data, ast_command->args.len);
		command_set_redirection(src_token, &commands[i], ast_command->redirection.data, ast_command->redirection.len);
		i++;
	}
	i = 0;
	while (i < ast->pipeline.len)
	{	
		pid_t pid = fork();
		if (pid == 0) //is child
		{
			t_command child_command = commands[i];
			printf("Command %s\nstdin: %i\nstdout: %i\n", child_command.path, child_command.stdin, child_command.stdout);
			dup2(child_command.stdin, STDIN_FILENO);
			dup2(child_command.stdout, STDOUT_FILENO);
			if (child_command.stdin != STDIN_FILENO)
				close(child_command.stdin);
			if (child_command.stdout != STDOUT_FILENO)
				close(child_command.stdout);
			char *const envp_child[1] = {NULL};
			execve(child_command.path, child_command.arguments, envp_child);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	wait(NULL);
	i = 0;
	while (i < ast->pipeline.len)
	{
		size_t j = 0;
		while (commands[i].arguments[j])
		{
			free(commands[i].arguments[j]);
			j++;
		}
		free(commands[i].arguments);
		i++;
	}
	free(commands);
}