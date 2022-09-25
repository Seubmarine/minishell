/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_variable.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 01:39:38 by tbousque          #+#    #+#             */
/*   Updated: 2022/09/24 23:28:53 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONEMENT_VARIABLE_H
# define ENVIRONEMENT_VARIABLE_H
# include <stdlib.h>
# include <string.h>
# include "vector.h" 

typedef struct s_env_key_value
{
	char *key;
	char *value;
}	t_env_key_value;


typedef struct s_env
{
	t_vec v;
}	t_env;


//get an array of key_value, pass the size of the array in length
t_env_key_value	*env_get_vars(t_env	env, size_t *length);

//remove the variable from this key in the env
void	env_remove_var(t_env *env, char *key);

//if key already exist overwrite value
void	env_set_var(t_env *env, char *key, char *value);

//if key is present get it's value, or NULL if key doesn't exist
char	*env_get_var(t_env env, char *key);
void	env_free(t_env	*env);
#endif