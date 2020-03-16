#ifndef __MYCONFIG_H
#define __MYCONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
/*################################### INCLUDES/DEFINES #####################################################*/
#include <stm32f4xx.h> 
/*################################### VARIABLES ############################################################*/

/*################################### FUNCTIONS ############################################################*/
void GPIO_Output_Config(void);
void GPIO_Input_Config(void);
void Configurar_interrupcion(void);
/*##########################################################################################################*/
#ifdef __cplusplus
}
#endif

#endif /* __MYCONFIG_H */

