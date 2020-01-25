/*########## FUNCTIONS FOR usDELAYS ########################################################################*/

#include "delay_us.h"

/*##########################################################################################################*/
/*
	For use this function we need define the timer used-> ejem: #define usTIM TIM5 in header file
*/
/*##########################################################################################################*/
void usDelay(uint32_t uSec)
	{												
	if(uSec < 2) uSec = 2;
	usTIM->ARR = uSec - 1;		/*Sets the value in the auto-reload register*/
	usTIM->EGR = 1;						/*Re-initialiser the timer*/
	usTIM->SR &= ~1;					//Reset the flag
	usTIM->CR1 |= 1;					//Enables the counter
	while((usTIM->SR&0x0001) != 1);
	usTIM->SR &= ~(0x0001);
}
/*##########################################################################################################*/
/*
	For use this function we have configure a timer of 16 bits with the following parameters:
	- Internal clock: 72 MHz -> A PBX timer clock
	- Prescaler (PSC - 16 bits): 72-1
  - Counter period (ARR): 0xFFFF-1

  Also need define as external the variable of timer choose: extern TIM_HandleTypeDef htimX 
  and library stdio.h for define variable tipe uintX_t.
	Also we need call function HAL_TIM_Base_Start(&htimX) in main function.
*/
/*##########################################################################################################*/
void Delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim9, 0);           //Set the counter value to 0
	while(__HAL_TIM_GET_COUNTER(&htim9) < us);	//Wait for the counter to reach the us input in the parameter
}
/*##########################################################################################################*/
