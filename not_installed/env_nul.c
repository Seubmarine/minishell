#include "ft_collect.h"

// A IGNORER DESCENDRE A 90
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

// FIN de A IGNORER, a mettre dans minishell

void	ft_collect_env_null(char *argv, t_env **c_env)
{
	t_env	*current;
	char	str[700];	// Si tu sais comment faire une valeur indetermine sans overflow, n'hésites pas a me le dire

	current = ft_new_env(c_env);
	getcwd(str, sizeof(str));
	current->name = "PWD";
	current->value = str;
	current = ft_new_env(c_env);
	current->name = "SHLVL";
	current->value = "1";
	current = ft_new_env(c_env);
	current->name = "_";
	current->value = argv;
}

int	main(int argc, char **argv, char **env)
{
	t_env	*c_env;
	c_env = NULL;
	(void)argc;
	// Partie a replacer dans minishell
	if (*env == NULL)
		ft_collect_env_null(argv[0], &c_env);
	else
	{
		(void)argv;
		ft_collect_env(env, &c_env);
	}
	// Fin de partie
	while (c_env != NULL)
	{
		printf("%s=%s\n", c_env->name, c_env->value);
		c_env = c_env->next;
	}
	return (0);
}