/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:20:14 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/08 01:42:59 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "libft.h"

//return 1 if it was a builtin and put the command result in exit_status
int	builtin(char **argv, t_env *env, int *exit_status)
{
	if (argv == NULL || argv[0] == NULL)
		return (-1);
	if (ft_strcmp(argv[0], "export") == 0)
		*exit_status = builtin_export(argv, env);
	else if (ft_strcmp(argv[0], "unset") == 0)
		*exit_status = builtin_unset(argv, env);
	else if (ft_strcmp(argv[0], "env") == 0)
		*exit_status = builtin_env(argv, env);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		*exit_status = builtin_pwd();
	else if (ft_strcmp(argv[0], "echo") == 0)
		*exit_status = ft_echo(argv);
	else if (ft_strcmp(argv[0], "exit") == 0)
		*exit_status = ft_exit(argv, env);
	else if (ft_strcmp(argv[0], "cd") == 0)
		*exit_status = ft_cd(argv, env);
	else
		return (0);
	return (1);
}
