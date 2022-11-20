/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 11:54:21 by mportrai          #+#    #+#             */
/*   Updated: 2021/12/16 17:32:05 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned long	i;
	unsigned char	*dstc;
	unsigned char	*srcc;

	if (!dst || !src)
		return (NULL);
	i = 0;
	dstc = (unsigned char *) dst;
	srcc = (unsigned char *) src;
	while (i < n)
	{
		dstc[i] = srcc[i];
		i++;
	}
	return (dst);
}
