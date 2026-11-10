#include "string.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t		i;
	char		*destination;
	const char	*source;

	destination = (char *)dest;
	source = (const char *)src;
	if (destination == source)
		return (dest);
	if (source < destination && source + n > destination)
	{
		i = n;
		while (i--)
			destination[i] = source[i];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			destination[i] = source[i];
			i++;
		}
	}
	return (dest);
}

void	*ft_memcpy(void	*dest, const void *src, size_t n)
{
	char		*destination;
	const char	*source;
	size_t		i;

	if (!dest && !src)
		return (NULL);
	i = 0;
	destination = (char *)dest;
	source = (const char *)src;
	while (i < n)
	{
		destination[i] = source[i];
		i++;
	}
	return (dest);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] > s2[i])
			return (1);
		else if (s1[i] < s2[i])
			return (-1);
		i++;
	}
	return (0);
}

void	*ft_memset(void *str, int c, size_t n)
{
	unsigned int	i;
	char			*dest;

	i = 0;
	dest = str;
	while (i < n)
	{
		dest[i] = (char)c;
		i++;
	}
	return (str);
}
