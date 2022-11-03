/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_variable.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 01:43:37 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/02 15:31:06 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environement_variable.h"

void	env_key_value_free(t_env_key_value *kv)
{
	free(kv->key);
	kv->key = NULL;
	free(kv->value);
	kv->value = NULL;
}

t_env_key_value key_value_init(char *kv)
{
	t_env_key_value var;
	size_t	i;

	var.key = NULL;
	var.value = NULL;
	i = 0;
	while (kv[i] && kv[i] != '=')
		i++;
	var.key = strndup(kv, i);
	if (var.key == NULL)
		return (var);
	var.value = strdup(kv + i + 1);
	return (var);
}

t_env	env_init_from_envp(const char *envp[])
{
	t_env env;
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	env.v = vec_new(sizeof(t_env_key_value), i, (void (*)(void *))env_key_value_free);
	i = 0;
	while (envp[i])
	{
		t_env_key_value key_value = key_value_init((char *)envp[i]);
		vec_append(&env.v, &key_value);
		i++;
	}
	return (env);
}

void	env_free(t_env	*env)
{
	vec_free(&env->v);
}

t_env_key_value	*env_get_vars(t_env	env, size_t *length)
{
	*length = env.v.len;
	return (env.v.data);
}

//if key is present get it's value, or NULL if key doesn't exist
t_env_key_value	*env_get_key_value_ptr(t_env env, char *key)
{
	size_t	i;

	i = 0;
	while (i < env.v.len)
	{
		t_env_key_value *kv = vec_get(&env.v, i);
		if (strcmp(key, kv->key) == 0)
			return (kv);
		i++;
	}
	return (NULL);
}


//get value from key
char	*env_get_var(t_env env, char *key)
{
	t_env_key_value *kv;
	kv = env_get_key_value_ptr(env, key);
	if (kv == NULL)
		return (NULL);
	return (kv->value);
}

//remove the variable from this key in the env
void	env_remove_var(t_env *env, char *key)
{
	t_env_key_value *current_var;
	t_env_key_value *last_var;


	current_var = env_get_key_value_ptr(*env, key);
	if (current_var == NULL)
		return ;
	last_var = vec_get(&env->v, env->v.len - 1);
	env_key_value_free(current_var);
	env->v.len -= 1;
	if (current_var != last_var)
	{
		current_var->key = last_var->key;
		current_var->value = last_var->value;
	}
}
/*
if key already exist overwrite value
key and value will be duplicated
*/
void	env_set_var(t_env *env, char *key, char *value)
{
	t_env_key_value *kv;

	kv = env_get_key_value_ptr(*env, key);
	if (kv)
	{
		free(kv->value);
		kv->value = strdup(value);
	}
	else
	{
		t_env_key_value var = {.key = strdup(key), .value = strdup(value)};
		vec_append(&env->v, &var);
	}
}

char *env_key_value_to_string(t_env_key_value kv)
{
	char			*str;
	const size_t	key_len = strlen(kv.key); 
	const size_t	value_len = strlen(kv.value); 

	str = malloc(strlen(kv.key) + strlen(kv.value) + 2);
	memcpy(str, kv.key, key_len);
	str[key_len] = '=';
	memcpy(str + key_len + 1, kv.value, value_len);
	str[key_len + 1 + value_len] = '\0';
	return (str);
}

char **env_to_envp(t_env env)
{
	char	**envp;
	size_t	i;

	envp = malloc(sizeof(*envp) * (env.v.len + 1));
	i = 0;
	while (i < env.v.len)
	{
		t_env_key_value kv = *(t_env_key_value *)vec_get(&env.v, i);
		envp[i] = env_key_value_to_string(kv);
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

void envp_free(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
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
