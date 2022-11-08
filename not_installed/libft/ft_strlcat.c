/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 11:16:10 by mportrai          #+#    #+#             */
/*   Updated: 2021/12/15 16:26:45 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	unsigned int	dsts;
	unsigned int	srcs;
	unsigned int	i;
	unsigned int	j;

	dsts = ft_strlen(dst);
	srcs = ft_strlen(src);
	i = 0;
	j = dsts;
	if (dstsize <= j)
		return (dstsize + srcs);
	while (src[i] && (i < dstsize - j - 1))
	{
		dst[dsts] = src[i];
		dsts ++;
		i++;
	}
	dst[dsts] = '\0';
	return (j + srcs);
}
