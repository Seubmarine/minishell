/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 14:13:18 by mportrai          #+#    #+#             */
/*   Updated: 2021/12/17 12:40:20 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*dstc;
	unsigned char	*srcc;

	dstc = (unsigned char *) dst;
	srcc = (unsigned char *) src;
	if (!dstc && !srcc)
		return (NULL);
	if (dstc < srcc)
		return (ft_memcpy(dst, src, len));
	while (len-- > 0)
		dstc[len] = srcc[len];
	return (dst);
}
