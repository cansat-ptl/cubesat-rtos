/*
 * objects.h
 * 
 * Created: 25.01.2021 08:44:34
 * Author: ThePetrovich
 */


#ifndef KOBJECTS_H_
#define KOBJECTS_H_

#include <types.h>
#include <common.h>

void tasks_addOwnedHeapBlock(kTaskHandle_t task, volatile struct kListItemStruct_t* blockItem);
void tasks_removeOwnedHeapBlock(kTaskHandle_t task, volatile struct kListItemStruct_t* blockItem);

void tasks_addHeldLock(kTaskHandle_t task, volatile struct kListItemStruct_t* blockItem);
void tasks_removeHeldLock(kTaskHandle_t task, volatile struct kListItemStruct_t* blockItem);

#endif
