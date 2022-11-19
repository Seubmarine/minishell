#include "command.h"

//close an fd and put the dummy value of -2 to show it's invalid
void	fd_close_reset(int *fd)
{
	if (close(*fd) != 0)
		printf("error closing fd %i\n", *fd);
	*fd = -2;
}

void	close_stdfd(void)
{
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}
