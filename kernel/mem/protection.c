/*
 * protection.c
 *
 * Created: 19.12.2020 2:48:56
 *  Author: Admin
 */
#include <types.h>


void memory_prepareProtectionRegion(void* pointer, int16_t size)
{
	if (pointer != NULL) {
		for (int16_t i = 0; i < size; i++) {
			*(uint8_t*)((uint8_t*)pointer + i) = 0xFE;
		}
	}
}

uint8_t memory_checkProtectionRegion(void* pointer, int16_t size)
{
	uint8_t exitcode = 0;
	if (pointer != NULL) {
		for (int16_t i = 0; i < size; i++) {
			if (*(uint8_t*)((uint8_t*)pointer + i) != 0xFE) {
				exitcode = 1;
				break;
			}
		}
	}
	return exitcode;
}