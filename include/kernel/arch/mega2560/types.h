/*
 * types.h
 * 
 * Created: 15.06.2021 11:55:26
 * Author: ThePetrovich
 */


#ifndef AVRM2560_TYPES_H_
#define AVRM2560_TYPES_H_

#include <stdint.h>

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

#endif /* AVRM2560_TYPES_H_ */