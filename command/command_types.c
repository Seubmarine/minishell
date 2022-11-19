#include "command.h"

int	ft_is_buitin(t_ast_command *ast_cmd)
{
	const char	*builtins_str[] = {"env", "export", "cd", "echo", \
	"exit", "pwd", "unset"};
	char		*cmd;
	size_t		i;

	i = 0;
	if (ast_cmd->args.len == 0)
		return (0);
	cmd = ((t_token *)vec_get(&ast_cmd->args, 0))->word;
	if (cmd == NULL)
		return (0);
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
	int			exit_status;
	t_command	cmd;
	int			fd_stdin;
	int			fd_stdout;

	command_init(&cmd, *ast_cmd);
	fd_stdin = dup(STDIN_FILENO);
	fd_stdout = dup(STDIN_FILENO);
	// if error
	exit_status = ft_exec_command(&cmd, env);
	if (fd_stdin != STDIN_FILENO)
	{
		dup2(fd_stdin, STDIN_FILENO);
		// if error
		close(fd_stdin);
	}
	if (fd_stdout != STDOUT_FILENO)
	{
		dup2(fd_stdout, STDOUT_FILENO);
		// if error
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