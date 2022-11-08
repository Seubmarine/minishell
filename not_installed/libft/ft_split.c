/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 11:27:30 by mportrai          #+#    #+#             */
/*   Updated: 2022/01/12 14:14:45 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_freetab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static int	ft_wordcount(char const	*s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && s[i])
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (count);
}

static char	**ft_intab(char const *s, char c, int count, char **tab)
{
	int	i;
	int	tabcount;
	int	start;

	i = -1;
	tabcount = 0;
	start = 0;
	while (tabcount < count && s[++i])
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] != c && s[i])
				i++;
			tab[tabcount] = ft_substr((s + start), 0, (i - start));
			if (tab[tabcount] == NULL)
			{
				ft_freetab(tab);
				return (NULL);
			}
			tabcount++;
		}
	}
	tab[tabcount] = NULL;
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		count;

	if (s == NULL)
		return (NULL);
	count = ft_wordcount(s, c);
	tab = malloc(sizeof(char *) * (count + 1));
	if (tab == NULL)
		return (NULL);
	tab = ft_intab(s, c, count, tab);
	return (tab);
}
