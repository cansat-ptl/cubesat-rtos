/*
 * lists.c
 *
 * Created: 17.12.2020 22:36:16
 *  Author: Admin
 */


#include <rtos/types.h>
#include <rtos/common/lists.h>

void common_listAddBack(struct kLinkedListStruct_t* list, struct kListItemStruct_t* item)
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

void common_listDropBack(struct kLinkedListStruct_t* list)
{
	if (list != NULL) {
		if (list->tail != NULL) {
			struct kListItemStruct_t* next;

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

void common_listAddFront(struct kLinkedListStruct_t* list, struct kListItemStruct_t* item)
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

void common_listDropFront(struct kLinkedListStruct_t* list)
{
	if (list != NULL) {
		if (list->head != NULL) {
			struct kListItemStruct_t* prev;

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

void common_listDeleteAny(struct kLinkedListStruct_t* list, struct kListItemStruct_t* item)
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