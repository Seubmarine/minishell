/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 10:49:45 by mportrai          #+#    #+#             */
/*   Updated: 2021/12/16 13:14:24 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*s;
	unsigned long	i;

	s = (unsigned char *)b;
	i = 0;
	while (i < len)
		s[i++] = (unsigned char) c;
	return ((unsigned char *)b);
}
