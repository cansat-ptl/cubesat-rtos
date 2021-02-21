/*
 * lists.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_COMMON_H_
#define KERNEL_COMMON_H_

#include <stddef.h>
#include <stdint.h>

#define common_LISTITEM_STRUCT_SIZE (sizeof(struct kListItemStruct_t)

struct kLinkedListStruct_t
{
	struct kListItemStruct_t* head;
	struct kListItemStruct_t* tail;
};

struct kListItemStruct_t
{
	struct kLinkedListStruct_t* list;
	struct kListItemStruct_t* next;
	struct kListItemStruct_t* prev;
	void* data;
};

void common_listAddBack(struct kLinkedListStruct_t* list, struct kListItemStruct_t* item);
void common_listAddFront(struct kLinkedListStruct_t* list, struct kListItemStruct_t* item);
void common_listDropBack(struct kLinkedListStruct_t* list);
void common_listDropFront(struct kLinkedListStruct_t* list);
void common_listDeleteAny(struct kLinkedListStruct_t* list, struct kListItemStruct_t* item);

#endif