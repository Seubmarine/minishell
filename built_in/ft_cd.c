/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 18:04:49 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/10 18:57:36 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	ft_is_flag_cd_nb_arg(char **arg)
{
	if (arg[1][0] == '-')
	{
		if ((access(arg[1], F_OK) == 0) && (ft_strlen_l(arg) == 2))
			return (0);
		ft_putstr_fd("Minishell: cd: no flag allowed\n", 2);
		return (1);
	}
	if (ft_strlen_l(arg) == 2)
		return (0);
	ft_putstr_fd("cd: too many arguments\n", 2);
	return (1);
}

int	ft_change_env(t_env *env)
{
	char	*str;

	str = env_get_var(*env, "PWD");
	if (str == NULL)
		return (ft_putstr_fd("Error : env_get_var\n", 2), 1);
	env_set_var(env, "OLDPWD", str);
	str = getcwd(NULL, 0);
	if (str)
	{
		env_set_var(env, "PWD", str);
		free(str);
	}
	return (0);
}

int	ft_overpass_permission(char	*file, char *arg, t_env *env)
{
	int		i;
	char	*new_cwd;

	new_cwd = NULL;
	i = ft_strlen(file);
	while ((i > 0) && (ft_strncmp(&file[i], "/", 1) != 0))
		i--;
	new_cwd = malloc(sizeof(char) * i + 1);
	if (new_cwd == NULL)
	{
		ft_putstr_fd("Minishell: malloc: ", 2);
		return (perror(arg), 1);
	}
	ft_strlcpy(new_cwd, file, i + 1);
	if (chdir(new_cwd) != 0)
	{
		free(new_cwd);
		ft_putstr_fd("Minishell: cd: ", 2);
		return (perror(arg), 1);
	}
	ft_change_env(env);
	free(new_cwd);
	return (0);
}

int	ft_change_dir(char *arg, t_env *env)
{
	char	*file;

	file = NULL;
	if (chdir(arg) != 0)
	{
		if (ft_strncmp(arg, "..", 2) == 0)
		{
			file = env_get_var(*env, "PWD");
			if (file == NULL)
				return (perror("Minishell: cd: "), 1);
			if ((access(file, F_OK) == 0) && (access(file, X_OK) != 0))
				return (ft_overpass_permission(file, arg, env));
		}
		ft_putstr_fd("Minishell: cd: ", 2);
		return (perror(arg), 1);
	}
	ft_change_env(env);
	return (0);
}

int	ft_cd(char **argv, t_env *env)
{
	char	*file;

	if (ft_strlen_l(argv) == 1)
	{
		file = env_get_var(*env, "HOME");
		if (file == NULL)
			return (ft_putstr_fd("cd: HOME not set\n", 2), 1);
		if (chdir(file) != 0)
			return (perror(argv[0]), 1);
		ft_change_env(env);
		return (0);
	}
	if (ft_is_flag_cd_nb_arg(argv) == 1)
		return (1);
	if (ft_change_dir(argv[1], env) != 0)
		return (1);
	return (0);
}
