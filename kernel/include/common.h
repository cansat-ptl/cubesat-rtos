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

#define KRESULT_SUCCESS 0
#define KRESULT_ERR_GENERIC -1
#define KRESULT_ERR_NULLPTR -2
#define KRESULT_ERR_MEMORY_VIOLATION -3
#define KRESULT_ERR_OUT_OF_MEMORY -4

#define KFLAG_INIT 0
#define KFLAG_TIMER_SET 1
#define KFLAG_TIMER_EN 2
#define KFLAG_TIMER_ISR 3
#define KFLAG_SD_INIT 4
#define KFLAG_CSW_ALLOWED 5
#define KFLAG_RROBIN_SWITCHED 6
#define KFLAG_LOG_SD 13
#define KFLAG_LOG_UART 14
#define KFLAG_DEBUG 15

#define KOSSTATUS_INIT 0
#define KOSSTATUS_RUNNING 1
#define KOSSTATUS_HALTED 2
#define KOSSTATUS_ERRORED 3

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