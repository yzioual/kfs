#include "printk.h"

static int	pk_putchar(char c)
{
	vga_putchar(c);
	return (1);
}

static int	pk_puts(const char *s)
{
	int	i;

	if (s == NULL)
	{
		vga_puts("(null)");
		return (6);
	}
	i = 0;
	while (s[i])
	{
		vga_putchar(s[i]);
		i++;
	}
	return (i);
}

static int	pk_putnbr(int nb)
{
	int	count;

	count = 0;
	if (nb >= 0 && nb < 10)
	{
		pk_putchar(nb + '0');
		return (1);
	}
	else if (nb < 0)
	{
		if (nb == -2147483648)
		{
			pk_putchar('-');
			pk_putchar('2');
			count += 2 + pk_putnbr(147483648);
		}
		else
		{
			pk_putchar('-');
			count = 1 + pk_putnbr(-nb);
		}
	}
	else
		count += pk_putnbr(nb / 10) + pk_putnbr(nb % 10);
	return (count);
}

static int	pk_put_unsigned(unsigned int num)
{
	int	count;

	count = 0;
	if (num >= 10)
		count += pk_put_unsigned(num / 10);
	pk_putchar('0' + (num % 10));
	count++;
	return (count);
}

static int	pk_put_hex(unsigned int num, const char *hex_chars)
{
	char	buf[8];
	int		i;

	if (num == 0)
		return (pk_putchar('0'));
	i = 0;
	while (num && i < 8)
	{
		buf[i++] = hex_chars[num & 0xF];
		num >>= 4;
	}
	int	count;
	int	j;

	count = i;
	j = i;
	while (j--)
		pk_putchar(buf[j]);
	return (count);
}

static int	pk_put_ptr(void *ptr)
{
	unsigned long	addr;
	char			buf[8];
	int				i;
	int				count;
	int				j;
	const char		*hex_chars;

	if (ptr == NULL)
	{
		pk_puts("(nil)");
		return (5);
	}
	hex_chars = "0123456789abcdef";
	addr = (unsigned long)ptr;
	pk_putchar('0');
	pk_putchar('x');
	count = 2;
	if (addr == 0)
		return (count + pk_putchar('0'));
	i = 0;
	while (addr && i < 8)
	{
		buf[i++] = hex_chars[addr & 0xF];
		addr >>= 4;
	}
	j = i;
	while (j--)
		pk_putchar(buf[j]);
	count += i;
	return (count);
}

static int	pk_dispatch(const char *fmt, va_list *args)
{
	char	c;

	if (*fmt == 'c')
	{
		c = (char)va_arg(*args, int);
		return (pk_putchar(c));
	}
	if (*fmt == '%')
		return (pk_putchar('%'));
	if (*fmt == 's')
		return (pk_puts(va_arg(*args, char *)));
	if (*fmt == 'd' || *fmt == 'i')
		return (pk_putnbr(va_arg(*args, int)));
	if (*fmt == 'u')
		return (pk_put_unsigned(va_arg(*args, unsigned int)));
	if (*fmt == 'x')
		return (pk_put_hex(va_arg(*args, unsigned int),
					"0123456789abcdef"));
	if (*fmt == 'X')
		return (pk_put_hex(va_arg(*args, unsigned int),
					"0123456789ABCDEF"));
	if (*fmt == 'p')
		return (pk_put_ptr(va_arg(*args, void *)));
	return (0);
}

int	printk(const char *format, ...)
{
	va_list	args;
	int		count;

	if (format == NULL)
		return (0);
	count = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format == '\0')
				break ;
			count += pk_dispatch(format, &args);
		}
		else
		{
			pk_putchar(*format);
			count++;
		}
		format++;
	}
	va_end(args);
	return (count);
}
