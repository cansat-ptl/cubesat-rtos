/*
 * archtypes.h
 * 
 * Created: 15.06.2021 11:56:12
 * Author: ThePetrovich
 */


#ifndef KERNEL_ARCHTYPES_H_
#define KERNEL_ARCHTYPES_H_

#ifdef KERNEL_ARCH_avr
	#if defined KERNEL_MCU_atmega128
		#include "./mega128/types.h"
	#elif defined KERNEL_MCU_atmega2560
		#include "./mega2560/types.h"
	#else
		#error "Unsupported/unknown AVR MCU"
	#endif
#else
	#error "Unsupported/unknown architecture"
#endif

#endif
