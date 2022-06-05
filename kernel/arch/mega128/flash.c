/*
 * flash.c
 * 
 * Created: 05.06.2022 07:37:30
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_avr
#ifdef KERNEL_MCU_atmega128

#include <kernel/types.h>
#include <kernel/config.h>
#include <avr/pgmspace.h>
#include <kernel/arch/mega128/arch.h>
#include <kernel/arch/mega128/flash.h>

char arch_flashReadChar(const char *addr)
{
        return (char)pgm_read_byte(addr);
}

#endif
#endif