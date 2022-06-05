/*
 * arch.h
 * 
 * Created: 08.02.2021 06:57:40
 * Author: ThePetrovich
 */


#ifndef KERNEL_ARCH_H_
#define KERNEL_ARCH_H_

#ifdef KERNEL_ARCH_avr
	#define KERNEL_ARCH "avr"
	#if defined KERNEL_MCU_atmega128
		#define KERNEL_MCU "atmega128"
		#include "./mega128/arch.h"
		#include "./mega128/uart.h"
		#include "./mega128/stack.h"
		#include "./mega128/flash.h"
	#elif defined KERNEL_MCU_atmega2560
		#define KERNEL_MCU "atmega2560"
		#include "./mega2560/arch.h"
		#include "./mega2560/uart.h"
		#include "./mega2560/stack.h"
	#else
		#error "Unsupported/unknown AVR MCU"
	#endif
#elif defined KERNEL_ARCH_stub
	#define KERNEL_ARCH "stub"
	#define KERNEL_MCU "generic"
	#include "./stub/arch.h"
	#include "./stub/uart.h"
	#include "./stub/flash.h"
	#include "./stub/stack.h"
#else
	#error "Unsupported/unknown architecture"
#endif

#ifndef KERNEL_ARCH
#define KERNEL_ARCH "unkn"
#endif

#ifndef KERNEL_MCU
#define KERNEL_MCU "unkn"
#endif

#endif /* KERNEL_ARCH_H_ */