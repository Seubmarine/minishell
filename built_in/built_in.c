/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:20:14 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/02 16:41:18 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

//return -1 on error, 0 when command is not a buitin and 1 on success
int	builtin(char **argv, t_env *env)
{
	if (argv == NULL || argv[0] == NULL)
		return (-1);
	if (strncmp(argv[0], "export", 6) == 0)
		return (builtin_export(argv, env));
	else if (strncmp(argv[0], "unset", 6) == 0)
		return (builtin_unset(argv, env));
	return (0);
}