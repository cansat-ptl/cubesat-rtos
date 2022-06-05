/*
 * flash.h
 * 
 * Created: 05.06.2022 08:04:06
 * Author: ThePetrovich
 */


#ifndef ARCHSTUB_FLASH_H_
#define ARCHSTUB_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

char __attribute__((weak)) arch_flashReadChar(const char *addr) {
        return *addr;
}

#ifdef __cplusplus
}
#endif

#endif /* ARCHSTUB_FLASH_H_ */