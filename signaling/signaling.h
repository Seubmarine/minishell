/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaling.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 19:11:22 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/23 13:57:05 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALING_H
# define SIGNALING_H
# include <signal.h>
# include <unistd.h>
# include <stdio.h>
# include "libft.h"
# include <termios.h>

void	signal_handling(void);
void	signal_handling_child(void);
void	remove_echo_ctrl(void);
void	add_echo_ctrl(void);
#endif