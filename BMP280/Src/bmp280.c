#include "bmp280.h"

void BMP_Init(BMP280_t handle)
{
	uint8_t buffer_id = 0;
	uint8_t buffer_status = 0;
	uint8_t measuring = 0;
	uint8_t update = 0;
	uint8_t cmd = 0xB6;

	// --- Optimal d'après la doc ---
	// osr_t x 1 0x01
	// osr_p x 4 0x03
	uint8_t mode = (uint8_t) 1 << 6 | (uint8_t) 3 << 2 | BMP280_NORMAL_MODE;

	BMP_Register_Write(handle, BMP280_ADDR_RESET, sizeof(uint8_t), &cmd, 1);
	BMP_Register_Write(handle, BMP280_ADDR_CTRL, sizeof(uint8_t), &mode, 1);

	BMP_Register_Read(handle, BMP280_ADDR_ID, sizeof(uint8_t), &buffer_id, 1);
	BMP_Register_Read(handle, BMP280_ADDR_STATUS, sizeof(uint8_t), &buffer_status, 1);

	measuring = (buffer_status >> 3) & 0x01;
	update = buffer_status & 0x01;

	printf("[BMP][INIT]: ID %d | measuring %d | update %d \n\r", buffer_id, measuring, update);
}

void BMP_Write(BMP280_t handle, uint8_t *data, uint16_t size)
{
	HAL_I2C_Master_Transmit(handle.hdl, BMP_ADDR, data, size, HAL_MAX_DELAY);
}

void BMP_Read(BMP280_t handle, uint8_t *buffer, uint16_t size)
{
	HAL_I2C_Master_Receive(handle.hdl, BMP_ADDR, buffer, size, HAL_MAX_DELAY);
}

void BMP_Register_Write(BMP280_t handle, uint8_t memaddr, uint16_t memsize, uint8_t *data, uint8_t size)
{
	HAL_I2C_Mem_Write(handle.hdl, BMP_ADDR, memaddr, memsize, data, size, HAL_MAX_DELAY);
}

void BMP_Register_Read(BMP280_t handle, uint8_t memaddr, uint16_t memsize, uint8_t *buffer, uint8_t size)
{
	HAL_I2C_Mem_Read(handle.hdl, BMP_ADDR, memaddr, memsize, buffer, size, HAL_MAX_DELAY);
}

void BMP_Get_Temp(BMP280_t handle)
{
	uint32_t raw_temp = 0;
	uint8_t raw_temp_buffer[3] = {0, 0, 0};

	BMP_Register_Read(handle, BMP280_ADDR_TEMP, I2C_MEMADD_SIZE_8BIT, raw_temp_buffer, 3);

	raw_temp = (
			(uint32_t) raw_temp_buffer[0] << 12 |
			(uint32_t) raw_temp_buffer[1] << 4  |
			(uint32_t) raw_temp_buffer[2] >> 4
	);
	printf("[BMP][TEMP]: raw_temp: %d\n\r", raw_temp);
	BMP_Convert_Temp(handle, raw_temp);
}


void BMP_Convert_Temp(BMP280_t handle, uint32_t raw_temp)
{
	temp_message_t a;

	a.raw_temp = raw_temp;


	int32_t t_fine = 0;

	int32_t var1, var2, T;

	uint8_t buffer[] = {0, 0};

	uint16_t dig_T1;
	int16_t	dig_T2;
	int16_t dig_T3;

	// T1
	BMP_Register_Read(handle, 0x88, I2C_MEMADD_SIZE_8BIT, buffer, 2);
	dig_T1 = ((uint16_t) buffer[1] << 8 | (uint16_t) buffer[0]);

	// T2
	BMP_Register_Read(handle, 0x8A, I2C_MEMADD_SIZE_8BIT, buffer, 2);
	dig_T2 = ((int16_t) buffer[1] << 8 | (int16_t) buffer[0]);

	// T3
	BMP_Register_Read(handle, 0x8C, I2C_MEMADD_SIZE_8BIT, buffer, 2);
	dig_T3 = ((int16_t) buffer[1] << 8 | (int16_t) buffer[0]);

	// ---- Code is from the official BMP280 doc ---- //

	var1 = ((((raw_temp>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
	var2 = (((((raw_temp>>4) - ((int32_t)dig_T1)) * ((raw_temp>>4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;

	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;

	// ----- END OF THE CODE FROM THE DOC ---- //
	a.real_temp = T;

	osStatus_t status = osMessageQueuePut(displayQueue, &a, 0, 100);

	printf("PUT queue = %p\r\n", displayQueue);
	printf("LLLLLLLLLLLL\n\r");
	if (status != osOK) {
		printf("[BMP][ERROR] Message failed when putting into a queue\n\r");
	} else {
		printf("[BMP][OK] messaaaaaage send to the queue\n\r");
	}

	// printf("[BMP][TEMP]: %d,%d Celsius\n\r", (T / 100), T % 100);

}

