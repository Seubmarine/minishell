/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 06:32:39 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/19 06:34:46 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_swap(unsigned char *a, unsigned char *b)
{
	unsigned char	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	ft_strrev(char *str)
{
	size_t			i;
	size_t			j;
	unsigned char	*ustr;

	ustr = (unsigned char *)str;
	i = 0;
	j = ft_strlen(str) - 1;
	while (i < j)
	{
		ft_swap(&ustr[i], &ustr[j]);
		i++;
		j--;
	}
}