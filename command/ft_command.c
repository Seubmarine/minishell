/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 17:07:41 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/19 22:43:10 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

// const char	*REDIRECTION_DEFUG[] = {
// 	"REDIRECTION_OUTPUT",
// 	"REDIRECTION_OUTPUT_APPEND",
// 	"REDIRECTION_INPUT",
// 	"REDIRECTION_INVALID"
// };

// void	ft_command_debug(t_command cmd)
// {
// 	size_t			i;
// 	t_redirection	r;

// 	i = 0;
// 	if (cmd.path == NULL)
// 		printf("cmd : NULL\n");
// 	else
// 		printf("cmd : %s\n", cmd.path);
// 	printf("fdin = %i, fdout = %i\n", cmd.fdin, cmd.fdout);
// 	printf("args = {\n");
// 	while (cmd.arguments[i])
// 	{
// 		printf("\targs[%zu] = \"%s\"\n", i, cmd.arguments[i]);
// 		i++;
// 	}
// 	printf("}\n");
// 	printf("redirection = {\n");
// 	i = 0;
// 	while (i < cmd.redirections_len)
// 	{
// 		r = cmd.redirections[i];
// 		printf("\t[%zu] type = %s, filename = 
		// \"%s\"\n", i, REDIRECTION_DEFUG[r.type], r.filename);
// 		i++;
// 	}
// 	printf("}\n");
// }

//put "ft_command_debug" at he beginning to debug

//find the path of a command if needed, open redirection and launch the command
int	ft_exec_command(t_command *cmd, t_env *env)
{
	char	*bin_path;
	char	**envp;
	int		exit_status;

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
	if (execve(cmd->path, cmd->arguments, envp) == -1)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(cmd->arguments[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	envp_free(envp);
	return (127);
}

void	manage_child(t_env *env, t_command *cmd, t_pidpes *pidpes, \
t_ast_command *ast_command)
{
	pidpes->pids[pidpes->i] = fork();
	if (pidpes->pids[pidpes->i] == -1)
		return (perror("Minishell: fork: "));
	if (pidpes->pids[pidpes->i] == 0)
	{
		env->is_child = 1;
		if (pidpes->pipes[READ_END] != STDIN_FILENO)
			close(pidpes->pipes[READ_END]);
		command_init(cmd, ast_command[pidpes->i]);
		cmd->fdin = pidpes->fdin;
		cmd->fdout = pidpes->pipes[WRITE_END];
		pidpes->exit_status = ft_exec_command(cmd, env);
		command_free(cmd);
	}
}

void	manage_parent(t_pidpes *pidpes)
{
	if (pidpes->fdin != STDIN_FILENO)
		fd_close_reset(&(pidpes)->fdin);
	if (pidpes->pipes[WRITE_END] != STDOUT_FILENO)
		fd_close_reset(&(pidpes)->pipes[WRITE_END]);
	pidpes->fdin = pidpes->pipes[READ_END];
	pidpes->i += 1;
}

void	command_init_int(t_pidpes *pidpes)
{
	pidpes->fdin = STDIN_FILENO;
	pidpes->i = 0;
	pidpes->exit_status = 0;
}

int	execute_command(t_ast *ast, t_env *env)
{
	t_pidpes		pidpes;
	t_ast_command	*ast_command;
	t_command		cmd;

	command_init_int(&pidpes);
	ast_command = vec_get(&ast->pipeline, 0);
	pidpes.pids = malloc(sizeof(pidpes.pids) * ast->pipeline.len);
	if (pidpes.pids == NULL)
		return (perror("Minishell: pids malloc: "), 1);
	signal_handling_child();
	while (pidpes.i < ast->pipeline.len)
	{
		if (preparing_fd_pipe(&pidpes, ast) == 0)
			break;
		manage_child(env, &cmd, &pidpes, ast_command);
		if (pidpes.pids[pidpes.i] == 0 || pidpes.pids[pidpes.i] == -1)
			break ;
		manage_parent(&pidpes);
	}
	if (env->is_child == 0)
		pidpes.exit_status = waiting_childs(env, ast, &pidpes);
	signal_handling();
	free(pidpes.pids);
	return (pidpes.exit_status);
}
