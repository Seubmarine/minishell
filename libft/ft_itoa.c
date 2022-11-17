/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 11:27:56 by mportrai          #+#    #+#             */
/*   Updated: 2022/01/12 06:21:32 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_write_itoa(char	*s, long n, int len)
{
	int		i;
	long	buff;

	buff = n;
	i = 0;
	if (n < 0)
	{
		s[i] = '-';
		n *= -1;
		buff = n;
		i++;
	}
	while (n != 0)
	{
		while (n >= 10)
			n %= 10;
		s[len - 1] = (((char)n) + 48);
		buff /= 10;
		n = buff;
		i++;
		len--;
	}
	s[i] = '\0';
	return (s);
}

static size_t	ft_strnum(long n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n *= -1;
		i++;
	}
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*s;
	long	nb;

	nb = n;
	len = ft_strnum(nb);
	s = malloc(sizeof(char) * len + 1);
	if (s == NULL)
		return (NULL);
	if (n == 0)
	{
		s[0] = '0';
		s[1] = '\0';
		return (s);
	}
	s = ft_write_itoa(s, nb, len);
	return (s);
}
