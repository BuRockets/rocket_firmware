#include "MPU9250.h"

#include "string.h"
#include <stdio.h>

#include "MPU9250_reg.h"
#include "i2c_er.h"
#include "main.h"


void MPU_init(void) {

	uint8_t R;
	uint8_t c;

	HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDRESS_W, PWR_MGMT_1, 1, 0x00, 1, 100); // Clear sleep mode bit (6), enable all sensors

	HAL_Delay(100);
	R = 0x01;
	HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDRESS_W, PWR_MGMT_1, 1, &R, 1, 100); // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001


	// Configure Gyro and Accelerometer
	// Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively;
	// DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both
	// Maximum delay is 4.9 ms which is just over a 200 Hz maximum rate
	R = 0x00;
	HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDRESS_W, CONFIG, 1, &R, 1, 100);

	// Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
	R = 0x01;
	HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDRESS_W, SMPLRT_DIV, 1, &R, 1, 100); // Use a 200 Hz rate; the same rate set in CONFIG above

	// Set gyroscope full scale range
	// Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
	HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS_R, GYRO_CONFIG, 1, &c, 6, 100); // get current GYRO_CONFIG register value
	c = c & ~0xE0; // Clear self-test bits [7:5]
	c = c & ~0x02; // Clear Fchoice bits [1:0]
	c = c & ~0x18; // Clear AFS bits [4:3]
	c = c | 0 << 3; // Set full scale range for the gyro - 0=+250dps
	c = c | 0x00; // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG
	HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDRESS_W, GYRO_CONFIG, 1, &c, 1, 100); // Write new GYRO_CONFIG value to register

   // Set accelerometer full-scale range configuration
	HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS_R, ACCEL_CONFIG, 1, &c, 6, 100); // get current ACCEL_CONFIG register value
	c = c & ~0xE0; // Clear self-test bits [7:5]
	c = c & ~0x18; // Clear AFS bits [4:3]
	c = c | 0 << 3; // Set full scale range for the accelerometer  - 0=2g
	HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDRESS_W, ACCEL_CONFIG, 1, &c, 1, 100); // Write new ACCEL_CONFIG register value

	 // Set accelerometer sample rate configuration
	 // It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for
	 // accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz
	HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS_R, MPU9250_ACCEL_CONFIG_2, 1, &c, 6, 100); // get current ACCEL_CONFIG2 register value
	c = c & ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
	c = c | 0x03; // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
	HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDRESS_W, MPU9250_ACCEL_CONFIG_2, 1, &c, 1, 100); // Write new ACCEL_CONFIG2 register value

	// The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
	// but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting

	// Configure Interrupts and Bypass Enable
   // Set interrupt pin active high, push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so additional chips
   // can join the I2C bus and all can be controlled by the Arduino as master
	R = 0x22;
	HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDRESS_W, MPU9250_INT_PIN_CFG, 1, &R, 1, 100);
	R = 0x01;
	HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDRESS_W, MPU9250_INT_ENABLE, 1, &R, 1, 100); // Enable data ready (bit 0) interrupt
}

void MAGN_init(float * destination) {

	uint8_t R;
	uint8_t rawData[3]; // x/y/z gyro calibration data stored here

	// First extract the factory calibration for each magnetometer axis
	R = 0x00;
	HAL_I2C_Mem_Write(&hi2c1, AK8963_ADDRESS_W, AK8963_CNTL, 1, &R, 1, 100); // Power down magnetometer
	HAL_Delay(10);
	R = 0x0F;
	HAL_I2C_Mem_Write(&hi2c1, AK8963_ADDRESS_W, AK8963_CNTL, 1, &R, 1, 100); // Enter Fuse ROM access mode
	HAL_Delay(10);
	HAL_I2C_Mem_Read(&hi2c1, AK8963_ADDRESS_W, AK8963_ASAX, 1, rawData, 3, 100); // Read the x-, y-, and z-axis calibration values
	destination[0] =  (float)(rawData[0] - 128) / 256.0f + 1.0f; // Return x-axis sensitivity adjustment values, etc.
	destination[1] =  (float)(rawData[1] - 128) / 256.0f + 1.0f;
	destination[2] =  (float)(rawData[2] - 128) / 256.0f + 1.0f;
	R = 0x00;
	HAL_I2C_Mem_Write(&hi2c1, AK8963_ADDRESS_W, AK8963_CNTL, 1, &R, 1, 100); // Power down magnetometer
	HAL_Delay(10);
	// Configure the magnetometer for continuous read and highest resolution
	// set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
	// and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
	R = 0 << 4 | 0x06;
	HAL_I2C_Mem_Write(&hi2c1, AK8963_ADDRESS_W, AK8963_CNTL, 1, &R, 1, 100); // Set magnetometer data resolution and sample ODR
	HAL_Delay(10);

	R = 0x40;
	HAL_I2C_Mem_Write(&hi2c1, AK8963_ADDRESS_W, AK8963_ASTC, 1, &R, 1, 100); // set self-test
}

void MPU_get_magn(int16_t * destination) {
	uint8_t rawData[7]; // x/y/z gyro register data, ST2 register stored here, must read ST2 at end of data acquisition
	uint8_t c;
	uint32_t status = HAL_I2C_Mem_Read(&hi2c1, AK8963_ADDRESS_R, AK8963_ST1, 1, &c, 1, 10);
	if (status != HAL_OK)
	{
		I2C_ClearBusyFlagErratum(&hi2c1, 1000);
	}
	if (c >= 0x01) {
		// wait for magnetometer data ready bit to be set
		status = HAL_I2C_Mem_Read(&hi2c1, AK8963_ADDRESS_R, AK8963_XOUT_L, 1, rawData, 7, 10); // Read the six raw data and ST2 registers sequentially into data array
		if (status != HAL_OK)
		{
			I2C_ClearBusyFlagErratum(&hi2c1, 1000);
		}
		c = rawData[6]; // End data read by reading ST2 register
		if (!(c & 0x08)) {
			// Check if magnetic sensor overflow set, if not then report data
			destination[1] = (int16_t)(((int16_t)rawData[1] << 8) | rawData[0]); // Turn the MSB and LSB into a signed 16-bit value
			destination[0] = (int16_t)(((int16_t)rawData[3] << 8) | rawData[2]); // Data stored as little Endian
			destination[2] = -(int16_t)(((int16_t)rawData[5] << 8) | rawData[4]);
		}
	}

}

void MPU_get_accel(float* destination) {//��� ��� �������� ������ ���������

	uint8_t rawData[6];
	uint32_t status= HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS_R, MPU9250_ACCEL_XOUT_H, 1, rawData, 6, 100);
	if (status != HAL_OK)
	{
		I2C_ClearBusyFlagErratum(&hi2c1, 1000);
	}
	destination[0] = ((float)(int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) / 16384 * (9.8)); // Turn the MSB and LSB into a signed 16-bit value
	destination[1] = ((float)(int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) / 16384 * (9.8));
	destination[2] = ((float)(int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) / 16384 * (9.8));

}

void MPU_get_gyro(float * destination) {

	uint8_t rawData[6]; // x/y/z gyro register data stored here
	uint32_t status = HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDRESS_R, MPU9250_GYRO_XOUT_H, 1, rawData, 6, 100); // Read the six raw data registers sequentially into data array
	if (status != HAL_OK)
	{
		I2C_ClearBusyFlagErratum(&hi2c1, 1000);
	}
	destination[0] = (float)(int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) / 131; // Turn the MSB and LSB into a signed 16-bit value
	destination[1] = (float)(int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) / 131;
	destination[2] = (float)(int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) / 131;
}
