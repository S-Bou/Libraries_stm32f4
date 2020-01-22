
#include "servosandbuttons.h"
#include "tcs34725.h"

/*##########################################################################################################*/
/*
	CONECTIONS
	----------
	- Servo sensor of colours: -> PA6 TIM3_CH1
	- Servo ramp: --------------> PA7 TIM3_CH2
	- Button 1: ----------------> PE0 GPIO_Input with Pull-up
	- Button 2: ----------------> PE5 GPIO_Input with Pull-up
*/
/*##########################################################################################################*/

bool tempStore = true;
bool tempBall = true;
uint8_t buttoncalibrate = 0;
uint8_t buttontwopulsed = 0;
extern TIM_HandleTypeDef htim3;
extern uint8_t rojo, verde, azul, morado, total;

/*########## FUNCTIONS FOR SERVOS ###############################################################*/
/*
Steps for config timer:
	- Select timer and chanel, in this case TIM3 Channel 1.
	- Select in channel PWM Generation CH1 ->X (This establishes pin PA6 as output).
	- Need pulses for 20 ms frecuency 50 Hz, to do this configure the below parameters:
          In this case in clock configuration I have in APB1 Timer clocks 72 MHz.
          72 MHz / 50 Hz = 1 440 000 -> Preescaler = 1440-1 and ARR = 1000-1.
          Prescaler (PSC - 16 bits value) = 1440-1 
          Counter Period (ARR)            = 1000-1
	- Remember declaring function that start PWM in main file, in this case: timer 3, chanel 1:
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
*/
void PositionServoSensor(uint8_t angle)	
{
	htim3.Instance->CCR1 = angle;
}

void PositionServoRamp(uint8_t angle)	
{
	htim3.Instance->CCR2 = angle;
}

