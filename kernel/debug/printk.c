/*
 * printk.c
 * 
 * Created: 20.03.2021 02:32:33
 * Author: awend0
 */

#include <stdarg.h>
#include <kernel/types.h>
#include <kernel/debug/printk.h>
#include <kernel/common/string.h>
#include <kernel/arch/arch.h>

static kSpinlock_t kPrintkSpinlock = 0;

static int debug_spacing(int count, char c)
{
	int ret = 0;

	while (ret < count)
	{
		uart_putc(c);
		ret++;
	}
	return (ret);
}

static int debug_putnbr(long number, int base, int len, char *alph)
{
	char str[17] = {0};
	int sign;

	sign = (number < 0) ? -1 : 1;
	number *= sign;
	len--;
	while (len >= 0)
	{
		str[len] = alph[number % base];
		number /= base;
		len--;
	}
	if (sign == -1)
	{
		str[0] = '-';
	}
	uart_puts(str);
	return (common_strlen(str));
}

static int debug_nbrlen(long nbr, int base)
{
	int	len = 0;

	if (nbr == 0)
	{
		return (1);
	}
	if (nbr < 0 && base == 10)
	{
		len += 1;
	}
	while (nbr)
	{
		nbr /= base;
		len += 1;
	}
	return (len);
}

static int debug_print_d(char **format, int width, int zero_flag, va_list args)
{
	long value = (long)va_arg(args, int);
	int len = debug_nbrlen(value, 10);
	int ret = 0;

	(*format)++;
	if (width == -1)
	{
		width = len;
	}
	if (width > len)
	{
		ret += debug_spacing(width - len, (zero_flag) ? '0' : ' ');
	}
	ret += debug_putnbr(value, 10, len, "0123456789");
	return (ret);
}

static int debug_print_x(char **format, int width, int zero_flag, int reg, va_list args)
{
	long value = (long)va_arg(args, unsigned int);
	int len = debug_nbrlen(value, 16);
	int ret = 0;

	(*format)++;
	if (width == -1)
	{
		width = len;
	}
	if (width > len)
	{
		ret += debug_spacing(width - len, (zero_flag) ? '0' : ' ');
	}
	ret += debug_putnbr(value, 16, len, (reg == 0) ? "0123456789ABCDEF" : "0123456789abcdef");
	return (ret);
}

static int debug_print_s(char **format, int width, int zero_flag, va_list args)
{
	char *str = va_arg(args, char*);
	int len;
	int ret = 0;

	(*format)++;
	if (!str)
	{
		str = "(null)";
	}
	len = common_strlen(str);
	if (width == -1)
	{
		width = len;
	}
	if (width > len)
	{
		ret += debug_spacing(width - len, (zero_flag) ? '0' : ' ');
	}
	uart_puts(str);
	return (ret + len);
}

static int debug_parse_flag(char **format)
{
	int ret = 0;

	if (**format == '0')
	{
		ret = 1;
		(*format)++;
	}
	return (ret);
}

static int debug_parse_width(char **format)
{
	int ret = -1;

	if (common_isdigit(**format))
	{
		ret = 0;
		while (common_isdigit(**format))
		{
			ret *= 10;
			ret += **format - '0';
			(*format)++;
		}
	}
	return (ret);
}

static int debug_printk_processor(char **format, va_list args)
{
	int ret = 0;
	int zero_flag;
	int width;

	(*format)++;
	zero_flag = debug_parse_flag(format);
	width = debug_parse_width(format);
	switch(**format)
	{
	case 'd':
		ret = debug_print_d(format, width, zero_flag, args);
		break;
	case 's':
		ret = debug_print_s(format, width, zero_flag, args);
		break;
	case 'x':
		ret = debug_print_x(format, width, zero_flag, 1, args);
		break;
	case 'X':
		ret = debug_print_x(format, width, zero_flag, 0, args);
		break;
	case '%':
		ret = 1;
		uart_putc('%');
		break;
	default:
		ret = 0;
		break;
	}
	return (ret);
}

static int debug_write_till_percent(char **format)
{
	int ret = 0;

	while (**format && **format != '%')
	{
		uart_putc(**format);
		ret++;
		(*format)++;
	}
	return (ret);
}

int debug_printk(char *format, ...)
{
	va_list args;

	int ret = 0;
	kStatusRegister_t sreg = arch_enterCriticalSectionSafe();
	va_start(args, format);
	while (*format)
	{
		ret += debug_write_till_percent(&format);

		if (*format == '%') {
			ret += debug_printk_processor(&format, args);
		}
	}
	va_end(args);
	arch_exitCriticalSectionSafe(sreg);
	return (ret);
}
