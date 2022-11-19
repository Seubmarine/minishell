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
