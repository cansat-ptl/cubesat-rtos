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
        debug_printk_p(ROMSTR("\r\nFATAL: Kernel panic - %s\r\n"), msg);
    #endif

    #if CFG_KERNEL_PANIC_ACTION == 1
        arch_reboot();
    #elif CFG_KERNEL_PANIC_ACTION == 2
        arch_halt();
    #endif
}

void kernel_panic_p(const char *msg)
{
    #if CFG_KERNEL_PANIC_ACTION != 0
        debug_printk_p(ROMSTR("\r\nFATAL: Kernel panic - "));
        debug_printk_p(msg);
        debug_printk_p("\r\n");
    #endif

    #if CFG_KERNEL_PANIC_ACTION == 1
        arch_reboot();
    #elif CFG_KERNEL_PANIC_ACTION == 2
        arch_halt();
    #endif
}