
/**
 * @defgroup tasks
 * @brief Kernel tasks module.
 * 
 * @file tasks.h
 *
 * @ingroup tasks
 *
 * @brief Kernel task manipulation functions.
 */

/*
 * tasks.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_TASKS_H_
#define KERNEL_TASKS_H_

#include <stddef.h>
#include <kernel/types.h>
#include <kernel/common/lists.h>

#define KTASKFLAG_DYNAMIC ((byte)(0x01))

struct kEventStruct_t
{							
	kEventState_t state;				/**< Event state, KEVENT_NONE or KEVENT_FIRED. */
	kBaseType_t eventFlags;				/**< User-defined event flags. */
};

/**
 * @brief Main task structure, which holds all task state information.
 */
struct kTaskStruct_t
{							
	kStackPtr_t stackPtr; 				/**< Current stack pointer. */
	kStackPtr_t stackBegin;				/**< Stack begin. */
	size_t stackSize;				/**< Stack size in bytes. */

	void (*entry)(void);				/**< Task entry point. */
	void* args;					/**< Task startup args, can be NULL if none. */

	kTaskState_t state;				/**< Current task state. */
	kTaskTicks_t sleepTime;				/**< Time to sleep in ticks, used when state is KSTATE_SLEEPING. */
	kTaskType_t type;				/**< Task type, KTASK_NORMAL or KTASK_CRITICAL.*/
	kBaseType_t priority;				/**< Task scheduling priority; 0 is reserved. */
	byte flags;					/**< Status flags, for now only KTASKFLAG_DYNAMIC. */
	kPid_t pid;					/**< Task ID. */
	char* name;					/**< Task display name. */

	volatile struct kEventStruct_t event;		/**< Struct for event system. */

	kLinkedList_t allocList;			/**< Heap allocated memory list. */

	kLinkedListItem_t activeTaskListItem;		/**< List item used for task scheduling and runqueues. */
	kLinkedListItem_t globalTaskListItem;		/**< List item for global task list. */
	kLinkedListItem_t childTaskListItem;		/**< List item used for child/parent relationship. */

	kLinkedList_t childTaskList;			/**< Child task list. */
};

#define tasks_TASK_STRUCT_SIZE ((sizeof(struct kTaskStruct_t) + ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) & ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK))

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup tasks
 * @brief Initializes internal task manager structures.
 */
void tasks_init();

/**
 * @ingroup tasks
 * @brief Creates a task using statically allocated buffer.
 * 
 * @param taskMemory Pointer to task memory buffer.
 * @param memorySize Task memory size. 
 * Note: memory size must include sizeof(kTask_t), stackSize and CFG_STACK_SAFETY_MARGIN if memory protection mode is set to 2 or 3.
 * @param entry Pointer to task entry point.
 * @param args Pointer to entry point parameters, can be NULL.
 * @param priority Task scheduling priority.
 * @param type Task type, can be KTASK_NORMAL or KTASK_CRITICAL. KTASK_CRITICAL will cause kernel panic on exit.
 * @param name Task display name string.
 * @return kTask_t* task handle on success, NULL otherwise.
 */
kTask_t *tasks_createTaskStatic(void *taskMemory, size_t memorySize, void (*entry)(void), void *args, kBaseType_t priority, kTaskType_t type, char *name);

/**
 * @ingroup tasks
 * @brief Creates a task using heap.
 * 
 * @param stackSize Task stack size. Memory needed for kTask_t struct is allocated automatically.
 * @param entry Pointer to task entry point.
 * @param args Pointer to entry point parameters, can be NULL.
 * @param priority Task scheduling priority.
 * @param type Task type, can be KTASK_NORMAL or KTASK_CRITICAL. KTASK_CRITICAL will cause kernel panic on exit.
 * @param name Task display name string.
 * @return kTask_t* task handle on success, NULL otherwise.
 */
kTask_t *tasks_createTaskDynamic(size_t stackSize, void (*entry)(void), void *args, kBaseType_t priority, kTaskType_t type, char *name);

/**
 * @ingroup tasks
 * @brief Deletes task.
 * 
 * @param task Task handle.
 */
void tasks_deleteTask(kTask_t *task);

/**
 * @ingroup tasks
 * @brief Sets task state.
 * 
 * @param task Task handle.
 * @param state New task state. Allowed values are KSTATE_SUSPENDED, KSTATE_BLOCKED, KSTATE_READY, KSTATE_SLEEPING.
 */
void tasks_setTaskState(kTask_t *task, kTaskState_t state);

/**
 * @ingroup tasks
 * @brief Sets task priority.
 * 
 * @param task Task handle.
 * @param priority New task priority. Value must be between 1 and CFG_NUMBER_OF_PRIORITIES - 1. 0 is reserved for idle task.
 */
void tasks_setTaskPriority(kTask_t *task, kBaseType_t priority);

/**
 * @ingroup tasks
 * @brief Gets task state.
 * 
 * @param task Task handle.
 * @return kTaskState_t - current task state. Possible values are KSTATE_SUSPENDED, KSTATE_BLOCKED, KSTATE_READY, KSTATE_SLEEPING. 
 * Returns KSTATE_UNINIT on failure.
 */
kTaskState_t tasks_getTaskState(kTask_t *task);

/**
 * @ingroup tasks
 * @brief Gets task priority.
 * 
 * @param task Task handle.
 * @return kBaseType_t - current task priority. Value is between 0 and CFG_NUMBER_OF_PRIORITIES - 1.
 * Returns 0 on failure.
 */
kBaseType_t tasks_getTaskPriority(kTask_t *task);

/**
 * @ingroup tasks
 * @brief Gets task type.
 * 
 * @param task Task handle.
 * @return kTaskType_t - task type. Possible values are KTASK_NORMAL or KTASK_CRITICAL.
 * Returns KTASK_UNINIT on failure.
 */
kTaskType_t tasks_getTaskType(kTask_t *task);

/**
 * @ingroup tasks
 * @brief Gets task allocated memory list.
 * 
 * @param task Task handle.
 * @return kLinkedList_t* - allocated memory list.
 * Returns NULL on failure.
 */
kLinkedList_t *tasks_getTaskAllocList(kTask_t *task);

/**
 * @ingroup tasks
 * @brief Sets task state to KSTATE_BLOCKED and appends it to blockList.
 * 
 * @param task Task handle.
 * @param blockList List with blocked tasks.
 */
void tasks_blockTask(kTask_t *task, kLinkedList_t *blockList);

/**
 * @ingroup tasks
 * @brief Sets task state to KSTATE_READY and removes it from blockList.
 * 
 * @param task Task handle.
 */
void tasks_unblockTask(kTask_t *task);

/**
 * @ingroup tasks
 * @brief Checks if task stack pointer is within task memory.
 * 
 * @param task Task handle.
 * @return kReturnValue_t - KRESULT_SUCCESS on success, KRESULT_ERR_GENERIC on failure.
 */
kReturnValue_t tasks_checkStackBounds(kTask_t *task);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_TASKS_H_ */