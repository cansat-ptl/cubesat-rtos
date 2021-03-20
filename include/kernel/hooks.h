/*
 * hooks.h
 * 
 * Created: 20.03.2021 02:03:05
 * Author: ThePetrovich
 */


#ifndef KERNEL_HOOKS_H_
#define KERNEL_HOOKS_H_

void kernel_taskReturnHook();
void kernel_stackCorruptionHook();
void kernel_BadAllocHook();
void kernel_OutOfMemoryHook();
 
#endif