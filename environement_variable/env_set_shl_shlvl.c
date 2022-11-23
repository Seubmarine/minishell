/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set_shl_shlvl.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 12:50:49 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/23 04:28:18 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environement_variable.h"
#include "built_in.h"
#include <stdio.h>
#include <fcntl.h>
#include <heredoc.h>

void	env_set_var_error(t_env *env, char *str)
{
	free(str);
	env_free(env);
	exit (1);
}

void	ft_prepare_shl_shlvl(t_env *env)
{
	int		lvl;
	char	*buff;

	buff = env_get_var(*env, "SHLVL");
	if (buff == NULL)
		lvl = 0;
	else
		lvl = ft_atoi(buff);
	buff = ft_itoa(lvl + 1);
	if (buff == NULL)
		ft_shl_shlvl_itoa_error(env);
	if (env_set_var(env, "SHLVL", buff) == 0)
		env_set_var_error(env, buff);
	free(buff);
}
