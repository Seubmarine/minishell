/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 19:42:43 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/23 18:03:10 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "vector.h"
#include <stdio.h>
#include "ast.h"
#include "command.h"
#include "environement_variable.h"
#include "signaling.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"

t_env	ft_prepare_env(const char **envp, char *argv)
{
	t_env	env;

	if (*envp == NULL)
		env = env_init_null(argv);
	else
	{
		(void) argv;
		env = env_init_from_envp(envp);
	}
	return (env);
}

int	prompt(t_env *env)
{
	char	*line;
	int		is_running;
	int		exit_status;

	exit_status = 0;
	is_running = 1;
	while (is_running)
	{
		line = readline("Minishell$ ");
		if (line == NULL)
		{
			is_running = 0;
			exit_status = 0;
			write(2, "\nexit\n", 6);
		}
		else
		{
			add_history(line);
			exit_status = execute_line(line, env);
			if (env->is_child)
				is_running = 0;
		}
		free(line);
	}
	return (exit_status);
}

int	main(int argc, char const *argv[], char const *envp[])
{
	t_env	env;
	int		exit_status;

	(void) argc;
	exit_status = 0;
	signal_handling();
	env = ft_prepare_env(envp, (char *)argv[0]);
	remove_echo_ctrl();
	exit_status = prompt(&env);
	rl_clear_history();
	env_free(&env);
	add_echo_ctrl();
	return (exit_status);
}
