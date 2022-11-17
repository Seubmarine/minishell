/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 13:36:27 by mportrai          #+#    #+#             */
/*   Updated: 2021/12/17 14:10:01 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned long	i;
	char			*search;
	char			*res;

	search = (char *) s;
	res = NULL;
	i = 0;
	if (c >= 256)
		c = c % 256;
	while (i < n)
	{
		if (search[i] == c)
		{
			res = &search[i];
			return (res);
		}
		i++;
	}
	return (res);
}
