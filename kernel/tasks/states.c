/*
 * states.c
 *
 * Created: 18.12.2020 6:58:25
 *  Author: Admin
 */

#include <types.h>
#include <config.h>
#include <common.h>
#include <arch/arch.h>
#include <tasks/tasks.h>

void tasks_setTaskState(kTaskHandle_t task, kTaskState_t state)
{
	kStatusRegister_t sreg = arch_startAtomicOperation();

	//kReturnValue_t sanityCheck = memory_pointerSanityCheck((void*)task);

	//if (sanityCheck == 0) {
		tasks_rescheduleTask(task, state);
		task->state = state;

	//}

	arch_endAtomicOperation(sreg);
}

kReturnValue_t tasks_setTaskPriority(kTaskHandle_t task, uint8_t priority)
{
	kReturnValue_t kresult = KRESULT_ERR_GENERIC;
	kStatusRegister_t sreg = arch_startAtomicOperation();

	//kReturnValue_t sanityCheck = memory_pointerSanityCheck((void*)task);

	//if (sanityCheck == 0) {
		if (priority <= CFG_NUMBER_OF_PRIORITIES) {
			task->priority = priority;
			if (task->state == KSTATE_READY) {
				tasks_rescheduleTask(task, KSTATE_READY);
			}
		}
		else {
			kresult = CFG_NUMBER_OF_PRIORITIES;
		}
	//}

	arch_endAtomicOperation(sreg);
	return kresult;
}