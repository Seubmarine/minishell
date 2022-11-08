#include "built_in.h"

int builtin_export(char **argv, t_env *env)
{
	size_t			i;
	t_env_key_value	kv;

	i = 1;
	while (argv[i])
	{
		kv = key_value_init(argv[i]);
		env_set_var(env, kv.key, kv.value);
		i++;
	}
	return (0);
}