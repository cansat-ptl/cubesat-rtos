/*
 * flash.h
 * 
 * Created: 05.06.2022 07:41:37
 * Author: ThePetrovich
 */


#ifndef AVRM128_FLASH_H_
#define AVRM128_FLASH_H_

#include <avr/io.h>
#include <avr/iom128.h>
#include <avr/common.h>

#ifdef __cplusplus
extern "C" {
#endif

char arch_flashReadChar(const char *addr);

#ifdef __cplusplus
}
#endif

#endif /* AVRM128_FLASH_H_ */