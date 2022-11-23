/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 16:54:33 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/23 16:54:34 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environement_variable.h"

void	env_vec_new_error(t_env *env)
{
	perror("Minishell: vec_new");
	env_free(env);
	exit (1);
}

void	env_last_status_error(void)
{
	perror("Minishell: malloc");
	exit (1);
}

void	ft_shl_shlvl_cwd_error(t_env *env)
{
	perror("Minishell: getcwd");
	env_free(env);
	exit (1);
}

void	ft_shl_shlvl_join_error(t_env *env)
{
	perror("Minishell: ft_strjoin");
	env_free(env);
	exit(1);
}

void	ft_shl_shlvl_itoa_error(t_env *env)
{
	perror("Minishell: ft_itoa");
	env_free(env);
	exit (1);
}
