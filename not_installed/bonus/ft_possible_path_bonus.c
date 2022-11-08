/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_possible_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 15:06:37 by mportrai          #+#    #+#             */
/*   Updated: 2022/06/13 17:51:38 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	ft_freetab(char **tab)
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

char	**ft_prepare_cmd(char *argv, t_fderr *fd_errnum)
{
	char	**cmd;

	cmd = NULL;
	if (ft_strlen(argv) == 0)
		ft_error_no_argv(fd_errnum);
	cmd = ft_split(argv, ' ');
	if (cmd == NULL)
		ft_error_path(fd_errnum);
	return (cmd);
}

char	*ft_prepare_w_path(char **cmd, char *e_path)
{
	char	**a_path;

	a_path = ft_prepare_a_path(e_path);
	free(e_path);
	if (a_path == NULL)
		return (NULL);
	e_path = ft_correct_path(cmd[0], a_path);
	ft_freetab(a_path);
	if (e_path == NULL)
		return (NULL);
	return (e_path);
}

char	*ft_check_d_path(char **cmd, t_fderr *fd_errnum)
{
	char	*e_path;

	e_path = NULL;
	if (!cmd[0])
		return (NULL);
	if (ft_strncmp(cmd[0], "/", 1) == 0)
	{
		e_path = malloc(sizeof(char *) * (ft_strlen(cmd[0]) + 1));
		if (e_path == NULL)
			ft_error(cmd, fd_errnum);
		ft_strlcpy(e_path, cmd[0], ft_strlen(cmd[0]) + 1);
		if (e_path == NULL)
			ft_error(cmd, fd_errnum);
		if (access(e_path, F_OK) != 0)
			ft_error_d_cmd(e_path, cmd, fd_errnum);
		return (e_path);
	}
	else
		return (NULL);
}

char	*ft_prepare_no_path(char **cmd, t_fderr *fd_errnum)
{
	char	*e_path;

	e_path = malloc(sizeof(char *) * (ft_strlen(cmd[0]) + 1));
	if (e_path == NULL)
		ft_error(cmd, fd_errnum);
	ft_strlcpy(e_path, cmd[0], ft_strlen(cmd[0]) + 1);
	if (e_path == NULL)
		ft_error(cmd, fd_errnum);
	if ((access(e_path, F_OK) != 0) || (ft_strncmp(e_path, "/", 1) != 0))
		ft_error_cmd(e_path, cmd, fd_errnum);
	return (e_path);
}
