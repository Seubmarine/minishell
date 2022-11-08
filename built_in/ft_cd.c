#include "built_in.h"

int ft_is_flag(char *arg)
{
    char *str;
    char *sentence;

    str = NULL;
    sentence = NULL;
    if ((arg[0] != '-') || (arg[0] == '-' && ft_strlen(arg) == 1))
        return(0);
    str = malloc(sizeof(char) * 3);
    ft_strlcpy(str, arg, 3);
    sentence = ft_strjoin("cd: ", str);
    free(str);
    str = ft_strjoin(sentence, "invalid flag\n");
    free(sentence);
    ft_putstr_fd(str, 2);
    free(str);
    return (1);
}

int ft_nb_arg(char **argv)
{
    if (ft_strlen_l(argv) == 2)
        return (0);
    ft_putstr_fd("cd: too many arguments\n", 2);
    return (1);
}

void    ft_create_oldpwd(t_env *c_env, char *str)
{
    t_env *new;
    t_env *read;

    read = *c_env;
    new = malloc(sizeof(t_env) * 1);
    new->name = "OLDPWD";
    new->value = NULL;
    while (read->next != NULL)
        read = read->next;
    read->next = new;
    read = *c_env;
    while (read && (ft_strncmp(read->name, "PWD", 3) != 0))
        read = read->next;
    new->value = str;
    free(str);
}

void    ft_change_env(t_env *c_env)
{
    char *str;
    t_env   *res;

    str = NULL;
    current = NULL;
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

t_env   *ft_get_home(t_env *c_env)
{
    t_env *res;

    res = *c_env;
    while (res && (ft_strncmp(res->name, "HOME", 4) != 0))
        res = res->next;
    return (res);
}

int    ft_change_dir(char *arg, t_env **c_env)
{
    if (chdir(arg) != 0)
        return (ft_putstr_fd("error chdir\n", 2), 1);
    ft_change_env(c_env);
}

int    ft_cd(char **argv, t_env *env)
{
	char	*sentence;
    t_env   file;

	sentence = NULL;
	if (ft_strlen_l(argv) == 1)
	{
        file = ft_get_home(c_env);
        if (file == NULL)
            return (ft_putstr_fd("cd : HOME not set\n", 2), 1);
		if (chdir(file->value) != 0)
            return (ft_putstr_fd("Error chdir\n", 2), 1);
        ft_change_env(c_env);
		return (0);
	}
    if ((ft_is_flag(argv) == 1) || (ft_nb_arg(argv) == 1)
        return (1);
    if (ft_change_dir(argv[1], c_env) != 0)
        return (1);
    return (0);
}