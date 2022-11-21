#include "heredoc.h"

char	*heredoc_naming(int heredoc_number, char *random_str)
{
	char	filename[HEREDOC_FILENAME_MAX_LEN] = HEREDOC_NAMING;
	char	number_str[HEREDOC_NUMBER_LEN];
	char	*new_filename;

	if (!ft_ullto_buf(heredoc_number, number_str, HEREDOC_NUMBER_LEN))
		return (NULL);
	if (ft_strlcat(filename, number_str, sizeof(filename)) >= sizeof(filename))
		return (NULL);
	if (ft_strlcat(filename, ".", sizeof(filename)) >= sizeof(filename))
		return (NULL);
	if (ft_strlcat(filename, random_str, sizeof(filename)) >= sizeof(filename))
		return (NULL);
	new_filename = ft_strdup(filename);
	return (new_filename);
}

int	heredoc_open_fd(t_hd_fd *hd_fds, char *filename)
{
	hd_fds->fdin_dup = dup(STDIN_FILENO);
	if (hd_fds->fdin_dup == -1)
	{
		perror("Minishell: dup");
		free(filename);
		return (1);
	}
	hd_fds->heredoc_fd = open(filename, O_WRONLY | O_EXCL | O_CREAT, \
	S_IWUSR | S_IROTH | S_IRUSR | S_IRGRP);
	if (hd_fds->heredoc_fd == -1)
	{
		perror("Minishell: heredoc open");
		free(filename);
		return (1);
	}
	return (0);
}

int	heredoc_null_line(t_hd_fd *hd_fds, char *filename)
{
	if (fcntl(STDIN_FILENO, F_GETFD) == -1) // TODO FONCTION INTERDITE A CHANGER
	{
		printf("\n");
		dup2(hd_fds->fdin_dup, STDIN_FILENO);//TODO check error
		close(hd_fds->heredoc_fd);
		unlink(filename);
		free(filename);
		return (1);
	}
	write(STDERR_FILENO, "Warning heredoc delimited by end of file\n", \
	41);
	return (0);
}
