/*
 * arch.c
 *
 * Created: 17.12.2020 20:53:05
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_stub

#include <kernel/types.h>
#include <kernel/config.h>


void arch_platformInit()
{
	return;
}

void arch_startScheduler()
{
	return;
}

void arch_enterCriticalSection()
{
	return;
}

void arch_exitCriticalSection()
{
	return;
}

void arch_spinlockAcquire(kSpinlock_t *spinlock)
{
	return;
}

void arch_spinlockRelease(kSpinlock_t *spinlock)
{
	return;
}

void arch_setupSystickTimer()
{
	return;
}

void arch_startSystickTimer()
{
	return;
}

void arch_stopSystickTimer()
{
	return;
}

kStatusRegister_t arch_enterCriticalSectionSafe()
{
	return 0;
}

void arch_exitCriticalSectionSafe(kStatusRegister_t sreg)
{
	return;
} 

void arch_halt()
{	
	return;
}

void arch_reboot()
{
	return;
}

void arch_yield()
{
	return;
}

void arch_tick()
{
	return;
}

#endif