#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

// A IGNORER DESCENDRE A 243, libft pour le traitement et pas besoin de compiler avec tous les fichiers

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned long	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((i < n - 1) && (s1[i] == s2[i]) && (s1[i] && s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*s1;
	unsigned int	i;

	if (s == NULL)
		return (NULL);
	i = 0;
	if ((ft_strlen(s) == 0) || (ft_strlen(s) < start))
	{
		s1 = malloc(sizeof(char) * 1);
		s1[0] = '\0';
		return (s1);
	}
	if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	s1 = malloc(sizeof(char) * (len + 1));
	if (s1 == NULL)
		return (NULL);
	while ((i < len) && (s[start + i] != '\0'))
	{
		s1[i] = s[start + i];
		i++;
	}
	s1[i] = '\0';
	return (s1);
}


static void	ft_freetab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static int	ft_wordcount(char const	*s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && s[i])
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (count);
}

static char	**ft_intab(char const *s, char c, int count, char **tab)
{
	int	i;
	int	tabcount;
	int	start;

	i = -1;
	tabcount = 0;
	start = 0;
	while (tabcount < count && s[++i])
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] != c && s[i])
				i++;
			tab[tabcount] = ft_substr((s + start), 0, (i - start));
			if (tab[tabcount] == NULL)
			{
				ft_freetab(tab);
				return (NULL);
			}
			tabcount++;
		}
	}
	tab[tabcount] = NULL;
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		count;

	if (s == NULL)
		return (NULL);
	count = ft_wordcount(s, c);
	tab = malloc(sizeof(char *) * (count + 1));
	if (tab == NULL)
		return (NULL);
	tab = ft_intab(s, c, count, tab);
	return (tab);
}

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

int	ft_atoi(const char *str)
{
	long long	res;
	int			sign;
	int			i;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] && ((str[i] >= '\t' && str[i] <= '\r')
			|| str[i] == ' '))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = sign * -1;
		i++;
	}
	while ((str[i] >= '0' && str[i] <= '9') && res < 3000000000)
		res = res * 10 + (str[i++] - '0');
	res = res * sign;
	if (res < -2147483648)
		return (0);
	else if (res > 2147483647)
		return (-1);
	else
		return ((int)res);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	unsigned long	i;

	i = 0;
	if (dstsize != 0)
	{
		while (i < dstsize - 1 && src[i] != '\0')
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	i = 0;
	while (src[i])
		i++;
	return (i);
}


size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	unsigned int	dsts;
	unsigned int	srcs;
	unsigned int	i;
	unsigned int	j;

	dsts = ft_strlen(dst);
	srcs = ft_strlen(src);
	i = 0;
	j = dsts;
	if (dstsize <= j)
		return (dstsize + srcs);
	while (src[i] && (i < dstsize - j - 1))
	{
		dst[dsts] = src[i];
		dsts ++;
		i++;
	}
	dst[dsts] = '\0';
	return (j + srcs);
}


char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*dest;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	if (i + j == 0)
	{
		dest = malloc(sizeof(char) * 1);
		dest[0] = '\0';
		return (dest);
	}
	dest = malloc(sizeof(char) * (i + j + 1));
	if (dest == NULL)
		return (NULL);
	i = ft_strlcpy(dest, s1, i + 1);
	j = ft_strlcat(&dest[i], s2, j + 1);
	return (dest);
}

// FIN A IGNORER

int	ft_strlen_l(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
	return (i);
}

int	ft_is_flag(char *arg)
{
	int i;

	i = 0;
	if (arg[0] != '-')
		return (0);
	while (arg[++i])
	{
		if (arg[i] != 'n')
			return (0);
	}
	return (1);
}

char	*ft_add_next_word(char *sentence, char *arg)
{
	char	*s1;
	char	*s2;
	
	s1 = NULL;
	s2 = NULL;
	//if ft_is_variable
	if (sentence == NULL)
	{
		s1 = malloc(sizeof(char) * (ft_strlen(arg) + 1));
		ft_strlcpy(s1, arg, ft_strlen(arg) + 1);
		sentence = ft_strjoin(s1, " ");
		free (s1);
		return (sentence);
	}
	s1 = ft_strjoin(sentence, arg);
	free(sentence);
	s2 = ft_strjoin(s1, " ");
	free (s1);
	return (s2);
}

void	ft_echo(char *str)
{
	char	**arg;
	char	*sentence;
	int		j;
	int		flag;

	j = -1;
	flag = 0;
	sentence = NULL;
	if (ft_strlen(str) == 4)
	{
		write (1, "\n", 1);
		return ;
	}
	arg = ft_split(&str[4], ' ');
	if (arg == NULL)
		return (ft_putstr_fd("Error split\n", 2));
	if ((flag = ft_is_flag(arg[0])) == 1)
		j++;
	if (ft_strlen_l(arg) > 1)
	{
		while (arg[++j])
			sentence = ft_add_next_word(sentence, arg[j]);
		write(1, sentence, ft_strlen(sentence));
	}
	if (flag != 1)
			write(1, "\n", 1);
	free (sentence);
}

int	main()
{
	char *str;

	while (1)
	{
		str = readline("> ");
		if (ft_strncmp(str, "echo", 4) == 0)
			ft_echo(str);
		free(str);
	}
}