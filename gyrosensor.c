#include "gyrosensor.h"

/**/

uint8_t data_rec[6];				// Raw data fetched from the sensor. Each axis occupies two bytes.
uint8_t reg;						// Register address to read and write data. Used in adxl_write and adxl_read functions
uint8_t numberofbytes;				// A parameter in adxl_read function. Indicates how many bytes to read from specified address.
uint8_t value;						// Value to be written to specified register in adxl_write function.
int16_t x,y,z;						// Splitted data in terms of axises of data_rec.
float xg,yg,zg;						// Converted data.
uint8_t buf3[30];					// Buffer to send out converted data.
/**/

void adxl_write(uint8_t reg, uint8_t value){


	uint8_t data[2];
	data[0]=reg;
	data[1]=value;
	HAL_I2C_Master_Transmit(&hi2c1, ADXL_ADDR, data, 2, 10);
}

void adxl_read(uint8_t reg, uint8_t numberofbytes){

	HAL_I2C_Mem_Read(&hi2c1, ADXL_ADDR, reg, 1, data_rec, numberofbytes, 100);
}

void adxl_init(void){

	// To test if the sensor is working. Value of data_rec[0] should be 0xE5 after running the line below.
	adxl_read(0x00,1);
	// Reset all bits.
	adxl_write(0x2d,0);
	// POWER_CTL.Running the line below should put the sensor in measurement mode.
	adxl_write(0x2d,0x08);
	// Range selection: 0x00:+-2g  0x01:+-4g  0x10:+-8g  0x11:+-16g
	adxl_write(0x31,0x01);

}

uint8_t read_data(uint16_t buffer){

	// Read Measurement Data
	adxl_read(0x32,6);
	x= (data_rec[1]<<8) | data_rec[0];
	y= (data_rec[3]<<8) | data_rec[2];
	z= (data_rec[5]<<8) | data_rec[4];

	// Convert The Measurement Data
	xg=x*0.0078*1000;
	yg=y*0.0078*1000;
	zg=z*0.0078*1000;

	// Write Data Into Buffer
	if(xg<0 && yg <0 && zg<0){
		xg=-xg;
		yg=-yg;
		zg=-zg;
		sprintf((char*)buf3,"\nX:-%u.%u Y:-%u.%u Z:-%u.%u\r\n",((unsigned int)xg/1000),((unsigned int)xg % 1000),((unsigned int)yg / 1000),((unsigned int)yg % 1000),((unsigned int)zg / 1000),((unsigned int)zg % 1000));
	}
	else if(xg<0 && yg<0 && zg >=0){
		xg=-xg;
		yg=-yg;
		sprintf((char*)buf3,"\nX:-%u.%u Y:-%u.%u Z:%u.%u\r\n",((unsigned int)xg/1000),((unsigned int)xg % 1000),((unsigned int)yg / 1000),((unsigned int)yg % 1000),((unsigned int)zg / 1000),((unsigned int)zg % 1000));
	}
	else if(xg<0 && yg>=0 && zg <0){
		xg=-xg;
		zg=-zg;
			sprintf((char*)buf3,"\nX:-%u.%u Y:%u.%u Z:-%u.%u\r\n",((unsigned int)xg/1000),((unsigned int)xg % 1000),((unsigned int)yg / 1000),((unsigned int)yg % 1000),((unsigned int)zg / 1000),((unsigned int)zg % 1000));
		}
	else if(xg<0 && yg>=0 && zg >=0){
		xg=-xg;
			sprintf((char*)buf3,"\nX:-%u.%u Y:%u.%u Z:%u.%u\r\n",((unsigned int)xg/1000),((unsigned int)xg % 1000),((unsigned int)yg / 1000),((unsigned int)yg % 1000),((unsigned int)zg / 1000),((unsigned int)zg % 1000));
		}
	else if(xg>=0 && yg<0 && zg <0){
		yg=-yg;
		zg=-zg;
			sprintf((char*)buf3,"\nX:%u.%u Y:-%u.%u Z:-%u.%u\r\n",((unsigned int)xg/1000),((unsigned int)xg % 1000),((unsigned int)yg / 1000),((unsigned int)yg % 1000),((unsigned int)zg / 1000),((unsigned int)zg % 1000));
		}
	else if(xg>=0 && yg<0 && zg >=0){
		yg=-yg;
			sprintf((char*)buf3,"\nX:%u.%u Y:-%u.%u Z:%u.%u\r\n",((unsigned int)xg/1000),((unsigned int)xg % 1000),((unsigned int)yg / 1000),((unsigned int)yg % 1000),((unsigned int)zg / 1000),((unsigned int)zg % 1000));
		}
	else if(xg>=0 && yg>=0 && zg <0){
		zg=-zg;
			sprintf((char*)buf3,"\nX:%u.%u Y:%u.%u Z:-%u.%u\r\n",((unsigned int)xg/1000),((unsigned int)xg % 1000),((unsigned int)yg / 1000),((unsigned int)yg % 1000),((unsigned int)zg / 1000),((unsigned int)zg % 1000));
		}
	else if(xg>=0 && yg>=0 && zg >=0){
			sprintf((char*)buf3,"\nX:%u.%u Y:%u.%u Z:%u.%u\r\n",((unsigned int)xg/1000),((unsigned int)xg % 1000),((unsigned int)yg / 1000),((unsigned int)yg % 1000),((unsigned int)zg / 1000),((unsigned int)zg % 1000));
		}

	return buf3;
}

void read_data_send_out(uint16_t buf3[24]){

	// Send Out The Buffer to UART
	HAL_UART_Transmit(&huart2, buf3, strlen((char*)buf3), HAL_MAX_DELAY);   						// Send Out Buffer (Temperature, Humidity or Error)
	osDelay(1100);

}
