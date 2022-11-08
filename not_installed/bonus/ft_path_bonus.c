/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 13:18:51 by mportrai          #+#    #+#             */
/*   Updated: 2022/06/13 17:52:15 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

char	**ft_path_join(char **a_path)
{
	char	**j_path;
	int		i;

	i = 0;
	while (a_path[i])
		i++;
	j_path = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (a_path[i])
	{
		j_path[i] = ft_strjoin(a_path[i], "/");
		if (j_path[i] == NULL)
		{
			ft_freetab(j_path);
			ft_freetab(a_path);
			return (NULL);
		}
		free(a_path[i]);
		i++;
	}
	j_path[i] = NULL;
	free(a_path);
	return (j_path);
}

char	*ft_correct_path(char *cmd, char **a_path)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	while (a_path[i] != NULL)
	{
		path = ft_strjoin(a_path[i], cmd);
		if (!path)
			return (NULL);
		if (access(path, F_OK) == 0)
		{
			return (path);
		}
		free(path);
		i++;
	}
	path = malloc(sizeof(char *) * 6);
	ft_strlcpy(path, "Error", 6);
	return (path);
}

char	*ft_prepare_e_path(char **envp)
{
	char	*e_path;
	int		i;
	int		check;

	i = -1;
	check = 1;
	while ((check != 0) && (envp[++i]))
		check = ft_strncmp(envp[i], "PATH=", 5);
	if (check == 0)
	{
		e_path = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
		if (e_path == NULL)
			return (NULL);
	}
	else
	{
		e_path = malloc(sizeof(char *) * 7);
		if (e_path == NULL)
			return (NULL);
		ft_strlcpy(e_path, "nopath", 7);
		if (e_path == NULL)
			return (NULL);
	}
	return (e_path);
}

char	**ft_prepare_a_path(char *e_path)
{
	char	**a_path;
	char	**j_path;

	j_path = NULL;
	a_path = ft_split(e_path, ':');
	if (a_path == NULL)
		return (NULL);
	j_path = ft_path_join(a_path);
	if (j_path == NULL)
		return (NULL);
	return (j_path);
}

char	*ft_prepare_path(char **cmd, char **envp, t_fderr *fd_errnum)
{
	char	*e_path;

	e_path = ft_check_d_path(cmd, fd_errnum);
	if (e_path != NULL)
		return (e_path);
	e_path = ft_prepare_e_path(envp);
	if (e_path == NULL)
		ft_error(cmd, fd_errnum);
	if (ft_strncmp(e_path, "nopath", 6) != 0)
	{
		e_path = ft_prepare_w_path(cmd, e_path);
		if (e_path == NULL)
			ft_error(cmd, fd_errnum);
	}
	else
	{
		free(e_path);
		e_path = ft_prepare_no_path(cmd, fd_errnum);
	}
	if (access(e_path, F_OK) != 0)
		ft_error_cmd(e_path, cmd, fd_errnum);
	return (e_path);
}
