
#ifndef __SERVOSANDBUTTONS_H
#define __SERVOSANDBUTTONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "ssd1306.h"
#include "sendwifi.h"
#include <stdint.h>
#include <stdbool.h>

#define usTIM TIM5

//Servo Sensor
#define POSUNO 24
#define POSDOS 60
#define POSTRES 95

//Servo Rampa
#define SRINICIO 67
#define SRROJO 84
#define SRVERDE 73
#define SRAZUL 62
#define SRMORADO 53
//#define SRNARANJA 50
#define SRINDETERMINADO 50

void ButtonOnePressed(void);
void ButtonTwoPressed(void);
void PositionServoSensor(uint8_t angle);
void PositionServoRamp(uint8_t angle);
void ContinuousServo(uint8_t init, uint8_t finish);
void ButtonTwoMenu(void);
void ButtonTwoAction(void);
void calibrateServos(void);

#ifdef __cplusplus
}
#endif

#endif /* __SERVOSANDBUTTONS_H */

