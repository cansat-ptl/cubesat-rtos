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

typedef struct kLinkedListStruct_t kLinkedList_t;
typedef struct kLinkedListItemStruct_t kLinkedListItem_t;

struct kLinkedListStruct_t
{
	kLinkedListItem_t *head;
	kLinkedListItem_t *tail;
};

struct kLinkedListItemStruct_t
{
	kLinkedList_t *list;

	kLinkedListItem_t *next;
	kLinkedListItem_t *prev;

	void *data;
};

#ifdef __cplusplus
extern "C" {
#endif

void common_listAddBack(kLinkedList_t *list, kLinkedListItem_t *item);
void common_listAddFront(kLinkedList_t *list, kLinkedListItem_t *item);
void common_listDropBack(kLinkedList_t *list);
void common_listDropFront(kLinkedList_t *list);
void common_listDeleteAny(kLinkedList_t *list, kLinkedListItem_t *item);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_COMMON_H_ */