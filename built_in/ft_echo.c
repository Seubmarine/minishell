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
		// if NULL
		ft_strlcpy(sentence, arg, ft_strlen(arg) + 1);
		return (sentence);
	}
	s1 = ft_strjoin(sentence, " ");
	// if NULL
	free(sentence);
	s2 = ft_strjoin(s1, arg);
	// if NULL
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
			// if NULL
	}
	ft_print_sentence(sentence, flag);
	return (0);
}
