#include "ft_collect.h"
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

// A IGNORER, DESCENDRE A 326

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

char	*ft_strdup(const char *s1)
{
	int		i;
	int		j;
	char	*dest;

	i = 0;
	j = 0;
	while (s1[j])
		j++;
	if (j == 0)
	{
		dest = malloc(sizeof(char) * 1);
		dest[0] = '\0';
		return (dest);
	}
	dest = malloc(sizeof(char) * j + 1);
	if (dest == NULL)
		return (NULL);
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
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

t_env	*ft_new_env(t_env **c_env)
{
	t_env	*new;
	t_env	*current;

	current = *c_env;
	if ((new = malloc(sizeof(t_env))) == NULL)
		return (ft_putstr_fd("Errror malloc\n", 2), NULL);
	new->name = NULL;
	new->value = NULL;
	new->next = NULL;
	if (*c_env == NULL)
		*c_env = new;
	else
	{
		while (current->next != NULL)
			current = current->next;		
		current->next = new;
	}
	return (new);
}

void	ft_collect_env(char **env, t_env **c_env)
{
	int		i;
	int 	j;
	t_env	*current;

	i = 0;
	j = 0;
	while (env[i])
	{
		j = 0;
		current = ft_new_env(c_env);
		while (env[i][j] != '=')
			j++;
		current->name = malloc(sizeof(char) * j + 1);
		if (current->name == NULL)
			return (ft_putstr_fd("Error malloc\n", 2));
		ft_strlcpy(current->name, env[i], (j + 1));
		j++;
		current->value = malloc(sizeof(char) * ((ft_strlen(env[i]) + 1) - j));
		if (current->value == NULL)
			return (ft_putstr_fd("Error malloc", 2));
		ft_strlcpy(current->value, &env[i][j], ((ft_strlen(env[i]) + 1) - j));
		i++;
	}
}

void	ft_collect_env_null(char *argv, t_env **c_env)
{
	t_env	*current;
	char	*str;	// Si tu sais comment faire une valeur indetermine sans overflow, n'hésites pas a me le dire

	current = ft_new_env(c_env);
	str = getcwd(NULL, 0);
	current->name = "PWD";
	current->value = str;
	current = ft_new_env(c_env);
	current->name = "SHLVL";
	current->value = "1";
	current = ft_new_env(c_env);
	current->name = "_";
	current->value = argv;
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

void    ft_check_dir_change(t_env **c_env)
{
    pid_t   pid;
    t_env   *path;
    char    **str;
    char    *ls[2];
    path = *c_env;
    ls[0] = "ls";
    ls[1]= NULL;
    while (path && (ft_strncmp(path->name, "PATH", 4) != 0))
        path = path->next;
    str = ft_split(path->value, ':');
    pid = fork();
    if (pid == 0)
        execve("/usr/bin/ls", ls, str);
    wait(NULL);
}


// FIN de A IGNORER, a mettre dans minishell

int	ft_strlen_l(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
    if (i > 1)
    {
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (i);
	}
	return (i);
}

int ft_is_flag(char *arg)
{
    char *str;
    char *sentence;
    if ((arg[0] != '-') || (arg[0] == '-' && ft_strlen(arg) == 1))
        return(0);
    str = malloc(sizeof(char) * 3);
    ft_strlcpy(str, arg, 3);
    sentence = ft_strjoin("cd: ", str);
    free(str);
    str = ft_strjoin(sentence, ": invalid flag\n");
    free(sentence);
    ft_putstr_fd(str, 2);
    free(str);
    return (1);
}

void    ft_create_oldpwd(t_env **c_env, char *str)
{
    t_env *new;
    t_env *read;

    read = *c_env;
    new = malloc(sizeof(t_env) * 1);
    new->name = "OLDPWD";
    new->value = NULL;
	new->next = NULL;
    while (read->next != NULL)
        read = read->next;
    read->next = new;
    read = *c_env;
    while (read && (ft_strncmp(read->name, "PWD", 3) != 0))
        read = read->next;
    new->value = ft_strdup(str);
	free(str);
}

void    ft_change_env(t_env **c_env)
{
    char *str;
    t_env   *res;

    str = NULL;
    res = *c_env;
    while (res && (ft_strncmp(res->name, "PWD", 3) != 0))
        res = res->next;
    str = ft_strdup(res->value);
    free(res->value);
    res->value = getcwd(NULL, 0);
    res = *c_env;
    while (res && (ft_strncmp(res->name, "OLDPWD", 6) != 0))
        res = res->next;
    if (res == NULL)
        ft_create_oldpwd(c_env, str);
    else
    {
		res->value = ft_strdup(str);
		free(str);
	}
}

t_env   *ft_get_home(t_env **c_env)
{
    t_env *res;

    res = *c_env;
    while (res && (ft_strncmp(res->name, "HOME", 4) != 0))
        res = res->next;
    return (res);
}

void    ft_change_dir(char *arg, t_env **c_env)
{
    if (chdir(arg) != 0)
	{
        ft_putstr_fd("error chdir\n", 2);
		return ;
	}
	ft_change_env(c_env);
}

void    ft_cd(char *str, t_env **c_env)
{
    char	**arg;
    t_env   *file;

	if (ft_strlen(str) == 2)
	{
        file = ft_get_home(c_env);
        if (file == NULL)
        {
            ft_putstr_fd("cd : HOME not set\n", 2);
            return ;
        }
		if (chdir(file->value) != 0)
        {
            ft_putstr_fd("Error chdir\n", 2);
            return ;
        }
        ft_change_env(c_env);
		return ;
	}
	arg = ft_split(&str[2], ' ');
	if (arg == NULL)
		return (ft_putstr_fd("Error split\n", 2));
    if ((ft_is_flag(arg[0]) == 1) || (ft_strlen_l(arg) > 1))
        return ;
    ft_change_dir(arg[0], c_env);
}

int	main(int argc, char **argv, char **env)
{
	t_env	*c_env;
    t_env   *read;
	c_env = NULL;
	char	*str;
	int	nul = 0;
	(void)argc;
	if (*env == NULL)
	{
		ft_collect_env_null(argv[0], &c_env);
		nul = 1;
	}
	else
	{
		(void)argv;
		ft_collect_env(env, &c_env);
	}
	while (1)
	{
        read = c_env;
		str = readline("> ");
		if (ft_strncmp(str, "cd", 2) == 0)
			ft_cd(str, &c_env);
        while (read)
        {
            if ((ft_strncmp(read->name, "PWD", 3) == 0) || (ft_strncmp(read->name, "OLDPWD", 6) == 0))
            {
				printf("%s=%s\n", read->name, read->value);
			}
			read = read->next;
		}
        if (nul == 0)
			ft_check_dir_change(&c_env);
		free(str);
		printf("\n\n");
	}
	return (0);
}