/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 14:00:42 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/08 01:48:15 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include <stdio.h>

int builtin_env(char **argv, t_env *env)
{
	size_t	i;
	t_env_key_value	*kv;

	(void) argv;
	i = 0;
	while (i < env->v.len)
	{
		kv = vec_get(&env->v, i);
		printf("%s=%s\n", kv->key, kv->value);
		i++;
	}
	return (0);
}