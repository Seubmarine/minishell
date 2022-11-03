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

int	ft_exit_error(char *str, int code)
{
	char	*message;
	char	*buff;
	
	buff = NULL;
	message = NULL;
	if (code == 1)
		return (ft_putstr_fd("exit: too many arguments\n", 2), 1); // NE PAS REMPLACER PAR EXIT
	buff = ft_strjoin("exit:", str);
	if (buff == NULL)
		return (ft_putstr_fd("Error malloc\n", 2), 2);
	message = ft_strjoin(buff, " : numeric arguement only\n");
	free (buff);
	if (message == NULL)
		return (ft_putstr_fd("Error malloc\n", 2), 2);
	ft_putstr_fd(message, 2);
	free(message);
	return (2); // remplacer par exit
}

int	ft_only_num(char *arg)
{
	int i;

	i = 0;
	if (arg[0] == '-')
	{
		if (ft_strlen(arg) == 1)
			return (1);
		else
			i++;
	}
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	ft_exit(char *str)
{
	int i;
	int errnum;
	char **arg;

	i = 0;
	errnum = 256;
	arg = NULL;
	if (ft_strlen(str) == 4)
		return (0);
	arg = ft_split(&str[4], ' ');
	if (ft_only_num(arg[0]) != 0)
		return (ft_exit_error(arg[0], 2)); // retirer le return
	while (arg[i])
		i++;
	if (i > 1)
		return (ft_exit_error(NULL, 1));
	i = ft_atoi(arg[0]);
	i = i % 256;
	errnum += i;
	errnum = errnum % 256;
	return (errnum); // Remplacer par exit
}

int	main()
{
	char *str;
	int	errnum = 0;
	while (1)
	{
		str = readline(">");
		if (strncmp(str, "exit", 4) == 0)
			errnum = ft_exit(str);
		printf("Error code : %d\n", errnum); // ATTENTION : SI TROP D ARGUMENT (exit_code 1) ET QUE PREMIER ARGUMENT EST VALIDE, LE PARENT NE QUITTE PAS. 
		free(str);
	}
}