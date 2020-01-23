/*##########################################################################################################*/
/*
	CONECTIONS
	----------
	- ESP8266 for wifi -> USART1:
		RX   -> PA9  USART2_TX
		TX   -> PA10 USART2_RX
		VCC  -> 3.3V
		CHPD -> 3.3V
		GND  -> GND
*/

#include "sendwifi.h"

float distance = 66.66;
char page[200];
uint32_t numTicks;
uint8_t chanel=66;
uint8_t rxData[LONGDATA];
extern uint8_t	rojo, verde, azul, morado, total;

/*##############################################################################################*/
//When is completed a receive will enter here
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART3)
	{
    find_str(rxData);
		//HAL_UART_Transmit_DMA(&huart2, (uint8_t *)rxData, LONGDATA);	
		HAL_UART_Receive_DMA(&HUARTNUM, (uint8_t *)rxData, LONGDATA);
		
		HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
	}
}
/*##############################################################################################*/
void InitESP(void)
{
	char uartComAT[100];
	
	HAL_GPIO_WritePin(RedLed_GPIO_Port, RedLed_Pin, GPIO_PIN_SET);
	
	sprintf(uartComAT, "AT+CIPMUX=1\r\n");
	HAL_UART_Transmit(&HUARTNUM, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_Delay(2000);
	
	sprintf(uartComAT, "AT+CIPSERVER=1,80\r\n");
	HAL_UART_Transmit(&HUARTNUM, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_Delay(2000);
	
	HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RedLed_GPIO_Port, RedLed_Pin, GPIO_PIN_RESET);
}

/*##############################################################################################*/
void find_str(uint8_t dataRX[]) 
{ 
	HAL_GPIO_WritePin(OrangeLed_GPIO_Port, OrangeLed_Pin, GPIO_PIN_SET);
	int i=0;
	for(i=0; i<400; i++)
	{		
		if(dataRX[i]=='+' && dataRX[i+1]=='I' && dataRX[i+2]=='P' && dataRX[i+3]=='D')
		{
			chanel = dataRX[i+5];
		}
  }	
	HAL_GPIO_WritePin(OrangeLed_GPIO_Port, OrangeLed_Pin, GPIO_PIN_RESET);
}
/*##############################################################################################*/
void UpPage(void)											
{													//For reset page <head><meta http-equiv=\"refresh\" content=\"5\"><h1>
	char uartComAT[300];
	
  sprintf(page, 
	"<!doctype html><html><head><h1>Clasificación de colores, resultado:<br><h1></head>"
	"<body><h2>Rojo: %d<br>Verde: %d<br>Azul: %d<br>Morado: %d<br>Total: %d"
	"<h2></body></html>", rojo, verde, azul, morado, total);             	
	
	sprintf(uartComAT, "AT+CIPSEND=0,%d\r\n", strlen(page));
	HAL_UART_Transmit(&HUARTNUM, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_Delay(2000);
	sprintf(uartComAT, "%s\r\n", page);
	HAL_UART_Transmit(&HUARTNUM, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_Delay(3000);
	sprintf(uartComAT, "AT+CIPCLOSE=%d\r\n", chanel-48);
	HAL_UART_Transmit(&HUARTNUM, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	
	HAL_GPIO_WritePin(OrangeLed_GPIO_Port, OrangeLed_Pin, GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(OrangeLed_GPIO_Port, OrangeLed_Pin, GPIO_PIN_RESET);
}
/*##############################################################################################*/
