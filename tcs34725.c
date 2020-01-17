/*##########################################################################################################*/
#include "tcs34725.h"

/*
	The slave address of the color sensor is 41 = 0x29.
	
	A. Test if we're connecting with the color sensor.
	   Read the id register (address code: 146 = 0x92) and check if we're communicating 
		 with the device, the id register value should be 68 = 0x44.
*/
uint8_t count = 0;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;
unsigned int Color_Sensor_Address = 0x29<<1;
uint16_t Clear_value, Red_value, Green_value, Blue_value;
uint32_t color, Clear_threshold, Red_threshold, Green_threshold, Blue_threshold;

/*##########################################################################################################*/
void Test_cts34725(void)
{
	unsigned char buffer[1] = {0x92};
	HAL_I2C_Master_Transmit(&hi2c1, Color_Sensor_Address, buffer, 1, 100);

	unsigned char data[1] = {0};
	HAL_I2C_Master_Receive(&hi2c1, Color_Sensor_Address, data, 1, 100);
	
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
	unsigned char Timing_register[2] = {0x81, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, Color_Sensor_Address, Timing_register, 2, 100);
	
	//2) Write the Control Register (address code: 143 = 0x8F) to 0 -> sets the gain to 1.
	unsigned char Control_register[2] = {0x8F, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, Color_Sensor_Address, Control_register, 2, 100);
	
	//3) Write the Enable Register (address code: 128 = 0x80) to 3 -> enables the ADC and turns the device on.
	unsigned char Enable_register[2] = {0x80, 0x03};
	HAL_I2C_Master_Transmit(&hi2c1, Color_Sensor_Address, Enable_register, 2, 100);
}
/*##########################################################################################################*/
/*
	C. Read data from the color sensor
*/
void Read_cts34725(void)
{
	//1) Read 2 bytes of CLEAR Data (address code: 148 = 0x94)
	unsigned char Clear_register[1] = {0x94};
	HAL_I2C_Master_Transmit(&hi2c1, Color_Sensor_Address, Clear_register, 1, 100);
	unsigned char Clear_data[2] = {0,0};
	HAL_I2C_Master_Receive(&hi2c1, Color_Sensor_Address, Clear_data, 2, 100);
	//16-bits, but we got it into 2 bytes (8-bits)
	Clear_value = (((int) Clear_data[1]) << 8) | Clear_data[0];
	
	//2) Read 2 bytes of RED Data (address code: 150 = 0x96)
	unsigned char Red_register[1] = {0x96};
	HAL_I2C_Master_Transmit(&hi2c1, Color_Sensor_Address, Red_register, 1, 100);
	unsigned char Red_data[2] = {0,0};
	HAL_I2C_Master_Receive(&hi2c1, Color_Sensor_Address, Red_data, 2, 100);
	//16-bits, but we got it into 2 bytes (8-bits)
	Red_value = (((int) Red_data[1]) << 8) | Red_data[0];
	
	//3) Read 2 bytes of GREEN Data (address code: 152 = 0x98)
	unsigned char Green_register[1] = {0x98};
	HAL_I2C_Master_Transmit(&hi2c1, Color_Sensor_Address, Green_register, 1, 100);
	unsigned char Green_data[2] = {0,0};
	HAL_I2C_Master_Receive(&hi2c1, Color_Sensor_Address, Green_data, 2, 100);
	//16-bits, but we got it into 2 bytes (8-bits)
	Green_value = (((int) Green_data[1]) << 8) | Green_data[0];
	
	//4) Read 2 bytes of BLUE Data (address code: 154 = 0x9A)
	unsigned char Blue_register[1] = {0x9A};
	HAL_I2C_Master_Transmit(&hi2c1, Color_Sensor_Address, Blue_register, 1, 100);
	unsigned char Blue_data[2] = {0,0};
	HAL_I2C_Master_Receive(&hi2c1, Color_Sensor_Address, Blue_data, 2, 100);
	//16-bits, but we got it into 2 bytes (8-bits)
	Blue_value = (((int) Blue_data[1]) << 8) | Blue_data[0];
	
	//5) Claculate theshold of color in sensor.
	color = Red_value*65536 + Green_value*256 + Blue_value;
}
/*##########################################################################################################*/
/*
	It save the threshold of each color for adapt to the environment of light.
	And it show on the console.
*/
void Store_Colors(void)
{
	if(count == 0){PositionServo(33);}
	Read_cts34725();
	char uartComAT[100];
	
if(count == 0)
	{
		Clear_threshold = color;
		HAL_GPIO_WritePin(RedLed_GPIO_Port, RedLed_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
		count++;
		sprintf(uartComAT, "Init process for store value of colors.\nWaiting color Red:\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
	}
	
if(count == 1)
	{
		Red_threshold = color;
		sprintf(uartComAT, "Waiting color Green:\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
		HAL_GPIO_WritePin(RedLed_GPIO_Port, RedLed_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, GPIO_PIN_SET);
		count++;
	}
	
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
	
	HAL_Delay(2000);
	
	sprintf(uartComAT, "\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)uartComAT, strlen(uartComAT), 100);	
}
/*####################### FUNCTIONS FOR SERVOS #############################################################*/
/*
Steps for config timer:
	- Select timer and chanel, in this case TIM3 Channel 1.
	- Select in channel PWM Generation CH1.
	- Need pulses for 20 ms frecuncy 50 Hz, to do this config prescaler (PSC - 15 bits value) and ARR.
	  In this case in clock configuration i have in APB1 Timer clocks 72 MHz.
	  72 MHz / 50 Hz = 1 440 000 -> Preescaler = 1440-1 and ARR = 1000.
	
	Init declaring function that start PWM, in this case with timer 3 (chanel 1):
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
*/

void PositionServo(uint8_t angle)	// 33, 67
{
	htim3.Instance->CCR1 = angle;
}

void ContinuousServo(uint8_t init, uint8_t finish)
{
	for(uint8_t i=init; i<finish; i++)
	{
		htim3.Instance->CCR1 = i;
		HAL_Delay(100);
	}
}
/*##########################################################################################################*/

