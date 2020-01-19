
#ifndef __TCS3200_H
#define __TCS3200_H

#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define RED 0
#define BLUE 1
#define CLEAR 2
#define GREEN 3
#define TIME 10

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void RegisterColor(int color);
void StoreColor(void);
void CalculeColor(void);

#endif /* __TCS3200_H */

