/*
 * types.h
 * 
 * Created: 16.06.2021 12:06:32
 * Author: ThePetrovich
 */


#ifndef ARCHSTUB_TYPES_H_
#define ARCHSTUB_TYPES_H_

#define ROMSTR(x) (x)

typedef volatile int kSpinlock_t;

typedef volatile int* kStackPtr_t;

typedef unsigned int kSysTicks_t;

typedef int kBaseType_t;

typedef int kIterator_t;
typedef int kReturnValue_t;
typedef int kError_t;

typedef int kRegister_t;
typedef int kStatusRegister_t;

typedef int kStackSize_t;
typedef int kTaskTicks_t;
typedef int kPid_t;

#endif /* ARCHSTUB_TYPES_H_ */