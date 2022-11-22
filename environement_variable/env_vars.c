/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 06:52:59 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/19 06:52:59 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environement_variable.h"

int	env_set_new_var(t_env *env, char *key, char *value)
{
	t_env_key_value	var;

	var = (t_env_key_value){.key = ft_strdup(key), \
	.value = ft_strdup(value)};
	if (var.key == NULL || var.value == NULL)
	{
		if (var.key != NULL)
			free(var.key);
		else
			free(var.value);
		return (0);
	}
	if (vec_append(&env->v, &var) == 0)
	{
		free(var.key);
		free(var.value);
		return (0);
	}
	return (1);
}

/*
if key already exist overwrite value
key and value will be duplicated
*/
int	env_set_var(t_env *env, char *key, char *value)
{
	t_env_key_value	*kv;

	kv = env_get_key_value_ptr(*env, key);
	if (kv)
	{
		free(kv->value);
		kv->value = ft_strdup(value);
		if (kv->value == NULL)
			return (0);
	}
	else
	{
		if (env_set_new_var(env, key, value) == 0)
			return (0);
	}
	return (1);
}

//remove the variable from this key in the env
void	env_remove_var(t_env *env, char *key)
{
	t_env_key_value	*current_var;
	t_env_key_value	*last_var;

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

//get value from key
char	*env_get_var(t_env env, char *key)
{
	t_env_key_value	*kv;

	if (ft_strcmp("?", key) == 0)
		return (env_get_last_status(&env));
	kv = env_get_key_value_ptr(env, key);
	if (kv == NULL)
		return (NULL);
	return (kv->value);
}

t_env_key_value	*env_get_vars(t_env	env, size_t *length)
{
	*length = env.v.len;
	return (env.v.data);
}
