/*
 * taskobjects.c
 * 
 * Created: 25.01.2021 08:43:49
 * Author: ThePetrovich
 */


#include <types.h>
#include <config.h>
#include <common.h>
#include <tasks/tasks.h>
#include <tasks/taskobjects.h>
#include <arch/arch.h>

void tasks_addTaskOwnedHeapBlock(kTaskHandle_t task, volatile struct kListItemStruct_t* blockItem)
{
    #if CFG_TRACK_MEMORY_BLOCK_OWNERS == 1
        if (task != NULL) {
            if (blockItem != NULL) {
                common_listAddBack(&(task->ownedHeapBlocks), blockItem);
            }
        }
    #endif
}

void tasks_removeTaskOwnedHeapBlock(kTaskHandle_t task, volatile struct kListItemStruct_t* blockItem)
{
    #if CFG_TRACK_MEMORY_BLOCK_OWNERS == 1
        if (task != NULL) {
            if (blockItem != NULL) {
                common_listDeleteAny(&(task->ownedHeapBlocks), blockItem);
            }
        }
    #endif
}

void tasks_addTaskHeldLock(kTaskHandle_t task, volatile struct kListItemStruct_t* blockItem)
{
    #if CFG_TRACK_TASK_HELD_LOCKS == 1
        if (task != NULL) {
            if (blockItem != NULL) {
                common_listAddBack(&(task->heldLocks), blockItem);
            }
        }
    #endif
}

void tasks_removeTaskHeldLock(kTaskHandle_t task, volatile struct kListItemStruct_t* blockItem)
{
    #if CFG_TRACK_TASK_HELD_LOCKS == 1
        if (task != NULL) {
            if (blockItem != NULL) {
                common_listDeleteAny(&(task->heldLocks), blockItem);
            }
        }
    #endif
}