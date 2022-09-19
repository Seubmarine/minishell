/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 18:38:34 by tbousque          #+#    #+#             */
/*   Updated: 2022/09/19 09:17:55 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include <stdio.h>

//simple case of growable string
int vec_test_1(void)
{
	t_vec name = vec_new(sizeof(char), 2, NULL);
	vec_append(&name, "T");
	vec_append(&name, "h");
	vec_append(&name, "e");
	vec_append(&name, "o");
	vec_append(&name, "\0");
	printf("Vector :\n");
	printf(".data= %s\n", (char *)name.data);
	printf(".len= %ld\n", name.len);
	printf(".capacity= %ld\n", name.capacity);
	vec_free(&name);
	return 0;
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
			str_up[j] -= 0x20;
		j++;
	}
	return (str_up);
}

int	vec_test_2(int argc, char const *argv[])
{
	t_vec	v;
	char	*str;
	int		i;

	i = 1;
	v = vec_new(sizeof(char *), 2, free);
	while (i < argc)
	{
		str = ft_strdup_to_upper(argv[i]);
		vec_append(&v, &str);
		i++;
	}
	i = 0;
	while (i < v.len)
	{
		str = *(char **)vec_get(&v, i);
		printf("%s\n", str);
		i++;
	}
	vec_free(&v);
	return (0);
}

typedef struct s_env_arg
{
	char	*key;
	char	*value;
}	t_env_arg;



t_env_arg env_arg_init(char *arg)
{

	t_env_arg p;
	int i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	p.key = strndup(arg, i);
	p.value = strdup(arg + i + 1);
	return (p);
}

void	env_arg_free(t_env_arg *p)
{
	printf("Free %p is: %s=%s\n", p, p->key, p->value);
	free(p->key);
	p->key = NULL;
	free(p->value);
	p->value = NULL;
}

void vec_test_3(void)
{
	t_vec env = vec_new(sizeof(t_env_arg), 2, (void (*)(void *))env_arg_free);
	t_env_arg tmp;
	size_t i;

	tmp = env_arg_init("HELLO=world");
	vec_append(&env, &tmp);
	tmp = env_arg_init("SHELL=minishell");
	vec_append(&env, &tmp);
	tmp = env_arg_init("HOME=/home/me");
	vec_append(&env, &tmp);
	tmp = env_arg_init("_=./a.out");
	vec_append(&env, &tmp);
	i = 0;
	while (i < env.len)
	{
		tmp = *(t_env_arg *)vec_get(&env, i);
		printf("Key: %s, value: %s\n", tmp.key, tmp.value);
		i++;
	}
	vec_free(&env);
}

/*
int main(int argc, char const *argv[])
{
	printf("\nTest 1: \"simple growable string\"\n"); //really simple example a growable string
	vec_test_1();
	printf("\nTest 2: \"copy argv and make it uppercase\"\n"); //a vector that store multiple string 
	vec_test_2(argc, argv);
	printf("\nTest 3: \"demonstration of complex structure and freeing\"\n");
	vec_test_3();
	return 0;
}
*/