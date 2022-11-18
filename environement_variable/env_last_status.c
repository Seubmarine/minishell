#include "environement_variable.h"
#include "built_in.h"
#include <stdio.h>
#include <fcntl.h>
#include <heredoc.h>

//set status as a string internally
char	*env_set_last_status(t_env *env, int status)
{
	if (env->_last_status_str == NULL)
	{
		write(STDERR_FILENO, "env last status isn't set corretly\n", 35);
		exit(EXIT_FAILURE);
	}
	status = (status + 256) % 256;
	env->_last_status = status;
	snprintf(env->_last_status_str, ENV_LAST_STATUS_SIZE - 1, "%i", status);//TODO: use itoa
	return (env->_last_status_str);
}

char	*env_get_last_status(t_env *env)
{
	return (env->_last_status_str);
}
