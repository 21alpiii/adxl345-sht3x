#include "sensors.h"

/**/
int val=0;						// Raw data fetched from the sensor
float temp_c=0;					// Processed temperature data to be sent out
float hum=0;					// Processed humidity data to be sent out
uint16_t MAX_16BIT_INT=0xFFFF;	// Received data is 16 bits
HAL_StatusTypeDef ret;  		// To check if Transmitting or Receiving is done successfully
uint8_t buf[12];				// Buffer to send out data
uint8_t buf2[12];
/**/

void SensorInit(void){

	//Soft Reset
	ret = HAL_I2C_Mem_Write(&hi2c1, SHT3X_ADDR, SOFT_RESET ,2, buf2, 1, HAL_MAX_DELAY);
	HAL_Delay(100);
}


uint8_t MeasureTemperatureTransmit(void){

	buf[0] = REG_TEMP;
	// Command To Measure Temperature
	ret = HAL_I2C_Mem_Write(&hi2c1, SHT3X_ADDR, MEASURE_TEMP ,2, buf, 1, HAL_MAX_DELAY);
	HAL_Delay(100);
	if ( ret != HAL_OK ) {
		//Error in Transmitting
		strcpy((char*)buf, "Error Tx\r\n");
	}
	return buf;
}


uint8_t MeasureTemperatureReceive(void){

	//Receive Temperature Data
	ret = HAL_I2C_Master_Receive(&hi2c1, SHT3X_ADDR, buf, 2, HAL_MAX_DELAY);
	osDelay(150);
	if ( ret != HAL_OK ) {

		//Error in Receiving
		strcpy((char*)buf, "Error Rx\r\n");
	}
	return buf;
}


uint8_t* CalculateTemperature(uint8_t buf[12]){

	//Combine The Bytes
	val = ((int16_t)buf[0] << 8) | (buf[1] >> 8);
	// Convert To Float Temperature Value (Celsius)
	temp_c = (-45+175*(float)val/(MAX_16BIT_INT-1));
	// Convert Temperature to Real World Format and Write Into Buffer
	temp_c *= 100;
	sprintf((char*)buf,"%u.%u C\r\n",((unsigned int)temp_c/100),((unsigned int)temp_c % 100));
	return buf;
}


void SendOut(uint8_t buf[12]){

	// Send Out Buffer (Temperature, Humidity or Error)
	HAL_UART_Transmit(&huart2, buf, strlen((char*)buf), HAL_MAX_DELAY);
	osDelay(150);
}


uint8_t MeasureHumidityTransmit(void){

	buf2[0] = REG_TEMP;
	//Command To Measure Humidity
	ret = HAL_I2C_Mem_Write(&hi2c1, SHT3X_ADDR, MEASURE_HUM ,2, buf2, 1, HAL_MAX_DELAY);
	osDelay(100);

	if ( ret != HAL_OK ) {
	  //Error in Transmitting
	  strcpy((char*)buf2, "Error Tx\r\n");
	  return 0;
	}
	return buf2;
	}


uint8_t MeasureHumidityReceive(void){

	//Receive Temperature Data
	ret = HAL_I2C_Master_Receive(&hi2c1, SHT3X_ADDR, buf2, 2, HAL_MAX_DELAY);
	osDelay(150);

	if ( ret != HAL_OK ) {

		//Error in Receiving
		strcpy((char*)buf2, "Error Rx\r\n");
	}
	return buf2;
	}


uint8_t CalculateHumidity(uint8_t buf2[12]){

	// Combine The Bytes
	val = ((int16_t)buf2[0] << 8) | (buf2[1] >> 8);
	// Convert Temperature to Real World Format and Write Into Buffer
	hum = 100*(float)val/(MAX_16BIT_INT-1);
	hum *= 100;
	sprintf((char*)buf2,"%u.%u H\r\n",((unsigned int)hum/100),((unsigned int)hum % 100));		// Convert Humidity to a User Friendly Format
	return buf2;
}
