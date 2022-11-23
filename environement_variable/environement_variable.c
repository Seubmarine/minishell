/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_variable.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 01:43:37 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/23 04:32:51 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environement_variable.h"
#include "built_in.h"
#include <stdio.h>

int	env_collect_from_envp(char **envp, char *argv, t_env *env)
{
	size_t			i;
	t_env_key_value	key_value;

	i = -1;
	while (envp[++i])
	{
		if (key_value_init(envp[i], &key_value) != 0)
		{
			if (vec_append(&env->v, &key_value) == 0)
			{
				env_key_value_free(&key_value);
				ft_putstr_fd("Minishell: vec_append: malloc error\n", 2);
				env_free(env);
				exit (1);
			}
		}
		else
		{
			env_key_value_free(&key_value);
			env_free(env);
			exit(1);
		}
	}
	ft_prepare_shl_shlvl(env, argv);
	return (1);
}

t_env	env_init_from_envp(const char *envp[], char *argv)
{
	t_env			env;
	size_t			i;

	i = 0;
	env._last_status_str = NULL;
	while (envp[i])
		i++;
	if (i <= 0)
		i = 1;
	if (env_set_random_str(&env) == 0)
		ft_env_set_random_error(&env);
	env.is_child = 0;
	env._last_status_str = malloc(sizeof(char) * ENV_LAST_STATUS_SIZE);
	if (env._last_status_str == NULL)
		env_last_status_error();
	env_set_last_status(&env, 0);
	env.v = vec_new(sizeof(t_env_key_value), i, \
	(void (*)(void *))env_key_value_free);
	if (env.v.data == NULL)
		env_vec_new_error(&env);
	env_collect_from_envp((char **)envp, argv, &env);
	return (env);
}

void	env_to_envp_error(char **envp, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(envp[i]);
		envp[i] = NULL;
		i++;
	}
	free(envp);
}

char	**env_to_envp(t_env env)
{
	char			**envp;
	size_t			i;
	t_env_key_value	kv;

	envp = malloc(sizeof(*envp) * (env.v.len + 1));
	if (envp == NULL)
		return (perror("Minishell: malloc error: env_to_envp"), NULL);
	i = -1;
	while (++i < env.v.len)
	{
		kv = *(t_env_key_value *)vec_get(&env.v, i);
		if (kv.key == NULL || kv.value == NULL)
			return (env_to_envp_error(envp, i), NULL);
		envp[i] = env_key_value_to_string(kv);
		if (envp[i] == NULL)
			break ;
	}
	envp[i] = NULL;
	return (envp);
}
