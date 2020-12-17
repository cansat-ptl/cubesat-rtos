/*
 * arch.c
 *
 * Created: 17.12.2020 22:29:34
 *  Author: Admin
 */


#include <types.h>
#include <config.h>
#include <arch/mega128/stack.h>
#include <arch/mega128/mega128.h>

extern kTaskHandle_t kCurrentTask;
volatile byte kReservedMemory[CFG_KERNEL_RESERVED_MEMORY];
kStackPtr_t kStackPointer = &kReservedMemory[CFG_KERNEL_RESERVED_MEMORY-1];

void tasks_tick();
void tasks_switchTask();

void __attribute__ (( naked, noinline )) arch_yield(void)
{
	arch_SAVE_CONTEXT();

	tasks_switchTask();

	arch_RESTORE_CONTEXT();
	arch_RET();
}

void __attribute__ (( naked, noinline )) arch_tick()
{
	arch_SAVE_CONTEXT();

	tasks_tick();

	arch_RESTORE_CONTEXT();
	arch_RET();
}

kStatusRegister_t arch_startAtomicOperation()
{
	kStatusRegister_t sreg = arch_STATUS_REG;
	arch_DISABLE_INTERRUPTS();
	return sreg;
}

void arch_endAtomicOperation(kStatusRegister_t sreg)
{
	arch_ENABLE_INTERRUPTS();
	arch_STATUS_REG = sreg;
	return;
}

void arch_spinlockAcquire(kSpinlock_t* spinlock)
{
	while(1) {
		asm volatile("": : :"memory");
		if(*spinlock == 0) {
			kStatusRegister_t sreg = arch_startAtomicOperation();
			if(*spinlock == 0) {
				*spinlock = 1;
				arch_endAtomicOperation(sreg);
				return;
			}
			arch_endAtomicOperation(sreg);
		}
	}
}

void arch_spinlockRelease(kSpinlock_t* spinlock)
{
	kStatusRegister_t sreg = arch_startAtomicOperation();
	*spinlock = 0;
	arch_endAtomicOperation(sreg);
}