
#ifndef __SERVOSANDBUTTONS_H
#define __SERVOSANDBUTTONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

#define usTIM TIM5
#define UMBRAL 100

//Servo Sensor
#define POSUNO 24
#define POSDOS 60
#define POSTRES 95

//Servo Rampa
#define SRROJO 95
#define SRVERDE 80
#define SRAZUL 70
#define SRMORADO 60
//#define SRNARANJA 50
#define SRINDETERMINADO 50

void ButtonOnePressed(void);
void ButtonTwoPressed(void);
void PositionServoSensor(uint8_t angle);
void PositionServoRamp(uint8_t angle);
void ContinuousServo(uint8_t init, uint8_t finish);

#ifdef __cplusplus
}
#endif

#endif /* __SERVOSANDBUTTONS_H */

