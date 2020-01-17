
#ifndef __TCS34725_H
#define __TCS34725_H

#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern TIM_HandleTypeDef htim3;

void PositionServo(uint8_t angle);
void ContinuousServo(uint8_t init, uint8_t finish);

void Test_cts34725(void);
void Init_cts34725(void);
void Read_cts34725(void);
void Store_Colors(void);
void Show_console(void);

#endif /* __TCS34725_H */

