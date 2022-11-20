/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 00:15:31 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/13 18:02:05 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "path_finder.h"

char	*path_concat(char *path, size_t path_len, char *exec_name)
{
	char	*concat;

	concat = malloc(path_len + strlen(exec_name) + 2);
	if (concat == NULL)
		return (NULL);
	ft_memcpy(concat, path, path_len);
	concat[path_len] = '/';
	ft_memcpy(concat + path_len + 1, exec_name, ft_strlen(exec_name));
	concat[path_len + 1 + ft_strlen(exec_name)] = '\0';
	return (concat);
}

char	*get_current_path(char *path_array, size_t *path_len, char *exec_name)
{
	char	*current_path;

	current_path = NULL;
	while (path_array[*path_len] && path_array[*path_len] != ':')
		*path_len += 1;
	current_path = path_concat(path_array, *path_len, exec_name);
	return (current_path);
}

//return the path to the executable given in a formated PATH string if it exist
//return NULL if the executable doesn't exist in the PATH
char	*find_exec(char *exec_name, char *path_array)
{
	char		*current_path;
	size_t		path_len;
	char		*executable_path;
	struct stat	file_info;

	executable_path = NULL;
	current_path = NULL;
	if (exec_name == NULL || path_array == NULL)
		return (NULL);
	while (*path_array)
	{
		path_len = 0;
		current_path = get_current_path(path_array, &path_len, exec_name);
		if (current_path == NULL)
			return (NULL);
		if (stat(current_path, &file_info) == 0)
			return (current_path);
		free(current_path);
		if (executable_path)
			return (executable_path);
		path_array += path_len;
		if (*path_array == ':')
			path_array += 1;
	}
	return (NULL);
}

// int main(int argc, char const *argv[])
// {
// 	char *path_array = getenv("PATH");
// 	printf("%s\n", path_array);
// 	if (!path_array)
// 		return (EXIT_FAILURE);
// 	char *exec_path = find_exec("ls", path_array);
// 	if (exec_path)
// 	{
// 		printf("%s\n", exec_path);
// 		free(exec_path);
// 	}
// 	return 0;
// }
