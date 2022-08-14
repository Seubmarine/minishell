/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:19:00 by tbousque          #+#    #+#             */
/*   Updated: 2022/08/14 03:37:28 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

t_vec	vec_new(size_t element_size, size_t capacity, void (*free_elem)(void *))
{
	t_vec	v;

	v.len = 0;
	v.capacity = 0;
	v.elem_size = 0;
	v.data = malloc(element_size * capacity);
	v.free_elem = free_elem;
	if (v.data == NULL)
		return (v);
	v.capacity = capacity;
	v.elem_size = element_size;
	return (v);
}

void	vec_grow(t_vec	*v)
{
	void	*tmp_data;

	tmp_data = malloc(v->elem_size * (v->capacity * 2));
	if (tmp_data == NULL)
	{
		vec_free(v);
		return ;
	}
	memcpy(tmp_data, v->data, v->elem_size * (v->len));
	free(v->data);
	v->data = tmp_data;
	v->capacity *= 2;
}

void	vec_append(t_vec *v, void *element)
{
	if ((v->len + 1) > v->capacity)
	{
		vec_grow(v);
		if (v->data == NULL)
			return ;
	}
	memcpy(v->data + v->elem_size * v->len, element, v->elem_size);
	v->len += 1;
}

void	*vec_get(t_vec *v, size_t index)
{
	return (v->data + index * v->elem_size);
}

void	vec_free(t_vec *v)
{
	size_t	i;

	if (v->free_elem != NULL)
	{
		i = 0;
		while (i < v->len)
		{
			v->free_elem(*(void **)(v->data + v->elem_size * i));
			i++;
		}
	}
	if (v->data != NULL)
		free(v->data);
	v->data = NULL;
	v->len = 0;
	v->elem_size = 0;
	v->capacity = 0;
	v->free_elem = NULL;
}
