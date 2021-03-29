/*
 * string.c
 * 
 * Created: 20.03.2021 02:32:33
 * Author: awend0
 */


#include <stddef.h>
#include <kernel/common/string.h>

int common_strlen(char *str)
{
	int ret = 0;

	while (*str) {
		ret++;
		str++;
	}

	return (ret);
}

int common_isdigit(char c)
{
	if (c >= '0' && c <= '9') {
		return (1);
	}

	return (0);
}

void common_memset(void *b, int c, size_t len)
{
	unsigned char *ptr;
	unsigned char sym;

	ptr = (unsigned char *)b;
	sym = (unsigned char)c;

	while (len--) {
		*ptr++ = sym;
	}
}

int common_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char *p1;
	unsigned char *p2;

	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	
	while (n--) {
		if (*p1++ != *p2++) {
			return (*--p1 - *--p2);
		}
	}

	return (0);
}

void common_memcpy(void *dst, const void *src, size_t n)
{
	char *dest;
	const char *source;

	if (src && dst) {
		source = src;
		dest = dst;

		while (n--) {
			*dest++ = *source++;
		}
	}
}