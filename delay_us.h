
#ifndef __DELAY_US_H
#define __DELAY_US_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include <stdio.h>

#define usTIM TIM9

extern TIM_HandleTypeDef htim9;

void usDelay(uint32_t uSec);
void Delay_us(uint16_t us);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_US_H */

