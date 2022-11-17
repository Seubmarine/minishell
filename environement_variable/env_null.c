/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_null.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 15:13:41 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/13 15:13:42 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environement_variable.h"
#include "built_in.h"
#include <fcntl.h>
#include "heredoc.h"
#include <stdio.h>

void	ft_getcwd_error(void)
{
	perror("Minishell: getcwd: ");
	exit (1);
}

void	ft_vecnew_error(void)
{
	perror("Minishell: vec_new: ");
	exit (1);
}

t_env	env_init_null(char *argv)
{
	t_env	env;
	char	*str;

	str = getcwd(NULL, 0);
	if (str == NULL)
		ft_getcwd_error();
	env.is_child = 0;
	env.v = vec_new(sizeof(t_env_key_value), 3, NULL);
	if (env.v.data == NULL)
		ft_vecnew_error();
	env_set_var(&env, "PWD", str);
	env_set_var(&env, "SHLVL", "1");
	env_set_var(&env, "_", argv);
	if (env_set_random_str(&env) == 0)
	{
		perror("Minishell: env_set_random_str:");//TODO: check error
		env_free(&env);
		exit (1);
	}
	env._last_status_str = malloc(sizeof(ENV_LAST_STATUS_SIZE));
	env_set_last_status(&env, 0);
	return (env);
}
