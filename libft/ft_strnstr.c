/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 12:07:15 by mportrai          #+#    #+#             */
/*   Updated: 2022/01/12 06:26:15 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	unsigned int	i;
	unsigned int	j;
	char			*s1;

	i = 0;
	j = 0;
	s1 = (char *)haystack;
	while (needle[j] != '\0')
		j++;
	if (j == 0)
		return (s1);
	while (haystack[i] != '\0' && len > 0 && j <= len)
	{
		if (!(ft_strncmp(&haystack[i], needle, j)))
			return (&s1[i]);
		i++;
		len--;
	}
	return (0);
}
