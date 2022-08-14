/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 18:38:34 by tbousque          #+#    #+#             */
/*   Updated: 2022/08/14 18:48:41 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

int	vec_test_1(int argc, char const *argv[])
{
	const t_vec	v = vec_new(sizeof(int *), 1, NULL);
	int			*m;
	size_t		i;

	i = 0;
	while (i < 52)
	{
		m = malloc(sizeof(int));
		*m = i * 2;
		vec_append(&v, (void *)m);
		i++;
	}
	i = 0;
	while (i < 52)
	{
		printf("%i\n", **(int **)vec_get(&v, i));
		i++;
	}
	vec_free(&v);
	return (0);
}

int	ft_islower(char c)
{
	return (c >= 'a' && c <= 'z');
}

char	*ft_strdup_to_upper(char const *str)
{
	char	*str_up;
	int		j;

	str_up = strdup(str);
	j = 0;
	while (str_up[j])
	{
		if (ft_islower(str_up[j]))
		{
			str_up[j] -= 'a';
			str_up[j] += 'A';
		}
		j++;
	}
	return (str_up);
}

int	vec_test_3(int argc, char const *argv[])
{
	t_vec	v;
	char	*str;
	int		i;

	i = 1;
	v = vec_new(sizeof(char *), 2, free);
	while (i < argc)
	{
		str = ft_strdup_to_upper(argv[i]);
		vec_append(&v, str);
		i++;
	}
	i = 0;
	while (i < v.len)
	{
		printf("%s\n", *(char **)vec_get(&v, i));
		i++;
	}
	vec_free(&v);
	return (0);
}
