/*
 * arch.h
 * 
 * Created: 08.02.2021 03:32:12
 * Author: ThePetrovich
 */


#ifndef KERNEL_MODULE_ARCH_H_
#define KERNEL_MODULE_ARCH_H_

#ifdef KERNEL_ARCH_avr
	#define KERNEL_ARCH "avr"
	#if defined KERNEL_MCU_atmega128
		#define KERNEL_MCU "atmega128"
		#include "./mega128/arch.h"
		#include "./mega128/uart.h"
		#include "./mega128/stack.h"
	#elif defined KERNEL_MCU_atmega2560
		#define KERNEL_MCU "atmega2560"
		#include "./mega2560/arch.h"
		#include "./mega2560/uart.h"
		#include "./mega2560/stack.h"
	#else
		#error "Unsupported/unknown AVR MCU"
	#endif
#else
	#error "Unsupported/unknown architecture"
#endif

#ifndef KERNEL_ARCH
#define KERNEL_ARCH "unkn"
#endif

#ifndef KERNEL_MCU
#define KERNEL_MCU "unkn"
#endif

#endif