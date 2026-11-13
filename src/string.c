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

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if ((unsigned char)s1[i] > (unsigned char)s2[i])
			return (1);
		else if ((unsigned char)s1[i] < (unsigned char)s2[i])
			return (-1);
		i++;
	}
	return (0);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (i < n - 1 && s1[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strcpy(char *dest, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int	ft_isxdigit(char c)
{
	return (ft_isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

int	parse_u32(const char *str, u32 *out)
{
	u32	val;
	int	base;
	int	digit;

	val = 0;
	base = 10;
	if (!str || !*str)
		return (0);
	while (ft_isspace(*str))
		str++;
	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
	{
		base = 16;
		str += 2;
	}
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			digit = *str - '0';
		else if (base == 16 && *str >= 'a' && *str <= 'f')
			digit = *str - 'a' + 10;
		else if (base == 16 && *str >= 'A' && *str <= 'F')
			digit = *str - 'A' + 10;
		else
			return (0);
		val = val * base + digit;
		str++;
	}
	if (out)
		*out = val;
	return (1);
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
