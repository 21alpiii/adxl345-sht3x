#include <string.h>
#include <stdio.h>
#include <main.h>

/**/
//ADXL345 SENSOR
#define ADXL_ADDR 				0x53<<1		// Use 8-bit address
#define ADXL_DEVID			0x00		// Device ID




extern uint8_t buf3[30];

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;



void adxl_write(uint8_t reg, uint8_t value); 											//Used to transmit data to sensor
void adxl_read(uint8_t reg, uint8_t numberofbytes); 									//Reads data from specified register
void adxl_init(void); 																	//Initialization of the sensor
uint8_t read_data(uint16_t buffer);														//Reads the measurement and writes it to buffer
void read_data_send_out(uint16_t buffer[30]);											//Sends the data to UART for transmitting
