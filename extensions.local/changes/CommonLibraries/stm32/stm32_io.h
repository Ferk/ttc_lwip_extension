#ifndef STM32_IO_H
#define STM32_IO_H

//{ includes

#include "ttc_basic.h"
//? #include "stm32_gpio.h"
//? #include <stdlib.h>
//? #include "stm32f10x.h"
//? #include "core_cm3.h"
//? #include "stm32f10x_conf.h"
//? #include "stm32f10x_gpio.h"

#include "stddef.h" // for size_t

//}includes
//{ commonly used defines (required by FreeRTOS and/or StdPeripheralsLibrary)

//} Defines

u16_t* arraySet(u16_t* Array, u16_t Value, u32_t Amount);
bool circa(u32_t A, u32_t B, u8_t Distance);
u16_t toggleValue(u16_t Value, u16_t Max, u16_t Min);

// limits given 
s16_t sLimit(s16_t Value, s16_t Min, s16_t Max);
u16_t uLimit(u16_t Value, u16_t Min, u16_t Max);

// malloc replacement; uses Assert() for out-of-memory detection
void* myMalloc(size_t Size);


#endif
