
#include "tcs3200.h"

uint8_t red = 0;
uint8_t blue = 0;
uint8_t clear = 0;
uint8_t green = 0;
uint32_t color = 0;

uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
uint8_t Is_First_Capture = 0;	// Is the first captured?

extern UART_HandleTypeDef huart2;

/*##############################################################################################*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)	// If the interrupt source is channel 2
	{
		if(Is_First_Capture==0)	// If the first value is not captured
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);	// Read the first value
			Is_First_Capture = 1;	// Set the first captured as true
			// Now changue the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if(Is_First_Capture==1)	// If the first is already captured
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);	// Read the second value
			__HAL_TIM_SET_COUNTER(htim, 0);	// Reset the counter
				
			if(IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2-IC_Val1;
			}
				
			Is_First_Capture = 0;	// Set it back to false
				// Set polarity to rising edge
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);	
		}
	}
}
/*##############################################################################################*/
void RegisterColor(int color)
{
		switch (color)
    {
    	case RED:
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_RESET);
				break;
			
    	case BLUE:
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_SET);
    		break;
			
			case CLEAR:		
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_RESET);
    		break;
			
    	case GREEN:
				HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_SET);
    		break;
			
    	default:
    		break;
    }
		
}
/*##############################################################################################*/
void StoreColor(void)
{
	char uartComAT[100];		
				
	HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
			
	RegisterColor(RED);
	sprintf(uartComAT, "Red: %d   ", Difference);
				red = Difference;
				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
				HAL_Delay(TIME);
	
				RegisterColor(GREEN);
				sprintf(uartComAT, "Green: %d   ", Difference);
				green = Difference;
				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
				HAL_Delay(TIME);
		
				RegisterColor(BLUE);
				sprintf(uartComAT, "Blue: %d   ", Difference);
				blue = Difference;
				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
				HAL_Delay(TIME);
		
				RegisterColor(CLEAR);
				sprintf(uartComAT, "Clear: %d  ", Difference);
				clear = Difference;
				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
				HAL_Delay(TIME);
			
				color = red*65536 + green*256 + blue;
				sprintf(uartComAT, "Color: %d", color);
				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
				HAL_Delay(TIME);
				
				sprintf(uartComAT, "\r\n");
				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
				HAL_Delay(TIME);
				
				HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
}
/*##############################################################################################*/
void CalculeColor(void)
{	
	double r,g,b; // r,v,b € [0..1]
			
	double offset = 3.0/clear; // compensacion de luminosidad
	
	if((offset+(clear/red)) < 1)
	{
		r = offset+(clear/red);
	}else{r = 1.0;}

	if((offset+(clear/green)) < 1)
	{
		g = offset+(clear/green);
	}else{g = 1.0;}
	
	if((offset+(clear/blue)) < 1)
	{
		b = offset+(clear/blue);
	}else{b = 1.0;}

	// transformacion RVB -> TSL
	// r,v,b € [0..1]
	// t € [0°..360°]; s,l € [%]
	double t,s,l;
	double maxRGB, minRGB;

		if(r >= b){maxRGB = r;
		}else{maxRGB = b;}
		if(g >= maxRGB){maxRGB = g;}
		
		if(r <= b){minRGB = r;
		}else{minRGB = b;}
		if(g <= minRGB){minRGB = g;}
	
	double delta = maxRGB-minRGB;
	double somme = maxRGB+minRGB;

 // luminancia basada en valores de colores segun software de diseño grafico
 l=(somme/2.0);

 if(delta==0.0) // gris
 {
 t=s=0.0;
 }else{
 //saturacion
 if ( l < 0.5 ) s = delta / somme;
 else s = delta / ( 2.0 - delta );

 // tinte
 double del_R = ( ( ( maxRGB - r ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
 double del_G = ( ( ( maxRGB - g ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
 double del_B = ( ( ( maxRGB - b ) / 6.0 ) + ( delta / 2.0 ) ) / delta;

 if ( r == maxRGB ) t = del_B - del_G;
 else if ( g == maxRGB ) t = ( 1.0 / 3.0 ) + del_R - del_B;
 else if ( b == maxRGB ) t = ( 2.0 / 3.0 ) + del_G - del_R;

 if ( t < 0 ) t += 1.0;
 if ( t > 1 ) t -= 1.0;
 }

 // normalizacion
t*=360.0; // [0°..360°]
s*=100.0; // [%]
l*=100.0; // [%]
 
// 				sprintf(uartComAT, "Color: %f", t);
//				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
//				HAL_Delay(TIME);
// 
// 				sprintf(uartComAT, "\r\n");
//				HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
//				HAL_Delay(TIME);
			
}
/*##############################################################################################*/
		