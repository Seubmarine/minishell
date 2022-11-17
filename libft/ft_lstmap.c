/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 15:38:25 by mportrai          #+#    #+#             */
/*   Updated: 2022/01/04 13:10:55 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*res;

	if (!lst)
		return (NULL);
	res = ft_lstnew(f(lst->content));
	if (!res)
	{
		ft_lstclear(&res, *del);
		return (NULL);
	}
	lst = lst->next;
	while (lst)
	{
		ft_lstadd_back(&res, ft_lstnew(f(lst->content)));
		if (!res)
		{
			ft_lstclear(&res, *del);
			return (NULL);
		}
		lst = lst->next;
	}
	return (res);
}
