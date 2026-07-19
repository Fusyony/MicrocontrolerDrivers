/****************************************
 *
 * MY ili9341 driver
 *
 */

#ifndef _ILI9341_DRIVER
#define _ILI9341_DRIVER

// SYSTEM Include

#include <stdlib.h>
#include <stdio.h>

// STM32 Include

#include "main.h"
#include "cmsis_os.h"
#include "definitions.h"

// ---- ILI9341 COMMANDS DEFINITION ---- //

#define ILI9341_SOFTWARE_RESET		0x1
#define ILI9341_SOFTWARE_SLEEP_OUT	0x11
#define ILI9341_DISPLAY_ON			0x29
#define ILI9341_MEMORY_WRITE		0x2C
#define ILI9341_PIXEL_FORMAT_SET	0x3A
#define ILI9341_COLUMN_ADDRESS_SET	0x2A
#define ILI9341_PAGE_ADDRESS_SET	0x2B

// ---- LIB Structures ---- //

typedef struct  {
	GPIO_TypeDef *port;
	uint16_t pin;
} ILI9341_Pin_t;

typedef struct  {
	SPI_HandleTypeDef *hdl;
	ILI9341_Pin_t dc;
	ILI9341_Pin_t rst;
	ILI9341_Pin_t cs;
} ILI9341_t;

// ---- LIB Functions ---- //

void ILI_Init( ILI9341_t hdl );
void ILI_Select( ILI9341_t hdl );
void ILI_Deselect(ILI9341_t hdl);
void ILI_CMD_Mode(ILI9341_t hdl);
void ILI_DATA_Mode(ILI9341_t hdl);
void ILI_Write(ILI9341_t hdl, uint8_t *data, size_t size);
void ILI_DrawRed(ILI9341_t hdl);
void ILI_Delay(uint32_t ms);

// ---- LIB in progress Functions ----- //

void ILI_DrawChar(ILI9341_t hdl, char character);
void ILI_DrawPixel(ILI9341_t hdl, uint16_t x, uint16_t y);
void ILI_WriteString(ILI9341_t hdl, char *string, int size, uint16_t posx, uint16_t posy);
void ILI_SetWindowLocation(ILI9341_t hdl, uint16_t posx, uint16_t maxx, uint16_t posy, uint16_t maxy);
extern const uint8_t my_bitmap[26][6];

#endif // _ILI9341_DRIVER






