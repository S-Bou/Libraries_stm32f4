
#include "tcs34725.h"

/*##########################################################################################################*/
/*
	CONECTIONS
	----------
	- Sensor of colors -> I2C1:
		SCL -> PB10 I2C1_SCL Serial clock line
		SDA -> PB11 I2C1_SDA Serial data line
		LED -> PB9 GPIO_Output output level -> Low
		3v3 -> 3.3V or VIN -> 5v
		GND -> GND
		
	- FTDI232 for serial comunication -> USART2:
		RX -> PA2 USART2_TX
		TX -> PA3 USART2_RX
*/
/*##########################################################################################################*/
/*
	The slave address of the color sensor is 41 = 0x29.
	
	A. Test if we're connecting with the color sensor.
	   Read the id register (address code: 146 = 0x92) and check if we're communicating 
		 with the device, the id register value should be 68 = 0x44.
*/

extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;
extern uint8_t buttoncalibrate;

unsigned int Color_Sensor_Address = 0x29<<1;

uint8_t ciclecolor = 0;
uint8_t takingsamples = 0;
uint8_t rojo=0, verde=0, azul=0, morado=0, total=0;
uint16_t Clear_value, Red_value, Green_value, Blue_value;
uint32_t color = 0;
uint32_t ColorsThreshold[4] = {0, 0, 0, 0};

