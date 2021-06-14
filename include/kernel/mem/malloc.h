/*
 * malloc.h
 * 
 * Created: 13.06.2021 10:48:35
 * Author: ThePetrovich
 */


#ifndef KERNEL_MALLOC_H_
#define KERNEL_MALLOC_H_

#include <kernel/types.h>

void *mem_malloc(size_t size);
void mem_free(void *pointer);

#endif