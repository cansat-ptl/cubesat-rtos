/*
 * panic.c
 * 
 * Created: 20.03.2021 01:47:17
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/debug/printk.h>
#include <kernel/arch/arch.h>

void kernel_panic(char *msg)
{
    #if CFG_KERNEL_PANIC_ACTION != 0
        debug_printk("\r\nFATAL: Kernel panic - %s\r\n", msg);
    #endif

    #if CFG_KERNEL_PANIC_ACTION != 2
        arch_reboot();
    #else
        arch_halt();
    #endif
}