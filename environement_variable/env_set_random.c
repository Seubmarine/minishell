#include "environement_variable.h"
#include "built_in.h"
#include <stdio.h>
#include <fcntl.h>
#include <heredoc.h>

void	env_random_open_error(void)
{
	perror("Minishell: open: ");
	exit (1);
}

void	env_random_read_error(int *fd_urandom)
{
	close(*fd_urandom);
	perror("Minishell: read: ");
	exit (1);
}

//return 1 on success 0 on error
int	env_set_random_str(t_env *env)
{
	long long unsigned	seed;
	int					fd_urandom; //check error

	fd_urandom = open("/dev/urandom", O_RDONLY);
	if (fd_urandom == -1)
		env_random_open_error();
	if (read(fd_urandom, &seed, sizeof(seed)) == -1)
		env_random_read_error(&fd_urandom);
	if (close(fd_urandom) == -1)
	{
		perror("Minishell: close: ");
		exit (1);
	}
	env->random_str[0] = '\0';
	if (itoa_buf(seed, env->random_str, RANDOM_STR_LEN * sizeof(char)) == 0)
	{
		write(STDERR_FILENO, "Minishell: error creating seed: itoa\n", 37);
		ft_strlcpy(env->random_str, "seed_error", \
		RANDOM_STR_LEN * sizeof(char));
		exit(1);
	}
	return (1);
}
