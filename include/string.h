#ifndef STRING_H
#define STRING_H

#include "types.h"

void	*ft_memset(void *str, int c, size_t n);
size_t	ft_strlen(const char *str);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memcpy(void	*dest, const void *src, size_t n);
int	ft_strcmp(const char *s1, const char *s2);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strncpy(char *dest, const char *src, size_t n);
int	ft_isspace(char c);
int	ft_isdigit(char c);
int	ft_isxdigit(char c);
int	parse_u32(const char *str, u32 *out);

#endif
