/*##########################################################################################################*/
/*
	CONECTIONS
	----------
	- ESP8266 for wifi -> USART2:
		RX   -> PA2 USART2_TX
		TX   -> PA3 USART2_RX
		VCC  -> 3.3V
		CHPD -> 3.3V
		GND  -> GND
*/

#include "sendwifi.h"

float distance = 0;
char uartComAT[100];
char page[200];
uint32_t numTicks;
extern UART_HandleTypeDef huart2;
uint8_t chanel=66;
uint8_t rxData[LONGDATA];

/*##############################################################################################*/
//When is completed a receive will enter here
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
    find_str(rxData);
		//HAL_UART_Transmit_DMA(&huart2, (uint8_t *)rxData, LONGDATA);	
		HAL_UART_Receive_DMA(&huart2, (uint8_t *)rxData, LONGDATA);
		
		HAL_GPIO_WritePin(BlueLed_GPIO_Port, BlueLed_Pin, GPIO_PIN_SET);
	}
}
/*##############################################################################################*/
void InitESP(void)
{
	sprintf(uartComAT, "AT+CIPMUX=1\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_GPIO_TogglePin(GreenLed_GPIO_Port, GreenLed_Pin);	//Led green
	HAL_Delay(2000);
	sprintf(uartComAT, "AT+CIPSERVER=1,80\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_GPIO_TogglePin(GreenLed_GPIO_Port, GreenLed_Pin);	//Led green
	HAL_Delay(2000);
	
	HAL_GPIO_WritePin(RedLed_GPIO_Port, RedLed_Pin, GPIO_PIN_SET);
}

/*##############################################################################################*/
void find_str(uint8_t dataRX[]) 
{ 
	int i=0;
	for(i=0; i<400; i++)
	{		
		if(dataRX[i]=='+' && dataRX[i+1]=='I' && dataRX[i+2]=='P' && dataRX[i+3]=='D')
		{
			chanel = dataRX[i+5];
			HAL_GPIO_TogglePin(GreenLed_GPIO_Port, GreenLed_Pin);	//Green	
		}
  }	
	HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
}
/*##############################################################################################*/
void UpPage(void)
{
  sprintf(page, "<!doctype html><html><head><meta http-equiv=\"refresh\" content=\"5\"></head><body>"
	"<h1>Esto es una prueba de funcionamiento."	
	"<br>"								             
	"Distancia: %.2f"
	"</h1></body></html>", distance);             	
	
	sprintf(uartComAT, "AT+CIPSEND=0,%d\r\n", strlen(page));
	HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_Delay(2000);
	sprintf(uartComAT, "%s\r\n", page);
	HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_Delay(3000);
	sprintf(uartComAT, "AT+CIPCLOSE=%d\r\n", chanel-48);
	HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	HAL_GPIO_WritePin(OrangeLed_GPIO_Port, OrangeLed_Pin, GPIO_PIN_SET);
}
/*##############################################################################################*/
