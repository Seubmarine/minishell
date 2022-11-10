#include "built_in.h"

int	builtin_unset(char **argv, t_env *env)
{
	size_t	i;

	i = 1;
	while (argv[i])
	{
		env_remove_var(env, argv[i]);
		i++;
	}
	return (0);
}
