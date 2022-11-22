/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 19:45:11 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/21 19:45:12 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

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
		if ((argv[0] >= '0' && argv[0] <= '9') || (ft_isalnum(argv[i]) == 0))
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

	i = 1;
	while (argv[i])
	{
		if ((is_non_alphanum(argv[i]) == 0) && \
		(key_value_init(argv[i], &kv) == 1))
		{	
			if (env_set_var(env, kv.key, kv.value) == 0)
			{
				perror("Minishell: export");
				env_key_value_free(&kv);
				return (1);
			}
			env_key_value_free(&kv);
		}
		else if (key_value_init(argv[i], &kv) == 0)
			return (perror("Minishell: key_value_init"), 1);
		i++;
	}
	return (0);
}
