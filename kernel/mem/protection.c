/*
 * protection.c
 *
 * Created: 19.12.2020 2:48:56
 *  Author: Admin
 */


#include <types.h>
#include <common.h>

void memory_prepareProtectionRegion(void* pointer, size_t size)
{
	if (pointer != NULL) {
		for (size_t i = 0; i < size; i++) {
			*(byte*)((byte*)pointer + i) = 0xFE;
		}
	}
}

kReturnValue_t memory_checkProtectionRegion(void* pointer, size_t size)
{
	kReturnValue_t kresult = KRESULT_SUCCESS;

	if (pointer != NULL) {
		for (size_t i = 0; i < size; i++) {
			if (*(byte*)((byte*)pointer + i) != 0xFE) {
				kresult = KRESULT_ERR_MEMORY_VIOLATION;
				break;
			}
		}
	}
	
	return kresult;
}