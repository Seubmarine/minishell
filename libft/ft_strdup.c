/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 15:15:36 by mportrai          #+#    #+#             */
/*   Updated: 2021/12/15 16:18:32 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_small_malloc(void)
{
	char	*str;

	str = malloc(sizeof(char) * 1);
	if (str == NULL)
		return (NULL);
	str[0] = '\0';
	return (str);
}

char	*ft_strdup(const char *s1)
{
	int		i;
	int		j;
	char	*dest;

	i = 0;
	j = 0;
	while (s1[j])
		j++;
	if (j == 0)
		return (ft_small_malloc());
	dest = malloc(sizeof(char) * j + 1);
	if (dest == NULL)
		return (NULL);
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