void ContinuousServo(uint8_t init, uint8_t finish)
{
	for(uint8_t i=init; i<finish; i++)
	{
		htim3.Instance->CCR1 = i;
		HAL_Delay(100);
	}
}
/*########## FUNCTIONS FOR BUTTONS ##############################################################*/
/*	
	This function reduces the posibility for the button do multiple signals.
	In this proyect I establishes the pin PE0 as input (Pull-Up), with the user name "ButtonOne"
*/
void ButtonOnePressed(void)
{
		if(HAL_GPIO_ReadPin(ButtonOne_GPIO_Port, ButtonOne_Pin) && tempBall)
	{
		HAL_Delay(5);
		if(HAL_GPIO_ReadPin(ButtonOne_GPIO_Port, ButtonOne_Pin))
		{
			HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
			tempBall = false;
		}
	}
	else if(HAL_GPIO_ReadPin(ButtonOne_GPIO_Port, ButtonOne_Pin)==GPIO_PIN_RESET)
	{
		HAL_Delay(5);
		if(HAL_GPIO_ReadPin(ButtonOne_GPIO_Port, ButtonOne_Pin)==GPIO_PIN_RESET)
		{
			HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
			//Acction when push the button:
			ButtonTwoAction();
			tempBall = true;
		}
	}
}
/*##########################################################################################################*/
/*
	This function reduces the posibility for the button do multiple signals.
	In this proyect I establishes the pin PC13 as input (Pull-Up), with the user name "ButtonTwo"
*/
void ButtonTwoPressed(void)
{
		if(HAL_GPIO_ReadPin(ButtonTwo_GPIO_Port, ButtonTwo_Pin) && tempStore)
	{
		HAL_Delay(50);
		if(HAL_GPIO_ReadPin(ButtonTwo_GPIO_Port, ButtonTwo_Pin))
		{
			HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_RESET);
			tempBall = false;
		}
	}
	else if(HAL_GPIO_ReadPin(ButtonTwo_GPIO_Port, ButtonTwo_Pin)==GPIO_PIN_RESET)
	{
		HAL_Delay(50);
		if(HAL_GPIO_ReadPin(ButtonTwo_GPIO_Port, ButtonTwo_Pin)==GPIO_PIN_RESET)
		{
			HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
		  //Acction when push the button:
			ButtonTwoMenu();
			if(buttoncalibrate == 1){CalibrateColour();}
			tempBall = true;
		}
	}
}
/*##########################################################################################################*/
/*
	This function reduces the posibility for the button do multiple signals.
	In this proyect I establishes the pin PC13 as input (Pull-Up), with the user name "ButtonTwo"
*/
void ButtonTwoAction(void)
{
	if(buttontwopulsed == 1)	//Select in menu sorter
	{
		SSD1306_Clear();
		CicleColor();
	}
	if(buttontwopulsed == 2)	//Select in menu calibration
	{
		SSD1306_Clear();
		CalibrateColour();
	}
	if(buttontwopulsed == 3)	//Select in menu reset counter
	{
		SSD1306_Clear();
		rojo=0; verde=0; azul=0; morado=0; total=0;
		
	}
	if(buttontwopulsed == 4)	//Select in menu send to web
	{
		SSD1306_Clear();
		
		UpPage();
	}
}
/*##########################################################################################################*/
/*
	Show menu in screen oled.
	In this proyect I establishes the pin PC13 as input (Pull-Up), with the user name "ButtonTwo"
*/
void ButtonTwoMenu(void)
{
	buttontwopulsed++;
	char uartComAT[100];
	
	if(buttontwopulsed == 1)
	{
		SSD1306_Clear();
		SSD1306_GotoXY (14,2);                    
		sprintf(uartComAT, "Menu:");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);  
		SSD1306_GotoXY (14, 16);                 
		sprintf(uartComAT, "1. Sorter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 28);               
		sprintf(uartComAT, "2. Calibration");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 41);                
		sprintf(uartComAT, "3. Reset counter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 53);                
		sprintf(uartComAT, "4. Send to web");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);
		SSD1306_DrawFilledCircle(5, 19, 4, 1);
		SSD1306_UpdateScreen(); 
	}
	else if(buttontwopulsed == 2)
	{
		SSD1306_Clear();
		SSD1306_GotoXY (14,2);                    
		sprintf(uartComAT, "Menu:");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);  
		SSD1306_GotoXY (14, 16);                 
		sprintf(uartComAT, "1. Sorter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 28);               
		sprintf(uartComAT, "2. Calibration");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 41);                
		sprintf(uartComAT, "3. Reset counter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 53);                
		sprintf(uartComAT, "4. Send to web");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);  
		SSD1306_DrawFilledCircle(5, 31, 4, 1);
		SSD1306_UpdateScreen(); 
	}
	else if(buttontwopulsed == 3)
	{
		SSD1306_Clear();
		SSD1306_GotoXY (14,2);                    
		sprintf(uartComAT, "Menu:");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);  
		SSD1306_GotoXY (14, 16);                 
		sprintf(uartComAT, "1. Sorter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 28);               
		sprintf(uartComAT, "2. Calibration");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 41);                
		sprintf(uartComAT, "3. Reset counter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 53);                
		sprintf(uartComAT, "4. Send to web");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);  
		SSD1306_DrawFilledCircle(5, 43, 4, 1);
		SSD1306_UpdateScreen(); 
	}
	else if(buttontwopulsed == 4)
	{
		SSD1306_Clear();
		SSD1306_GotoXY (14,2);                    
		sprintf(uartComAT, "Menu:");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);  
		SSD1306_GotoXY (14, 16);                 
		sprintf(uartComAT, "1. Sorter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 28);               
		sprintf(uartComAT, "2. Calibration");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 41);                
		sprintf(uartComAT, "3. Reset counter");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);   
		SSD1306_GotoXY (14, 53);                
		sprintf(uartComAT, "4. Send to web");
		SSD1306_Puts (uartComAT, &Font_7x10, 1);  
		SSD1306_DrawFilledCircle(5, 56, 4, 1);
		SSD1306_UpdateScreen(); 
	}
	
	if(buttontwopulsed == 5){buttontwopulsed = 0;}
}
/*########## FUNCTIONS FOR usDELAYS #############################################################*/
//void usDelay(uint32_t uSec)
//	{													//We need define timer-> ejem: #define usTIM TIM5
//	if(uSec < 2) uSec = 2;
//	usTIM->ARR = uSec - 1;		/*Sets the value in the auto-reload register*/
//	usTIM->EGR = 1;						/*Re-initialiser the timer*/
//	usTIM->SR &= ~1;					//Reset the flag
//	usTIM->CR1 |= 1;					//Enables the counter
//	while((usTIM->SR&0x0001) != 1);
//	usTIM->SR &= ~(0x0001);
//}
/*##############################################################################################*/
//void Delay_us(uint16_t us)
//{
//	__HAL_TIM_SET_COUNTER(&htim1, 0); //Set the counter value to 0
//	while(__HAL_TIM_GET_COUNTER(&htim1) < us);	//Wait for the counter to reach the us input in the parameter
//}
/*###############################################################################################*/
