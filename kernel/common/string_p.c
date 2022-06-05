/*
 * string_p.c
 * 
 * Created: 05.06.2022 07:39:49
 * Author: ThePetrovich
 */


#include <stddef.h>
#include <kernel/config.h>
#include <kernel/arch/arch.h>
#include <kernel/common/string.h>

#if CFG_USE_PLATFORM_LIBC_IN_KERNEL == 1
	#include <string.h>
#endif

int common_strlen_p(const char *str)
{
	#if CFG_USE_PLATFORM_LIBC_IN_KERNEL == 1
		return strlen(str);
	#else
		int ret = 0;

		while (arch_flashReadChar(str)) {
			ret++;
			str++;
		}

		return (ret);
	#endif
}

int common_strcmp_p(const char *s1, const char *s2)
{
	#if CFG_USE_PLATFORM_LIBC_IN_KERNEL == 1
		return strcmp(s1, s2);
	#else
		while (arch_flashReadChar(s1) == arch_flashReadChar(s2))
		{
			if (!*s1)
				return (0);
			s1++;
			s2++;
		}

		return ((arch_flashReadChar((unsigned char *)s1) < arch_flashReadChar((unsigned char *)s2)) ? -1 : +1);
	#endif
}