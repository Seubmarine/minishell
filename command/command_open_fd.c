#include "command.h"

int	preparing_fd_pipe(t_pidpes *pidpes, t_ast *ast)
{
	pidpes->pipes[READ_END] = STDIN_FILENO;
	pidpes->pipes[WRITE_END] = STDOUT_FILENO;
	if (pidpes->i < ast->pipeline.len - 1)
	{
		if (pipe(pidpes->pipes) == -1)
		{
			return (perror("Minishell: pipe: "), 1);
			free(pidpes->pids);
			return (1);
		}
	}
	return (0);
}

void	ft_open_input(int *fdin, char *filename)
{
	if (*fdin != STDIN_FILENO)
		close(*fdin);
	*fdin = open(filename, O_RDONLY);
	if (*fdin == -1)
		perror("Minishell: opening redirection type input");
}

void	ft_open_output(int *fdout, t_redirection redir)
{
	if (*fdout != STDOUT_FILENO)
		close(*fdout);
	if (redir.type == REDIRECTION_OUTPUT)
	{
		*fdout = open(redir.filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (*fdout == -1)
			perror("Minishell: opening redirection type output");
	}
	if (redir.type == REDIRECTION_OUTPUT_APPEND)
	{
		*fdout = open(redir.filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (*fdout == -1)
			perror("Minishell: opening redirection type output append");
	}
}

//return 0 on error 1 on success
int	ft_open_fd_child(t_command *cmd)
{
	if (ft_analyse_fd(cmd) == 0)
		return (0);
	if (cmd->fdin != STDIN_FILENO)
	{
		if (dup2(cmd->fdin, STDIN_FILENO) == -1)
			perror("Minishell: dup2 stdin");
		if (close(cmd->fdin) == -1)
			perror("Minishell: closing original of dup2 stdin");
	}
	if (cmd->fdout != STDOUT_FILENO)
	{
		if (dup2(cmd->fdout, STDOUT_FILENO) == -1)
			perror("Minishell: dup2 stdout");
		if (close(cmd->fdout) == -1)
			perror("Minishell: closing original of dup2 stdout");
	}
	return (1);
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
		else if (redir.type == REDIRECTION_OUTPUT || \
		redir.type == REDIRECTION_OUTPUT_APPEND)
			ft_open_output(&(cmd->fdout), redir);
		if (cmd->fdin == -1 || cmd->fdout == -1)
			return (perror("Minishell: open: "), 0);
		i++;
	}
	return (1);
}
