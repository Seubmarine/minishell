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

t_env	env_init_null(char *argv)
{
	t_env	env;
	char	*str;

	str = getcwd(NULL, 0);
	// if str == NULL
	env.is_child = 0;
	env.v = vec_new(sizeof(t_env_key_value), 3, NULL);
	env_set_var(&env, "PWD", str);
	env_set_var(&env, "SHLVL", "1");
	env_set_var(&env, "_", argv);
	if (env_set_random_str(&env) == 0)
		perror("Minishell: env set random str");//TODO: check error
	env._last_status_str = malloc(sizeof(ENV_LAST_STATUS_SIZE));
	env_set_last_status(&env, 0);
	return (env);
}
