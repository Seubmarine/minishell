/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_key.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 21:30:01 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/21 17:28:24 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environement_variable.h"

void	env_key_value_free(t_env_key_value *kv)
{
	if (kv->key != NULL)
	{	free(kv->key);
		kv->key = NULL;
	}
	if (kv->value != NULL)
	{
		free(kv->value);
		kv->value = NULL;
	}
}

int	key_value_init(char *kv, t_env_key_value *var)
{
	size_t			i;

	var->key = NULL;
	var->value = NULL;
	i = 0;
	while (kv[i] && kv[i] != '=')
		i++;
	if (kv[i] != '=' || i == 0)
		return (0);
	var->key = ft_strndup((const char *)kv, i);
	if (var->key == NULL)
		return (0);
	var->value = ft_strdup(kv + i + 1);
	if (var->value == NULL)
		return (0);
	return (1);
}

//if key is present get it's value, or NULL if key doesn't exist
t_env_key_value	*env_get_key_value_ptr(t_env env, char *key)
{
	size_t			i;
	t_env_key_value	*kv;

	i = 0;
	while (i < env.v.len)
	{
		kv = vec_get(&env.v, i);
		if (ft_strcmp(key, kv->key) == 0)
			return (kv);
		i++;
	}
	return (NULL);
}

char	*env_key_value_to_string(t_env_key_value kv)
{
	char			*str;
	const size_t	key_len = ft_strlen(kv.key);
	const size_t	value_len = ft_strlen(kv.value);

	str = malloc(ft_strlen(kv.key) + ft_strlen(kv.value) + 2);
	ft_memcpy(str, kv.key, key_len);
	str[key_len] = '=';
	ft_memcpy(str + key_len + 1, kv.value, value_len);
	str[key_len + 1 + value_len] = '\0';
	return (str);
}
