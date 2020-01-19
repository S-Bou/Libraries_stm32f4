
#ifndef __SERVOS_H
#define __SERVOS_H

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

#define usTIM TIM5

bool tempStore = true;
bool tempBall = true;

extern TIM_HandleTypeDef htim3;

void PositionServo(uint8_t angle);
void ContinuousServo(uint8_t init, uint8_t finish);

#endif /* __SERVOS_H */
