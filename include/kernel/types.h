/*
 * types.h
 *
 * Created: 17.12.2020 20:56:33
 *  Author: Admin
 */


#ifndef KERNEL_TYPES_H_
#define KERNEL_TYPES_H_

#include <stdint.h>
#include <stddef.h>

#define KRESULT_SUCCESS 0
#define KRESULT_ERR_GENERIC -1
#define KRESULT_ERR_NULLPTR -2
#define KRESULT_ERR_MEMORY_VIOLATION -3
#define KRESULT_ERR_OUT_OF_MEMORY -4

#define KOSSTATUS_INIT 0
#define KOSSTATUS_RUNNING 1
#define KOSSTATUS_HALTED 2
#define KOSSTATUS_ERRORED 3

typedef uint8_t byte;

typedef volatile uint8_t kSpinlock_t;

typedef volatile uint8_t* kStackPtr_t;

typedef uint32_t kSysTicks_t;

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
typedef enum {KTASK_UNINIT, KTASK_NORMAL, KTASK_CRITICAL} kTaskType_t;
typedef enum {KLOCK_UNINIT, KLOCK_SEMAPHORE, KLOCK_MUTEX} kLockType_t;

typedef struct kTaskStruct_t kTask_t;
typedef struct kLockStruct_t kLock_t;

#endif /* TYPES_H_ */