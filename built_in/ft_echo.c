/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 18:08:41 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/23 20:25:13 by tbousque         ###   ########.fr       */
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
		if (sentence == NULL)
			return (NULL);
		ft_strlcpy(sentence, arg, ft_strlen(arg) + 1);
		return (sentence);
	}
	s1 = ft_strjoin(sentence, " ");
	free(sentence);
	if (s1 == NULL)
		return (NULL);
	s2 = ft_strjoin(s1, arg);
	free (s1);
	if (s2 == NULL)
		return (NULL);
	return (s2);
}

int	ft_print_sentence(char *sentence, int flag)
{
	int	print_return;

	print_return = 0;
	if (sentence != NULL)
	{
		if (flag == 1)
			print_return = printf("%s", sentence);
		else
			print_return = printf("%s\n", sentence);
		free (sentence);
	}
	else
	{
		if (flag == 0)
			print_return = printf("\n");
	}
	if (print_return < 0)
	{	
		perror("Minishell: echo");
		return (1);
	}
	return (0);
}

char	*ft_prepare_sentence(char **argv, char *sentence, int i)
{
	while (argv[i])
	{
		sentence = ft_add_next_word(sentence, argv[i++]);
		if (sentence == NULL)
		{
			ft_putstr_fd("Minishell: echo: error malloc\n", 2);
			return (NULL);
		}
	}
	return (sentence);
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
	{
		if (printf("\n") < 0)
			return (perror("Minishell: echo"), 1);
		return (0);
	}
	flag = ft_is_flag_echo(argv[1]);
	if (flag == 1)
		i++;
	if ((flag == 1 && (ft_strlen_l(argv) > 2)) \
	|| (flag == 0 && (ft_strlen_l(argv) > 1)))
	{
		sentence = ft_prepare_sentence(argv, sentence, i);
		if (sentence == NULL)
			return (1);
	}
	return (ft_print_sentence(sentence, flag));
}
