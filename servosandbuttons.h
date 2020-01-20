
#ifndef __SERVOSANDBUTTONS_H
#define __SERVOSANDBUTTONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

#define usTIM TIM5
#define POSUNO 24
#define POSDOS 60
#define POSTRES 95

#define SRROJO 100
#define SRVERDE 85
#define SRAZUL 70
#define SRMORADO 55
#define SRNARANJA 45

void ButtonOnePressed(void);
void ButtonTwoPressed(void);
void PositionServoSensor(uint8_t angle);
void PositionServoRamp(uint8_t angle);
void ContinuousServo(uint8_t init, uint8_t finish);

#ifdef __cplusplus
}
#endif

#endif /* __SERVOSANDBUTTONS_H */

