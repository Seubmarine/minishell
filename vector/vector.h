/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 20:19:21 by tbousque          #+#    #+#             */
/*   Updated: 2022/08/13 22:05:06 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H
# include <stddef.h>
# include <string.h>
# include <stdlib.h>
# include "libft.h"

typedef struct s_vec
{
	void	*data;
	size_t	len;
	size_t	capacity;
	size_t	elem_size;
	void	(*free_elem)(void *);
}	t_vec;

t_vec	vec_new(size_t element_size, size_t capacity,
			void (*free_elem)(void *));

void	vec_append(t_vec *v, void *element);

void	*vec_get(t_vec *v, size_t index);

void	vec_free(t_vec *v);
#endif