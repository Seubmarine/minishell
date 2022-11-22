/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_null.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 15:13:41 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/22 19:45:10 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environement_variable.h"
#include "built_in.h"
#include <fcntl.h>
#include "heredoc.h"
#include <stdio.h>

void	ft_getcwd_error(void)
{
	perror("Minishell: getcwd");
	exit (1);
}

void	ft_vecnew_error(void)
{
	perror("Minishell: vec_new");
	exit (1);
}

void	ft_env_set_random_error(t_env *env)
{
	perror("Minishell: env_set_random_str");
	env_free(env);
	exit (1);
}

void	ft_set_last_status(t_env *env)
{
	env->_last_status_str = malloc(sizeof(char) * ENV_LAST_STATUS_SIZE);
	if (env->_last_status_str == NULL)
	{
		env_free(env);
		exit(1);
	}
	env_set_last_status(env, 0);
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
	env_init_null_set_undescore(&env, argv);
	if (env_set_random_str(&env) == 0)
		ft_env_set_random_error(&env);
	ft_set_last_status(&env);
	return (env);
}
