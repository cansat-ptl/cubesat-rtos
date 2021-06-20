/*
 * lists.c
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/common/lists.h>

void common_listAddBack(kLinkedList_t *list, kLinkedListItem_t *item)
{
	if (list != NULL && item != NULL) {
		item->next = NULL;
		item->prev = list->tail;

		if (list->tail != NULL) {
			list->tail->next = item;
		}

		list->tail = item;

		if (list->head == NULL) {
			list->head = item;
		}

		item->list = list;
	}
	return;
}

void common_listDropBack(kLinkedList_t *list)
{	
	kLinkedListItem_t *next = NULL;

	if (list != NULL) {
		if (list->tail != NULL) {
			next = list->tail;
			list->tail = list->tail->prev;

			if (list->tail != NULL) {
				list->tail->next = NULL;
			}

			if (next == list->head) {
				list->head = NULL;
			}

			next->list = NULL;
		}
	}
	return;
}

void common_listAddFront(kLinkedList_t *list, kLinkedListItem_t *item)
{
	if (list != NULL && item != NULL) {
		item->next = list->head;
		item->prev = NULL;

		if (list->head != NULL) {
			list->head->prev = item;
		}

		list->head = item;

		if (list->tail == NULL) {
			list->tail = item;
		}

		item->list = list;
	}
	return;
}

void common_listDropFront(kLinkedList_t *list)
{	
	kLinkedListItem_t *prev = NULL;

	if (list != NULL) {
		if (list->head != NULL) {
			prev = list->head;
			list->head = list->head->next;

			if (list->head != NULL) {
				list->head->prev = NULL;
			}

			if (prev == list->tail) {
				list->tail = NULL;
			}

			prev->list = NULL;
		}
	}
	return;
}

void common_listDeleteAny(kLinkedList_t *list, kLinkedListItem_t *item)
{
	if (list != NULL && item != NULL) {
		if (list->head == item) {
			common_listDropFront(list);
		}
		else {
			if (list->tail == item) {
				common_listDropBack(list);
			}
			else {
				if (item->next != NULL) {
					item->next->prev = item->prev;
				}

				if (item->prev != NULL) {
					item->prev->next = item->next;
				}
			}
		}
		item->list = NULL;
	}

	return;
}