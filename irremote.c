
#include "irremote.h"

/*##########################################################################################################*/
/*
	CONECTIONS
	----------

*/
/*##########################################################################################################*/	
uint32_t data;
uint8_t count;
char uartBufH[100];
extern UART_HandleTypeDef huart2;
/*##########################################################################################################*/	
uint32_t receive_data (void)
{
	uint32_t code=0;
	
		  /* The START Sequence begin here
	   * there will be a pulse of 9ms LOW and
	   * than 4.5 ms space (HIGH)
	   */
	  while (!(HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1)));  // wait for the pin to go high.. 9ms LOW
	
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);

	  while ((HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1)));  // wait for the pin to go low.. 4.5ms HIGH

	  /* START of FRAME ends here*/

	  /* DATA Reception
	   * We are only going to check the SPACE after 562.5us pulse
	   * if the space is 562.5us, the bit indicates '0'
	   * if the space is around 1.6ms, the bit is '1'
	   */

	  for (int i=0; i<32; i++)
	  {
		  count=0;

		  while (!(HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1))); // wait for pin to go high.. this is 562.5us LOW

		  while ((HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1)))  // count the space length while the pin is high
		  {
			  count++;
			  Delay_us(100);
		  }

		  if (count > 12) // if the space is more than 1.2 ms
		  {
			  code |= (1UL << (31-i));   // write 1
		  }

		  else code &= ~(1UL << (31-i));  // write 0
	  }
		
		return code;
}
/*##########################################################################################################*/
void convert_code (uint32_t code)
{
		switch (code)
		{
			case (0xFFA25D):
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
				break;
			
			case (0xFF629D):
		sprintf(uartBufH, "%d", code);
		HAL_UART_Transmit(&huart2, (uint8_t *)uartBufH, strlen(uartBufH), 100); 
				break;			

			case (0xFFE21D):

				break;

			case (0xFF22DD):
	
				break;

			case (0xFF02FD):

				break;

			case (0xFFC23D):

				break;

			case (0xFFE01F):

				break;

			case (0xFFA857):

				break;

			case (0xFF906F):

				break;

			case (0xFFB04F):

				break;

			case (0XFF6897):

				break;			
						
			case (0xFF9867):

				break;

			case (0xFF38C7):

				break;

			case (0xFF18E7):

				break;
			
			case (0xFF10EF):

				break;			

			case (0xFF5AA5):

				break;
			
			case (0xFF4AB5):

				break;
			
			default :
				break;
		}
}
/*##########################################################################################################*/
