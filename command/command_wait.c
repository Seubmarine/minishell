/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_wait.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mportrai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 19:43:20 by mportrai          #+#    #+#             */
/*   Updated: 2022/11/21 19:43:21 by mportrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

int	waiting_childs(t_env *env, t_ast *ast, t_pidpes *pidpes)
{
	int		wstatus;

	pidpes->i = 0;
	wstatus = 0;
	while (env->is_child == 0 && pidpes->i < ast->pipeline.len)
	{
		waitpid(pidpes->pids[pidpes->i], &wstatus, 0);
		pidpes->exit_status = WEXITSTATUS(wstatus);
		pidpes->i += 1;
	}
	if (WIFSIGNALED(wstatus))
	{
		pidpes->exit_status = WTERMSIG(wstatus) + 128;
		if (WTERMSIG(wstatus) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(wstatus) == SIGSEGV)
			write(STDERR_FILENO, "Segmentation fault (core dumped)\n", 33);
	}
	return (pidpes->exit_status);
}
