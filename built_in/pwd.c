/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 20:13:41 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/08 01:48:27 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

int builtin_pwd(void)
{
	char	*current_directory;

	current_directory = getcwd(NULL, 0);
	if (current_directory == NULL)
		return (1);
	printf("%s\n", current_directory);
	free(current_directory);
	return (0);
}