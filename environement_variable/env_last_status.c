/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_last_status.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 21:30:53 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/19 07:05:57 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environement_variable.h"

//TODO: replace ft_itoa to ft_itoa_buf

//set status as a string internally
char	*env_set_last_status(t_env *env, int status)
{
	if (env->_last_status_str == NULL)
	{
		write(STDERR_FILENO, "env last status isn't set corretly\n", 35);
		return (NULL);
	}
	status = (status + 256) % 256;
	env->_last_status = status;
	if (ft_ullto_buf(status, env->_last_status_str, \
		ENV_LAST_STATUS_SIZE * sizeof(char)) == 0)
	{
		perror("Minishell: env_set_last_status: ft_ullto_buf");
		env->_last_status_str[0] = '\0';
	}
	return (env->_last_status_str);
}

char	*env_get_last_status(t_env *env)
{
	return (env->_last_status_str);
}
