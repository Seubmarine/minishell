/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:09:07 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/08 01:49:07 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H
# include <unistd.h>
# include <stdio.h>
# include "environement_variable.h"

int	builtin_unset(char **argv, t_env *env);
int	builtin_export(char **argv, t_env *env);
int builtin_env(char **argv, t_env *env);
int builtin_pwd(void);
int	builtin(char **argv, t_env *env, int *exit_status);
#endif