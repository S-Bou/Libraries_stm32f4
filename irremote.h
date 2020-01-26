
#ifndef __IRREMOTE_H
#define __IRREMOTE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include <stdint.h>
#include <string.h>
#include "delay_us.h"

uint32_t receive_data (void);
void convert_code (uint32_t code);

#ifdef __cplusplus
}
#endif

#endif /* __IRREMOTE_H */

