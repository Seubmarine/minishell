#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*dupped;
	size_t	i;

	i = 0;
	while (s[i] && i < n)
		i++;
	dupped = malloc(sizeof(*dupped) * (i + 1));
	if (dupped == NULL)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		dupped[i] = s[i];
		i++;
	}
	dupped[i] = '\0';
	return (dupped);
}
