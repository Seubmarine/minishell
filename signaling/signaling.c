/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaling.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 19:09:02 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/16 18:04:27 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signaling.h"
#include <readline/readline.h>

void	handler(int signum)
{
	if (signum == SIGINT)
	{
		// printf("default handling\n");
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	return ;
}

void	signal_handling(void)
{
	struct sigaction	sa;
	
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	handler_child(int signum)
{
	
	if (signum == SIGINT)
	{
		// printf("handling child\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		// rl_redisplay();
	}
	return ;
}

void	signal_handling_child(void)
{
	struct sigaction	sa_child;

	sigemptyset(&sa_child.sa_mask);
	sa_child.sa_flags = SA_RESTART;
	sa_child.sa_handler = handler_child;
	sigaction(SIGINT, &sa_child, NULL);
	// sigaction(SIGQUIT, &sa_child, NULL);
}
