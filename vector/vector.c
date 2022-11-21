/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:19:00 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/20 02:22:35 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

/*
A vector structure that is generic with (void *) but you miss the type hint
look at vector_test.c for more information

a dynamic array that will realloc when necessary to contains a given type
*/
/*
create a vector and need the size of the type it will contains 
if your vector contains a type that need to free some ressource
you can pass a function in free_elem that will be executed to every elements
in the vector
*/
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

//helper fonction that will multiply by two the size of the vector 
int	vec_grow(t_vec	*v)
{
	void	*tmp_data;

	if (v->capacity * 2 <= v->capacity)
	{
		ft_putstr_fd("Minishell: vector capacity overflow\n", STDERR_FILENO);
		return (0);
	}
	tmp_data = malloc(v->elem_size * (v->capacity * 2));
	if (tmp_data == NULL)
	{
		ft_putstr_fd("Minishell: malloc error: vec_grow\n", STDERR_FILENO);
		return (0);
	}
	ft_memcpy(tmp_data, v->data, v->elem_size * (v->len));
	free(v->data);
	v->data = tmp_data;
	v->capacity *= 2;
	return (1);
}

/*
add an element at the end of the vector
element is a pointer to the value you want to put in the vector
will grow the vector if neccesary
*/
int	vec_append(t_vec *v, void *element)
{
	if ((v->len + 1) > v->capacity)
	{
		if (vec_grow(v) == 0)
			return (0);
	}
	ft_memcpy(v->data + v->elem_size * v->len, element, v->elem_size);
	v->len += 1;
	return (1);
}

/*
Get a pointer to your elements at the given index
*/
void	*vec_get(t_vec *v, size_t index)
{
	return (v->data + index * v->elem_size);
}

/*
Free the vector and allmak of it's elements with the free_elem function
*/
void	vec_free(t_vec *v)
{
	size_t	i;

	if (v->free_elem != NULL)
	{
		i = 0;
		while (i < v->len)
		{
			v->free_elem(v->data + v->elem_size * i);
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
