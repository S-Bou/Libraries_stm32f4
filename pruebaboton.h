#ifndef __PRUEBABOTON_H
#define __PRUEBABOTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdbool.h>

#define PORTBUTON GPIOA
#define PINBUTON GPIO_PIN_0

extern TIM_HandleTypeDef htim1;

void ButtonUserPressed(void);
void pruebaEXTint(void);
void Delay_us(uint16_t us);

#ifdef __cplusplus
}
#endif

#endif /* __PRUEBABOTON_H */

