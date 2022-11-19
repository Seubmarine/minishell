/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 05:06:09 by tbousque          #+#    #+#             */
/*   Updated: 2022/11/19 06:36:31 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

//transform the ast token to a redirection token
//if the ast token is not a good redirection token it's marked as invalid
enum e_redirection_type	token_to_redirection_type(enum e_token_type type)
{
	if (type == TOKEN_REDIRECT_INPUT || type == TOKEN_HERE_DOCUMENT_OPEN)
		return (REDIRECTION_INPUT);
	else if (type == TOKEN_REDIRECT_OUTPUT)
		return (REDIRECTION_OUTPUT);
	else if (type == TOKEN_REDIRECT_OUTPUT_APPEND)
		return (REDIRECTION_OUTPUT_APPEND);
	else
		return (REDIRECTION_INVALID);
}
