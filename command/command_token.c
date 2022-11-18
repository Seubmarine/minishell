#include "command.h"

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
