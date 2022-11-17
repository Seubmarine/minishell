/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 15:22:46 by mportrai          #+#    #+#             */
/*   Updated: 2022/01/12 14:14:48 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*s1;
	unsigned int	i;

	if (s == NULL)
		return (NULL);
	i = 0;
	if ((ft_strlen(s) == 0) || (ft_strlen(s) < start))
	{
		s1 = malloc(sizeof(char) * 1);
		s1[0] = '\0';
		return (s1);
	}
	if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	s1 = malloc(sizeof(char) * (len + 1));
	if (s1 == NULL)
		return (NULL);
	while ((i < len) && (s[start + i] != '\0'))
	{
		s1[i] = s[start + i];
		i++;
	}
	s1[i] = '\0';
	return (s1);
}
