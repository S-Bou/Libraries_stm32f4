
#ifndef __SERVOS_BUTTONS_H
#define __SERVOS_BUTTONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

#define usTIM TIM5

bool tempStore = true;
bool tempBall = true;

void PositionServo(uint8_t angle);
void ContinuousServo(uint8_t init, uint8_t finish);

#ifdef __cplusplus
}
#endif

#endif /* __SERVOS_BUTTONS_H */

