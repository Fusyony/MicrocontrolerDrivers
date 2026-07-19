
#ifndef _BMP280_DRIVER
#define _BMP280_DRIVER

#include "main.h"
#include "definitions.h"
#include <stdio.h>


// ---- LIB Structures ---- //

typedef struct  {
	I2C_HandleTypeDef *hdl;
} BMP280_t;

#define BMP_ADDR (0x76 << 1)

// ---- COMMANDS -----  //

#define BMP280_ADDR_ID		0xD0
#define BMP280_ADDR_RESET	0xE0
#define BMP280_ADDR_STATUS	0xF3
#define BMP280_ADDR_CTRL	0xF4

// CTRL

#define BMP280_SLEEP_MODE	0x00
#define BMP280_FORCED_MODE	0x01
#define BMP280_NORMAL_MODE	0x03


// Temperature

#define BMP280_ADDR_TEMP		0xFA
#define BMP280_ADDR_TEMP_SIZE	0x14

// ---- LIB Functions ---- //

void BMP_Init(BMP280_t handle);
void BMP_Write(BMP280_t handle, uint8_t *data, uint16_t size);
void BMP_Read(BMP280_t handle, uint8_t *buffer, uint16_t size);
void BMP_Register_Write(BMP280_t handle, uint8_t memaddr, uint16_t memsize, uint8_t *data, uint8_t size);
void BMP_Register_Read(BMP280_t handle, uint8_t memaddr, uint16_t memsize, uint8_t *buffer, uint8_t size);
void BMP_Get_Temp(BMP280_t handle);


void BMP_Convert_Temp(BMP280_t handle, uint32_t raw_temp);


#endif
