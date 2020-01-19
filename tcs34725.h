
#ifndef __TCS34725_H
#define __TCS34725_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "stm32f4xx_hal.h"
#include "writeFlash.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern TIM_HandleTypeDef htim3;

void Test_cts34725(void);
void Init_cts34725(void);
void Read_cts34725(void);
void Store_Colors(void);
void Show_console(void);
void CicleColor(void);
void StartThreshold(void);


#ifdef __cplusplus
}
#endif

#endif /* __TCS34725_H */

