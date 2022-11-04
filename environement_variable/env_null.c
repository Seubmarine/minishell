#include "environement_variable.h"
#include <stdio.h>
t_env	env_init_null(char *argv)
{
	t_env	env;
	char	*str;

	str = getcwd(NULL, 0);
	env.v = vec_new(sizeof(t_env_key_value), 3, NULL);
	env_set_var(&env, "PWD", str);
	free(str);
	env_set_var(&env, "SHLVL", "1");
	env_set_var(&env, "_", argv);
	return (env);
}