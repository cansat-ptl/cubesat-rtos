/*
 * memory.h
 *
 * Created: 18.12.2020 6:19:21
 *  Author: Admin
 */ 


#ifndef MEMORY_H_
#define MEMORY_H_

kReturnValue_t memory_pointerSanityCheck(void* pointer);

void memory_prepareProtectionRegion(void* pointer, int16_t size);
uint8_t memory_checkProtectionRegion(void* pointer, int16_t size);

#endif /* MEMORY_H_ */