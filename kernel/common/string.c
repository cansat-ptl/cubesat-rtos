#include <string.h>

int common_strlen(char *str)
{
	int ret = 0;

	while (*str)
	{
		ret++;
		str++;
	}
	return (ret);
}

int common_isdigit(char c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}