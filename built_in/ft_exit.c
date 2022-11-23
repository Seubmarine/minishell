/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 18:06:06 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/24 00:48:07 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int	ft_write_exit(t_env *env)
{
	if (env->is_child == 0)
		write(2, "exit\n", 5);
	env->is_child = 1;
	return (0);
}

int	ft_exit_error(char *str, int code, t_env *env)
{
	if (code == 1)
		return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
	ft_putstr_fd("Minishell: exit: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	env->is_child = 1;
	return (2);
}

long long int	ft_atolli(char *str, t_env *env)
{
	long long unsigned int	res;
	int						sign;
	int						i;

	i = 0;
	sign = 1;
	res = 0;
	if (ft_strncmp(str, "-9223372036854775808", 20) == 0)
		return (0);
	while (str[i] && ((str[i] >= '\t' && str[i] <= '\r')
			|| str[i] == ' '))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = sign * -1;
		i++;
	}
	while ((str[i] >= '0' && str[i] <= '9') && (res < 9223372036854775807))
		res = res * 10 + (str[i++] - '0');
	if ((res > 9223372036854775807) && (sign == 1))
		return (ft_exit_error(str, 2, env));
	if ((res > 9223372036854775807) && (sign == -1))
		return (ft_exit_error(str, 2, env));
	return (res * sign);
}

int	ft_only_num(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '-')
	{
		if (ft_strlen(arg) == 1)
			return (1);
		else
			i++;
	}
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	ft_exit(char **argv, t_env *env)
{
	int				i;
	long long int	errnum;

	i = 0;
	errnum = 256;
	if (ft_strlen_l(argv) == 1)
		return (ft_write_exit(env), env->_last_status);
	if (ft_only_num(argv[1]) != 0)
		return (ft_exit_error(argv[1], 2, env));
	while (argv[i])
		i++;
	if (i > 2)
		return (ft_exit_error(NULL, 1, env));
	i = ft_atolli(argv[1], env);
	i = i % 256;
	errnum += i;
	errnum = errnum % 256;
	ft_write_exit(env);
	return (errnum);
}
