#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

// A IGNORER DESCENDRE A 51

int	ft_strcmp(char *s1, char *s2)  // Te moques pas, c'est le strcmp de ma piscine
{
	int	i;
	int	a;

	i = 0;
	a = 0;
	while (s1[i] == s2[i] && (s1[i] != '\0'))
	{
		i++;
	}
	if (s1[i] > s2[i])
	{
		a = s1[i] - s2[i];
		return (a);
	}
	else
	{
		a = s1[i] - s2[i];
		return (a);
	}
	return (a);
}

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

// FIN A IGNORER

int	ft_heredoc(char *str)
{
	int		fd;
	char	*line;

	fd = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)		
		return (ft_putstr_fd("Error open\n", 2), 1);
	while  (1)
	{
		line = readline("> ");
		if (ft_strcmp(line, str) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (0);
}

int	main()
{
	char *str;

	while (1)
	{
		str = readline("Heredoc  ");
		ft_heredoc(str);
		unlink(".heredoc");
	}
}