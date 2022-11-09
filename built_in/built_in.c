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

//return 1 if it was a builtin and put the command result in exit_status
int	builtin(char **argv, t_env *env, int *exit_status)
{
	size_t	command_size;

	command_size = strlen(argv[0]);
	if (argv == NULL || argv[0] == NULL)
		return (-1);
	if (strncmp(argv[0], "export", command_size) == 0)
		*exit_status = builtin_export(argv, env);
	else if (strncmp(argv[0], "unset", command_size) == 0)
		*exit_status = builtin_unset(argv, env);
	else if (strncmp(argv[0], "env", command_size) == 0)
		*exit_status = builtin_env(argv, env);
	else if (strncmp(argv[0], "pwd", command_size) == 0)
		*exit_status = builtin_pwd();
	else if (strncmp(argv[0], "echo", command_size) == 0)
		*exit_status = ft_echo(argv);
	else if (strncmp(argv[0], "exit", command_size) == 0)
		*exit_status = ft_exit(argv);
	// else if (strncmp(argv[0], "cd", command_size) == 0)
	// 	*exit_status = ft_cd(argv, env);
	else
		return (0);
	return (1);
}