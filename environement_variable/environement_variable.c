/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_variable.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 01:43:37 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/15 07:34:21 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environement_variable.h"
#include "built_in.h"
#include <stdio.h>

void	env_vec_new_error(t_env *env)
{
	perror("Minishell: vec_new: ");
	env_free(env);
	exit (1);
}

void	env_last_satus_error(t_env *env)
{
	perror("Minishell: malloc: ");
	env_free(env);
	exit (1);
}

t_env	env_collect_from_envp(char **envp, char *argv, t_env env, \
t_env_key_value *key_value)
{
	size_t	i;

	i = 0;
	while (envp[i])
	{
		*key_value = key_value_init(envp[i]);
		vec_append(&env.v, key_value);
		i++;
	}
	ft_prepare_shl_shlvl(&env, argv);
	return (env);
}

t_env	env_init_from_envp(const char *envp[], char *argv)
{
	t_env			env;
	size_t			i;
	t_env_key_value	key_value;

	i = 0;
	while (envp[i])
		i++;
	if (i <= 0)
		i = 1;
	if (env_set_random_str(&env) == 0)
		ft_env_set_random_error(&env);
	env.is_child = 0;
	env._last_status_str = malloc(sizeof(char) * ENV_LAST_STATUS_SIZE); //TODO: check malloc error
	if (env._last_status_str == NULL)
		env_last_satus_error(&env);
	env_set_last_status(&env, 0);
	env.v = vec_new(sizeof(t_env_key_value), i, \
	(void (*)(void *))env_key_value_free);
	if (env.v.data == NULL)
		env_vec_new_error(&env);
	env = env_collect_from_envp((char **)envp, argv, env, &key_value);
	return (env);
}

char	**env_to_envp(t_env env)
{
	char			**envp;
	size_t			i;
	t_env_key_value	kv;

	envp = malloc(sizeof(*envp) * (env.v.len + 1));
	// if NULL
	i = 0;
	while (i < env.v.len)
	{
		kv = *(t_env_key_value *)vec_get(&env.v, i);
		envp[i] = env_key_value_to_string(kv);
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

// #include <stdio.h>
// int main(int argc, char const *argv[], const char *envp[])
// {
// 	t_env	env = env_init_from_envp(envp);
// 	size_t	env_len;
// 	size_t	i;
// 	t_env_key_value *env_array = env_get_vars(env, &env_len);

// 	i = 0;
// 	while (i < env_len)
// 	{
// 		printf("%s=%s\n", env_array[i].key, env_array[i].value);
// 		i++;
// 	}
// 	return (EXIT_SUCCESS);
// }

// int main(int argc, char const *argv[], const char *envp[])
// {
// 	// char *key_values[] = {"KEY=value", "SHELL=minishell", "PATH=/bin:/usr/bin", "MOMMY=sophie"};
// 	t_env env = env_init_from_envp(envp);
// 	char *str;

// 	env_set_var(&env, "PATH", "/tmp/bin");
// 	env_remove_var(&env, "PATH");
// 	str = env_get_var(env, "PATH");
// 	if (str)
// 		printf("%s\n", str);
// 	env_free(&env);
// 	return (0);
// }
