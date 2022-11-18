#include "environement_variable.h"
#include "built_in.h"
#include <stdio.h>
#include <fcntl.h>
#include <heredoc.h>

/*
if key already exist overwrite value
key and value will be duplicated
*/
void	env_set_var(t_env *env, char *key, char *value)
{
	t_env_key_value	*kv;
	t_env_key_value	var;

	kv = env_get_key_value_ptr(*env, key);
	if (kv)
	{
		free(kv->value);
		kv->value = ft_strdup(value);
	}
	else
	{
		var = (t_env_key_value){.key = ft_strdup(key), \
		.value = ft_strdup(value)};
		vec_append(&env->v, &var);
	}
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