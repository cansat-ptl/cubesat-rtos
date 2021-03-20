/*
 * printk.h
 * 
 * Created: 20.03.2021 02:32:33
 * Author: awend0
 */


#ifndef KENREL_PRINTK_H_
#define KENREL_PRINTK_H_

#include <stdarg.h>
#include <string.h>

#define MAX_NBR_LEN 16

int debug_printk(char *format, ...);

#endif