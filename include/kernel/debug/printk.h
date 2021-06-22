/*
 * printk.h
 * 
 * Created: 20.03.2021 02:32:33
 * Author: awend0
 */


#ifndef KENREL_PRINTK_H_
#define KENREL_PRINTK_H_

#include <stdarg.h>

#define MAX_NBR_LEN 16

#ifdef __cplusplus
extern "C" {
#endif

int debug_printk(char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* KENREL_PRINTK_H_ */