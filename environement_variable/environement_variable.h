/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_variable.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 01:39:38 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/22 19:58:39 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONEMENT_VARIABLE_H
# define ENVIRONEMENT_VARIABLE_H
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include "vector.h"
# include "libft.h"
# define ENV_LAST_STATUS_SIZE 256
# define D_TERM "dumb"
# define D_PATH "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
# define RANDOM_STR_LEN 32

typedef struct s_env_key_value
{
	char	*key;
	char	*value;
}	t_env_key_value;

typedef struct s_env
{
	t_vec	v;
	int		_last_status;
	char	*_last_status_str;
	int		is_child;
	char	random_str[RANDOM_STR_LEN];
}	t_env;

int				env_set_random_str(t_env *env);
void			ft_prepare_shl_shlvl(t_env *env, char *argv);
t_env_key_value	*env_get_key_value_ptr(t_env env, char *key);
char			*env_key_value_to_string(t_env_key_value kv);
void			env_key_value_free(t_env_key_value *kv);
void			ft_env_set_random_error(t_env *env);

//create a key value from a string "key=value"
int				key_value_init(char *kv, t_env_key_value *var);

//create an environement using an array of KEY=value
//envp must be terminated by NULL
t_env			env_init_from_envp(const char *envp[], char *argv);
int				env_collect_from_envp(char **envp, char *argv, t_env *env);
int				env_init_null_set_undescore(t_env *env, char *argv);
t_env			env_init_null(char *argv);

//get an array of key_value, pass the size of the array in length
t_env_key_value	*env_get_vars(t_env	env, size_t *length);

//remove the variable from this key in the env
void			env_remove_var(t_env *env, char *key);

//if key already exist overwrite value
int				env_set_var(t_env *env, char *key, char *value);

//set and get special variable $?
char			*env_set_last_status(t_env *env, int status);
char			*env_get_last_status(t_env *env);

//if key is present get it's value, or NULL if key doesn't exist
char			*env_get_var(t_env env, char *key);
void			env_free(t_env	*env);

//copy env as a formated envp to pass to function
char			**env_to_envp(t_env env);
void			envp_free(char **envp);
#endif