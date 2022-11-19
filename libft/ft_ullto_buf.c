/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ullto_buf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 06:26:24 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/19 06:50:22 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	_ullto_buf(unsigned long long x, char *buffer, size_t buffer_size)
{
	if (buffer_size == 0)
		return (0);
	if (x == 0)
	{
		buffer[0] = '\0';
		return (1);
	}
	buffer[0] = (x % 10 + '0');
	if (!_ullto_buf(x / 10, buffer + 1, buffer_size - 1))
		return (0);
	return (1);
}

//put an int in a string with a max size
//return 0 on failure 1 one success
int	ft_ullto_buf(unsigned long long x, char *buffer, size_t buffer_size)
{
	unsigned long long	n;
	const int			is_neg = 0;

	n = x;
	if (buffer_size == 0)
		return (0);
	if (n == 0)
	{
		if (buffer_size < 2)
			return (0);
		ft_memcpy(buffer, "0", 2);
		return (1);
	}
	if (is_neg)
	{
		buffer[0] = '-';
		n = -n;
	}
	if (!_ullto_buf(n, buffer + (is_neg), buffer_size - (is_neg)))
		return (0);
	ft_strrev(buffer + (is_neg));
	return (1);
}