/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:20:14 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/04 20:21:07 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

//return -1 on error, 0 when command is not a buitin and 1 on success
int	builtin(char **argv, t_env *env)
{
	size_t	command_size;

	command_size = strlen(argv[0]);
	if (argv == NULL || argv[0] == NULL)
		return (-1);
	if (strncmp(argv[0], "export", command_size) == 0)
		return (builtin_export(argv, env));
	else if (strncmp(argv[0], "unset", command_size) == 0)
		return (builtin_unset(argv, env));
	else if (strncmp(argv[0], "env", command_size) == 0)
		return (builtin_env(argv, env));
	else if (strncmp(argv[0], "pwd", command_size) == 0)
		return (builtin_pwd());
	return (0);
}