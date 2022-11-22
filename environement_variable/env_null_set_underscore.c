/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_null_set_underscore.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 19:45:32 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/22 20:09:58 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environement_variable.h"

int	env_init_null_set_undescore(t_env *env, char *argv)
{
	char	*name;
	size_t	i;

	i = ft_strlen(argv);
	while (i)
	{
		if (argv[i] == '/')
			break ;
		i--;
	}
	env_set_var(env, "_", "error");
	if (argv[i] == '/')
	{
		name = ft_strdup(&argv[i + 1]);
		if (name != NULL)
			env_set_var(env, "_", name);
		else
			return (0);
		free(name);
	}
	return (1);
}
