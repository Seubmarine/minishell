#ifndef FT_COLLECT_H
# define FT_COLLECT_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_env
{
	char    	*name;
	char     	*value;
	struct s_env	*next;
}	t_env;

#endif