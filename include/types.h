/*
 * types.h
 *
 * Created: 17.12.2020 20:56:33
 *  Author: Admin
 */


#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>
#include <stddef.h>

typedef void kTask;
typedef void (*kTask_t)(void*);

typedef uint8_t byte;

//TODO: move to arch

typedef volatile uint8_t kSpinlock_t;

typedef volatile uint8_t* kStackPtr_t;

typedef uint8_t kBaseType_t;

typedef int16_t kIterator_t;
typedef int16_t kReturnValue_t;
typedef int16_t kError_t;

typedef uint8_t kRegister_t;
typedef uint8_t kStatusRegister_t;

typedef uint16_t kStackSize_t;
typedef uint16_t kTaskTicks_t;
typedef uint16_t kPid_t;

typedef enum {KSTATE_UNINIT, KSTATE_SUSPENDED, KSTATE_BLOCKED, KSTATE_READY, KSTATE_SLEEPING} kTaskState_t;
typedef enum {KEVENT_UNINIT, KEVENT_NONE, KEVENT_FIRED} kEventState_t;
typedef enum {KTASK_UNINIT, KTASK_NORMAL_STATIC, KTASK_CRITICAL_STATIC, KTASK_NORMAL, KTASK_CRITICAL} kTaskType_t;
typedef enum {KLOCK_UNINIT, KLOCK_SEMAPHORE, KLOCK_MUTEX} kLockType_t;

struct kTaskStruct_t;
typedef volatile struct kTaskStruct_t* kTaskHandle_t;

#endif /* TYPES_H_ */