/*##########################################################################################################*/
void Test_cts34725(void)
{
	unsigned char buffer[1] = {0x92};
	HAL_I2C_Master_Transmit(&hi2c2, Color_Sensor_Address, buffer, 1, 100);

	unsigned char data[1] = {0};
	HAL_I2C_Master_Receive(&hi2c2, Color_Sensor_Address, data, 1, 100);
	
	if(data[0] == 68)
	{
		HAL_GPIO_WritePin(BlueLed_GPIO_Port, BlueLed_Pin, GPIO_PIN_SET);
		HAL_Delay(2000);
		HAL_GPIO_WritePin(BlueLed_GPIO_Port, BlueLed_Pin, GPIO_PIN_RESET);
		HAL_Delay(2000);
	}else 
	{
		HAL_GPIO_WritePin(BlueLed_GPIO_Port, BlueLed_Pin, GPIO_PIN_SET);
	}
}
/*##########################################################################################################*/
/*
	B. Initialize the color sensor
*/
void Init_cts34725(void)
{
	//1) Write the RGBC Timing Register (address code: 129 = 0x81) to 0 -> sets to maximun sensitivity.
	unsigned char Timing_register[2] = {0x81, 0xD5};
	HAL_I2C_Master_Transmit(&hi2c2, Color_Sensor_Address, Timing_register, 2, 100);
	
	//2) Write the Control Register (address code: 143 = 0x8F) to 0 -> sets the gain to 1.
	unsigned char Control_register[2] = {0x8F, 0x00};
	HAL_I2C_Master_Transmit(&hi2c2, Color_Sensor_Address, Control_register, 2, 100);
	
	//3) Write the Enable Register (address code: 128 = 0x80) to 3 -> enables the ADC and turns the device on.
	unsigned char Enable_register[2] = {0x80, 0x03};
	HAL_I2C_Master_Transmit(&hi2c2, Color_Sensor_Address, Enable_register, 2, 100);
}
/*##########################################################################################################*/
/*
	C. Read data from the color sensor
*/
void Read_cts34725(void)
{
	//1) Read 2 bytes of CLEAR Data (address code: 148 = 0x94)
	unsigned char Clear_register[1] = {0x94};
	HAL_I2C_Master_Transmit(&hi2c2, Color_Sensor_Address, Clear_register, 1, 100);
	unsigned char Clear_data[2] = {0,0};
	HAL_I2C_Master_Receive(&hi2c2, Color_Sensor_Address, Clear_data, 2, 100);
	//16-bits, but we got it into 2 bytes (8-bits)
	Clear_value = (((int) Clear_data[1]) << 8) | Clear_data[0];
	
	//2) Read 2 bytes of RED Data (address code: 150 = 0x96)
	unsigned char Red_register[1] = {0x96};
	HAL_I2C_Master_Transmit(&hi2c2, Color_Sensor_Address, Red_register, 1, 100);
	unsigned char Red_data[2] = {0,0};
	HAL_I2C_Master_Receive(&hi2c2, Color_Sensor_Address, Red_data, 2, 100);
	//16-bits, but we got it into 2 bytes (8-bits)
	Red_value = (((int) Red_data[1]) << 8) | Red_data[0];
	
	//3) Read 2 bytes of GREEN Data (address code: 152 = 0x98)
	unsigned char Green_register[1] = {0x98};
	HAL_I2C_Master_Transmit(&hi2c2, Color_Sensor_Address, Green_register, 1, 100);
	unsigned char Green_data[2] = {0,0};
	HAL_I2C_Master_Receive(&hi2c2, Color_Sensor_Address, Green_data, 2, 100);
	//16-bits, but we got it into 2 bytes (8-bits)
	Green_value = (((int) Green_data[1]) << 8) | Green_data[0];
	
	//4) Read 2 bytes of BLUE Data (address code: 154 = 0x9A)
	unsigned char Blue_register[1] = {0x9A};
	HAL_I2C_Master_Transmit(&hi2c2, Color_Sensor_Address, Blue_register, 1, 100);
	unsigned char Blue_data[2] = {0,0};
	HAL_I2C_Master_Receive(&hi2c2, Color_Sensor_Address, Blue_data, 2, 100);
	//16-bits, but we got it into 2 bytes (8-bits)
	Blue_value = (((int) Blue_data[1]) << 8) | Blue_data[0];
	
	//5) Claculate theshold of color in sensor.
	color = ((Red_value*65536 + Green_value*256 + Blue_value)/Clear_value)/10;
}
/*##########################################################################################################*/
/*
	It save the threshold of each color for adapt to the environment of light.
	And it show on the console.
*/
void Store_Colors(void)
{
//	if(count == 0){PositionServoSensor(POSUNO);}	
//	char uartComAT[100];
//	
//	if(count == 0)
//		{
//			ColorsThreshold[0] = color;
//			MY_FLASH_WriteN(0, ColorsThreshold, 4, DATA_TYPE_32);
//			sprintf(uartComAT, "Init process for store value of colors.\nWaiting color Red:\r\n");
//			HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
//		}
//	
//	if(count == 1)
//		{
//			ColorsThreshold[1] = color;
//			MY_FLASH_WriteN(0, ColorsThreshold, 4, DATA_TYPE_32);
//			sprintf(uartComAT, "Waiting color Green:\r\n");
//			HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
//		}
//	
//	if(count == 2){count = 0;}
	
}
/*##########################################################################################################*/
void Show_console(void)
{
	char uartComAT[100];
	
	sprintf(uartComAT, "C: %d   ", Clear_value);
	HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
	
	sprintf(uartComAT, "R: %d   ", Red_value);
	HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
	
	sprintf(uartComAT, "G: %d   ", Green_value);
	HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
	
	sprintf(uartComAT, "B: %d   ", Blue_value);
	HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
	
	sprintf(uartComAT, "T: %d   ", color);
	HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);		
		
	if(buttoncalibrate == 0)
	{
		DefineColour(color);
	}else
	{
		PositionServoRamp(SRAZUL);
		sprintf(uartComAT, " -> Calibrando Sensor");
		HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
	}
	
	sprintf(uartComAT, "\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
	
}
/*##########################################################################################################*/
void CicleColor(void)
{
	for(uint8_t i=0; i<12; i++)
	{
	HAL_GPIO_WritePin(LedSensor_GPIO_Port, LedSensor_Pin, GPIO_PIN_SET);
	PositionServoSensor(POSDOS);	//Positions degrees
	HAL_Delay(1000);
		
	Read_cts34725();
  HAL_Delay(1000);
	Show_console();
	
	PositionServoSensor(POSTRES);	//Positions degrees
	HAL_Delay(500);
		
	HAL_GPIO_WritePin(LedSensor_GPIO_Port, LedSensor_Pin, GPIO_PIN_RESET);
	PositionServoSensor(POSUNO);	//Positions degrees
	HAL_Delay(1000);
	}
}
/*##########################################################################################################*/
void mapData(void)
{	
//	
//	Red = ((double)Red_value/(double)Clear_value)*256.0;
//	Green = ((double)Green_value/(double)Clear_value)*256.0;
//	Blue = ((double)Blue_value/(double)Clear_value)*256.0;
//	
//	color = Red*65536 + Green*256 + Blue;
}
/*##########################################################################################################*/
void CalibrateColour(void)
{	
	char uartComAT[100];
	if(buttoncalibrate == 0)
	{
		sprintf(uartComAT, "Put balls in this order: Red, Green, Blue and Purple.\r\n"
		                   "And press button 1 ...\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
		
		SSD1306_GotoXY (0,10);  
		SSD1306_Puts ("Put balls in this order:", &Font_7x10, 1); 
		SSD1306_GotoXY (0,20);  
		SSD1306_Puts ("order: Red, Green, ", &Font_7x10, 1);		
		SSD1306_GotoXY (0,30);  
		SSD1306_Puts ("Blue and Purple.", &Font_7x10, 1);  
		SSD1306_GotoXY (0,40);  
		SSD1306_Puts ("And press button ", &Font_7x10, 1);  
		SSD1306_GotoXY (0,50);  
		SSD1306_Puts ("Acept...", &Font_7x10, 1); 		
		SSD1306_UpdateScreen(); 
		
		buttoncalibrate = 1;
	}else
	{
		sprintf(uartComAT, "Saving color samples...\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	

		
		for(uint8_t i=0; i<4; i++)
		{
			HAL_GPIO_WritePin(LedSensor_GPIO_Port, LedSensor_Pin, GPIO_PIN_SET);
			PositionServoSensor(POSDOS);	//Positions degrees
			HAL_Delay(1000);
		
			Read_cts34725();
			HAL_Delay(1000);
			Show_console();
	
			ColorsThreshold[i] = color;
			
			PositionServoSensor(POSTRES);	//Positions degrees
			HAL_Delay(500);
		
			HAL_GPIO_WritePin(LedSensor_GPIO_Port, LedSensor_Pin, GPIO_PIN_RESET);
			PositionServoSensor(POSUNO);	//Positions degrees
			HAL_Delay(1000);
			}
		
		buttoncalibrate = 0;
		sprintf(uartComAT, "Color sampling is finished.\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
	}	
}
/*##########################################################################################################*/
void DefineColour(uint32_t colour)
{	
	char uartComAT[100];
	rojo=0; verde=0; azul=0; morado=0; total=0;
//	uint32_t UMBRAL = ColorsThreshold[1] - ColorsThreshold[2];
	
	if( colour <= (ColorsThreshold[0]+UMBRAL) && colour >= (ColorsThreshold[0]-UMBRAL))
	{
		PositionServoRamp(SRROJO);
		sprintf(uartComAT, "= Red");
		HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
		rojo++;
	}
	else if( colour <= (ColorsThreshold[1]+UMBRAL) && colour >= (ColorsThreshold[1]-UMBRAL))
	{
		PositionServoRamp(SRVERDE);
		sprintf(uartComAT, "= Green");
		HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);
		verde++;
	}
	else if( colour <= (ColorsThreshold[2]+UMBRAL) && colour >= (ColorsThreshold[2]-UMBRAL))
	{
		PositionServoRamp(SRAZUL);
		sprintf(uartComAT, "= Blue");
		HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
		azul++;
	}
	else if( colour <= (ColorsThreshold[3]+UMBRAL) && colour >= (ColorsThreshold[3]-UMBRAL))
	{
		PositionServoRamp(SRMORADO);
		sprintf(uartComAT, "= Purple");
		HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
		morado++;
	}
//	else if( colour <= (ColorsThreshold[4]+UMBRAL) && colour >= (ColorsThreshold[4]-UMBRAL))
//	{
//		PositionServoRamp(SRNARANJA);
//		sprintf(uartComAT, "= Orange");
//		HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
//	}
	else
	{
		PositionServoRamp(SRINDETERMINADO);
		sprintf(uartComAT, " Indeterminate");
		HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
	}
	
	SSD1306_GotoXY (10,10);                     
	sprintf(uartComAT, "Red:    %d", rojo);
	SSD1306_Puts (uartComAT, &Font_7x10, 1);   
	SSD1306_GotoXY (10,20);                 
	sprintf(uartComAT, "Green:  %d", verde);
	SSD1306_Puts (uartComAT, &Font_7x10, 1);   
	SSD1306_GotoXY (10,30);               
	sprintf(uartComAT, "Blue:   %d", azul);
	SSD1306_Puts (uartComAT, &Font_7x10, 1);   
	SSD1306_GotoXY (10,40);                
	sprintf(uartComAT, "Purple: %d", morado);
	SSD1306_Puts (uartComAT, &Font_7x10, 1);   
	SSD1306_GotoXY (10,50);  
	total = rojo+verde+azul+morado;
	sprintf(uartComAT, "Total:  %d", total);
	SSD1306_Puts (uartComAT, &Font_7x10, 1);   
	SSD1306_UpdateScreen();                   
}
/*##########################################################################################################*/

