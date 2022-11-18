#include "environement_variable.h"
#include "built_in.h"
#include <stdio.h>
#include <fcntl.h>
#include <heredoc.h>

void	env_free(t_env	*env)
{
	vec_free(&env->v);
	env->_last_status = 0;
	free(env->_last_status_str);
}

void	envp_free(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
