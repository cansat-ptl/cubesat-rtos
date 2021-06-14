/*
 * string.c
 * 
 * Created: 20.03.2021 02:32:33
 * Author: awend0
 */


#include <stddef.h>
#include <kernel/config.h>
#include <kernel/common/string.h>

#if CFG_USE_PLATFORM_LIBC_IN_KERNEL == 1
	#include <string.h>
#endif

int common_strlen(char *str)
{
	#if CFG_USE_PLATFORM_LIBC_IN_KERNEL == 1
		return strlen(str);
	#else
		int ret = 0;

		while (*str) {
			ret++;
			str++;
		}

		return (ret);
	#endif
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
	#if CFG_USE_PLATFORM_LIBC_IN_KERNEL == 1
		memset(b, c, len);
	#else
		unsigned char *ptr;
		unsigned char sym;

		ptr = (unsigned char *)b;
		sym = (unsigned char)c;

		while (len--) {
			*ptr++ = sym;
		}
	#endif
}

int common_memcmp(const void *s1, const void *s2, size_t n)
{	
	#if CFG_USE_PLATFORM_LIBC_IN_KERNEL == 1
		return memcmp(s1, s2, n);
	#else
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
	#endif
}

void common_memcpy(void *dst, const void *src, size_t n)
{	
	#if CFG_USE_PLATFORM_LIBC_IN_KERNEL == 1
		memcpy(dst, src, n);
	#else
		char *dest;
		const char *source;

		if (src && dst) {
			source = src;
			dest = dst;

			while (n--) {
				*dest++ = *source++;
			}
		}
	#endif
}