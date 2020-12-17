/*
 * common.h
 *
 * Created: 17.12.2020 22:37:15
 *  Author: Admin
 */


#ifndef COMMON_H_
#define COMMON_H_

#include <stddef.h>
#include <stdint.h>

#define common_SET_BIT(x,y) ((x) |= (1u << (y)))
#define common_CLEAR_BIT(x,y) ((x) &= ~(1u << (y)))
#define common_CHECK_BIT(x,y) (((0u == ((x) & (1u << (y)))) ? 0u : 1u))
#define common_SWITCH_BIT(x,y) (((x) ^= (1u << (y))))
#define common_WRITE_BIT(x,y,z) ((x) = ((x) & ~(1u << (y))) | ((z) << (y)));

#define common_ARRAY_INDEX_FROM_ADDR(base, addr, type) (((uint16_t)(addr)-(uint16_t)(base))/sizeof(type))

#define common_LISTITEM_STRUCT_SIZE (sizeof(struct kListItemStruct_t)

struct kLinkedListStruct_t
{
	volatile struct kListItemStruct_t* head;
	volatile struct kListItemStruct_t* tail;
};

struct kListItemStruct_t
{
	volatile struct kLinkedListStruct_t* list;
	volatile struct kListItemStruct_t* next;
	volatile struct kListItemStruct_t* prev;
	volatile void* data;
};

void common_listAddBack(volatile struct kLinkedListStruct_t* list, volatile struct kListItemStruct_t* item);
void common_listAddFront(volatile struct kLinkedListStruct_t* list, volatile struct kListItemStruct_t* item);
void common_listDropBack(volatile struct kLinkedListStruct_t* list);
void common_listDropFront(volatile struct kLinkedListStruct_t* list);
void common_listDeleteAny(volatile struct kLinkedListStruct_t* list, volatile struct kListItemStruct_t* item);

#endif /* COMMON_H_ */