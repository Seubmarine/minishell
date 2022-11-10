/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 18:08:41 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/10 18:08:41 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include <stdio.h>

// EN attendant la libft,

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	unsigned long	i;

	i = 0;
	if (dstsize != 0)
	{
		while (i < dstsize - 1 && src[i] != '\0')
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	i = 0;
	while (src[i])
		i++;
	return (i);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	unsigned int	dsts;
	unsigned int	srcs;
	unsigned int	i;
	unsigned int	j;

	dsts = ft_strlen(dst);
	srcs = ft_strlen(src);
	i = 0;
	j = dsts;
	if (dstsize <= j)
		return (dstsize + srcs);
	while (src[i] && (i < dstsize - j - 1))
	{
		dst[dsts] = src[i];
		dsts ++;
		i++;
	}
	dst[dsts] = '\0';
	return (j + srcs);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*dest;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	if (i + j == 0)
	{
		dest = malloc(sizeof(char) * 1);
		dest[0] = '\0';
		return (dest);
	}
	dest = malloc(sizeof(char) * (i + j + 1));
	if (dest == NULL)
		return (NULL);
	i = ft_strlcpy(dest, s1, i + 1);
	j = ft_strlcat(&dest[i], s2, j + 1);
	return (dest);
}

// A RETIRER QUAND LIBFT INTEGRE.

int	ft_strlen_l(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
	return (i);
}

int	ft_is_flag_echo(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] != '-')
		return (0);
	while (arg[++i])
	{
		if (arg[i] != 'n')
			return (0);
	}
	return (1);
}

char	*ft_add_next_word(char *sentence, char *arg)
{
	char	*s1;
	char	*s2;

	s1 = NULL;
	s2 = NULL;
	if (sentence == NULL)
	{
		sentence = malloc(sizeof(char) * (ft_strlen(arg) + 1));
		ft_strlcpy(sentence, arg, ft_strlen(arg) + 1);
		return (sentence);
	}
	s1 = ft_strjoin(sentence, " ");
	free(sentence);
	s2 = ft_strjoin(s1, arg);
	free (s1);
	return (s2);
}

void	ft_print_sentence(char *sentence, int flag)
{
	if (sentence != NULL)
	{
		if (flag == 1)
			printf("%s", sentence);
		else
			printf("%s\n", sentence);
		free (sentence);
	}
	else
	{
		if (flag == 0)
			printf("\n");
	}
}

int	ft_echo(char **argv)
{
	char	*sentence;
	int		i;
	int		flag;

	i = 1;
	flag = 0;
	sentence = NULL;
	if (ft_strlen_l(argv) == 1)
		return (printf("\n"), 0);
	flag = ft_is_flag_echo(argv[1]);
	if (flag == 1)
		i++;
	if ((flag == 1 && (ft_strlen_l(argv) > 2)) \
	|| (flag == 0 && (ft_strlen_l(argv) > 1)))
	{
		while (argv[i])
			sentence = ft_add_next_word(sentence, argv[i++]);
	}
	ft_print_sentence(sentence, flag);
	return (0);
}
