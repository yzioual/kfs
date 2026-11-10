#ifndef STRING_H
#define STRING_H

#include "types.h"

void	*ft_memset(void *str, int c, size_t n);
size_t	ft_strlen(const char *str);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memcpy(void	*dest, const void *src, size_t n);
int	ft_strcmp(char *s1, char *s2);

#endif
