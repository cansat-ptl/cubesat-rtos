/*
 * memory.h
 *
 * Created: 18.12.2020 6:19:21
 *  Author: Admin
 */


#ifndef MEMORY_H_
#define MEMORY_H_

#include <types.h>

void memory_prepareProtectionRegion(void* pointer, size_t size);
kReturnValue_t memory_checkProtectionRegion(void* pointer, size_t size);

#endif /* MEMORY_H_ */