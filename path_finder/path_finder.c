/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 00:15:31 by tbousque          #+#    #+#             */
/*   Updated: 2022/09/25 01:34:06 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
char *exec_is_in_path(char *exec_name, char *path)
{
	DIR *d;
	struct dirent *dir;
	char *exec_path = NULL;

	d = opendir(path); //TODO: check error
	if (d)
	{
		while (1)
		{
			dir = readdir(d);
			if (dir == NULL)
				break;
			if (strcmp(exec_name, dir->d_name) == 0)
			{
				exec_path = malloc(strlen(exec_name) + strlen(path) + 2);
				memcpy(exec_path, path, strlen(path));
				exec_path[strlen(path)] = '/';
				memcpy(exec_path + strlen(path) + 1, exec_name, strlen(exec_name));
				exec_path[strlen(exec_name) + strlen(path) + 1] = '\0';
				closedir(d); //TODO: check error
				return (exec_path);
			}
		}
		closedir(d);
	}
	return (NULL);
}


//return the path to the executable given in a formated PATH string if it exist
//return NULL if the executable doesn't exist in the PATH
char *find_exec(char *exec_name, char *path_array)
{
	char	*current_path;
	char	*chr_result = path_array;
	char	*executable_path = NULL;

	while (1)
	{
		if (chr_result == NULL)
			break;
		chr_result = strchr(path_array, ':');
		current_path = strndup(path_array, chr_result - path_array);
		executable_path = exec_is_in_path(exec_name, current_path);
		free(current_path);
		if (executable_path)
			return (executable_path);
		path_array = chr_result + 1;
	}
	return (NULL);
}

int main(int argc, char const *argv[])
{
	char *path_array = getenv("PATH");
	printf("%s\n", path_array);
	if (!path_array)
		return (EXIT_FAILURE);
	char *exec_path = find_exec("sol", path_array);
	if (exec_path)
	{
		printf("%s\n", exec_path);
		free(exec_path);
	}
	return 0;
}
