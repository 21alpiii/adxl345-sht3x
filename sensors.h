#include <string.h>
#include <stdio.h>
#include <main.h>

/**/
//SHT3x Sensor
#define SHT3X_ADDR  0x44 << 1 												    // Use 8-bit address
#define REG_TEMP  0x00
#define SOFT_RESET  0x30A2
#define MEASURE_TEMP  0x2C06
#define MEASURE_HUM  0x2C10
#define FETCH_DATA 0xE000
#define ENABLE_ART 0x2B32
#define BREAK 0x3093
#define HEATER_ENABLE 0x306D
#define HEATER_DISABLE 0x3066
/**/
extern int count;
extern uint8_t buf[12];
extern uint8_t buf2[12];
extern HAL_StatusTypeDef ret;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

/**/
void SensorInit(void);															// A simple soft reset to run once, outside the loop.
uint8_t MeasureTemperatureTransmit(void);										// Transmits the command to measure data.
uint8_t MeasureTemperatureReceive(void);										// Receives data to buffer.
uint8_t* CalculateTemperature(uint8_t buf[12]);									// Converts the data in buffer to a real world format.
void SendOut(uint8_t buf[12]);													// Send the data in buffer out via UART.
uint8_t MeasureHumidityTransmit(void);											// Transmits the command to measure data.
uint8_t MeasureHumidityReceive(void);											// Receives data to buffer.
uint8_t CalculateHumidity(uint8_t buf2[12]);									// Converts the data in buffer to a real world format.
/**/




