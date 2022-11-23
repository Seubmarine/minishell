/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 19:45:11 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/24 00:32:23 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	ft_isalnum_underscore(char c)
{
	if (c == '_')
		return (1);
	return (ft_isalnum(c));
}

int	is_non_alphanum(char *argv)
{
	int	i;

	i = 0;
	if (argv[0] == '=')
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(argv, 2);
		ft_putstr_fd(": not a valid identifier\n", 2);
		return (1);
	}
	while (argv[i] && argv[i] != '=')
	{
		if ((argv[0] >= '0' && argv[0] <= '9') || \
		(ft_isalnum_underscore(argv[i]) == 0))
		{
			ft_putstr_fd("Minishell: ", 2);
			ft_putstr_fd(argv, 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			return (1);
		}
		i++;
	}
	return (0);
}

int	builtin_export(char **argv, t_env *env)
{
	size_t			i;
	t_env_key_value	kv;

	i = 0;
	while (argv[++i])
	{
		if (is_non_alphanum(argv[i]) == 0)
		{
			if (key_value_init(argv[i], &kv) == 1)
			{	
				if (env_set_var(env, kv.key, kv.value) == 0)
				{
					perror("Minishell: export");
					return (env_key_value_free(&kv), 1);
				}
				env_key_value_free(&kv);
			}
			else
			{
				perror("Minishell: export");
				return (env_key_value_free(&kv), 1);
			}
		}
	}
	return (0);
}
