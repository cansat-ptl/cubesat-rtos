/*
 * objects.c
 * 
 * Created: 25.01.2021 08:43:49
 * Author: ThePetrovich
 */


#include <types.h>
#include <config.h>
#include <common.h>
#include <tasks/tasks.h>
#include <tasks/objects.h>
#include <arch/arch.h>

void tasks_addOwnedHeapBlock(kTaskHandle_t task, volatile struct kListItemStruct_t* blockItem)
{
    if (task != NULL) {
        if (blockItem != NULL) {
            common_listAddBack(&(task->ownedHeapBlocks), blockItem);
        }
    }
}

void tasks_removeOwnedHeapBlock(kTaskHandle_t task, volatile struct kListItemStruct_t* blockItem)
{
    if (task != NULL) {
        if (blockItem != NULL) {
            common_listDeleteAny(&(task->ownedHeapBlocks), blockItem);
        }
    }
}

void tasks_addHeldLock(kTaskHandle_t task, volatile struct kListItemStruct_t* blockItem)
{
    if (task != NULL) {
        if (blockItem != NULL) {
            common_listAddBack(&(task->heldLocks), blockItem);
        }
    }
}

void tasks_removeHeldLock(kTaskHandle_t task, volatile struct kListItemStruct_t* blockItem)
{
    if (task != NULL) {
        if (blockItem != NULL) {
            common_listDeleteAny(&(task->heldLocks), blockItem);
        }
    }
}