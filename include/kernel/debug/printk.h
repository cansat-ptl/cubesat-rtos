#ifndef PRINTK_H
# define PRINTK_H
# define MAX_NBR_LEN 16
# include <stdarg.h>
# include <string.h>

int		debug_printk(char *format, ...);

#endif