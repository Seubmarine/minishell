/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaling_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 16:34:02 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/24 00:14:51 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signaling.h"
#include <readline/readline.h>

void	handler_child(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
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
}
