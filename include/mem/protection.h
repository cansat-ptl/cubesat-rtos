/*
 * protection.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_MEMORY_H_
#define KERNEL_MEMORY_H_

#include <types.h>

void memory_prepareProtectionRegion(void* pointer, size_t size);
kReturnValue_t memory_checkProtectionRegion(void* pointer, size_t size);

#endif /* MEMORY_H_ */