#include "ili9341.h"

const uint8_t my_bitmap[26][6] = {
    {0x0, 0x3E, 0x48, 0x48, 0x3E, 0x0}, // a
	{0x0, 0x7E, 0x52, 0x52, 0x2C, 0x0}, // b
    {0x0, 0x3C, 0x22, 0x22, 0x22, 0x0}, // c
    {0x0, 0x7E, 0x42, 0x42, 0x3C, 0x0}, // d
    {0x0, 0x7E, 0x52, 0x52, 0x42, 0x0}, // e
    {0x0, 0x7E, 0x48, 0x48, 0x40, 0x0}, // f
    {0x0, 0x3C, 0x42, 0x4A, 0x4E, 0x0}, // g
    {0x0, 0x7E, 0x10, 0x10, 0x7E, 0x0}, // h
    {0x0, 0x00, 0x5E, 0x00, 0x00, 0x0}, // i
    {0x0, 0x02, 0x02, 0x5C, 0x00, 0x0}, // j
    {0x0, 0x7E, 0x08, 0x14, 0x22, 0x0}, // k
    {0x0, 0x7E, 0x02, 0x02, 0x02, 0x0}, // l
    {0x0, 0x7E, 0x30, 0x30, 0x7E, 0x0}, // m
    {0x0, 0x3E, 0x40, 0x40, 0x7E, 0x0}, // n
    {0x0, 0x1C, 0x22, 0x22, 0x1C, 0x0}, // o
    {0x0, 0x7E, 0x48, 0x48, 0x30, 0x0}, // p
    {0x0, 0x7C, 0x44, 0x4C, 0x7E, 0x0}, // q
    {0x0, 0x7E, 0x48, 0x4C, 0x32, 0x0}, // r
    {0x0, 0x32, 0x52, 0x52, 0x4C, 0x0}, // s
    {0x0, 0x20, 0x3E, 0x20, 0x20, 0x0}, // t
    {0x0, 0x3C, 0x02, 0x02, 0x3C, 0x0}, // u
    {0x0, 0x3C, 0x02, 0x3C, 0x00, 0x0}, // v
    {0x0, 0x3C, 0x02, 0x3C, 0x02, 0x3C}, // w
    {0x0, 0x36, 0x08, 0x08, 0x36, 0x0}, // x
    {0x0, 0x20, 0x10, 0x0E, 0x70, 0x0}, // y
    {0x0, 0x62, 0x52, 0x4A, 0x7E, 0x46} // z
};

void ILI_Select(ILI9341_t hdl)
{
	HAL_GPIO_WritePin(hdl.cs.port, hdl.cs.pin, GPIO_PIN_RESET);
}

void ILI_Deselect(ILI9341_t hdl)
{
	HAL_GPIO_WritePin(hdl.cs.port, hdl.cs.pin, GPIO_PIN_SET);
}

void ILI_CMD_Mode(ILI9341_t hdl)
{
	HAL_GPIO_WritePin(hdl.dc.port, hdl.dc.pin, GPIO_PIN_RESET);
}

void ILI_DATA_Mode(ILI9341_t hdl)
{
	HAL_GPIO_WritePin(hdl.dc.port, hdl.dc.pin, GPIO_PIN_SET);
}

void ILI_Write(ILI9341_t hdl, uint8_t *data, size_t size)
{
	HAL_SPI_Transmit(hdl.hdl, data, size, HAL_MAX_DELAY);
}

void ILI_Delay(uint32_t ms)
{
	#ifdef osCMSIS
		if (osKernelGetState() == osKernelRunning ) {
			osDelay(ms);
		} else {
			HAL_Delay(ms);
		}
	#else
		HAL_Delay(ms);
	#endif
}

void ILI_Init(ILI9341_t hdl)
{
	uint8_t data;
	// Reset
	HAL_GPIO_WritePin(hdl.rst.port, hdl.rst.pin, GPIO_PIN_RESET);
	ILI_Delay(20);
	HAL_GPIO_WritePin(hdl.rst.port, hdl.rst.pin, GPIO_PIN_SET);
	ILI_Delay(120);

	ILI_Select(hdl);

	ILI_CMD_Mode(hdl);
	data = ILI9341_SOFTWARE_SLEEP_OUT;
	ILI_Write(hdl, &data , 1);
	ILI_Delay(120);

	ILI_CMD_Mode(hdl);
	data = ILI9341_PIXEL_FORMAT_SET;
	ILI_Write(hdl, &data , 1);

	ILI_CMD_Mode(hdl);
	data = ILI9341_DISPLAY_ON;
	ILI_Write(hdl, &data , 1);

	printf("[ILI][INIT]: Display init\n\r");
}

void ILI_SetWindowLocation(ILI9341_t hdl, uint16_t posx, uint16_t maxx, uint16_t posy, uint16_t maxy)
{
	// SET THE AREA OF WRITING
	uint8_t cmd;

	ILI_CMD_Mode(hdl);
	cmd = ILI9341_COLUMN_ADDRESS_SET;
	ILI_Write(hdl, &cmd, 1);

	ILI_DATA_Mode(hdl);
	uint8_t colData[] = {
		posx >> 8,
		posx & 0xFF,
		maxx >> 8,
		maxx & 0xFF
	};

	ILI_Write(hdl, colData, 4);

	ILI_CMD_Mode(hdl);
	cmd = ILI9341_PAGE_ADDRESS_SET;
	ILI_Write(hdl, &cmd, 1);

	ILI_DATA_Mode(hdl);
	uint8_t rowData[] = {
		posy >> 8,
		posy & 0xFF,
		maxy >> 8,
		maxy & 0xFF
	};
	ILI_Write(hdl, rowData, 4);
}

void ILI_WriteString_tmp(ILI9341_t hdl, char *string, int size, uint16_t posx, uint16_t posy)
{
	// --- TO IMPLEMENT PROPERLY ---
	// TODO
	/*
	 * Create a zone to draw based on the Posx - Posy | Ignoring the size by
	 *
	 */
	int len = 0;
	uint8_t cmd;
	uint8_t *line = NULL;

	for (; string[len]; len++);
	line = malloc(len * 6 * sizeof(uint8_t) * 3);

	// FILL THE BUFFER
	for (int i = 0; string[i]; i++) {
		for (int y = 0; y < 5; y++) {
			for (int j = 0; j < 8; j++) {
				if (my_bitmap[string[i] - 97][y] & (1 << j)) { // Si le bitmap est 1
					line[i * 3] = 0x00;
					line[i * 3 + 1] = 0xFF;
					line[i * 3 + 2] = 0x00;
				} else {  // sinon draw background
					line[i * 3] = 0xFF;
					line[i * 3 + 1] = 0x00;
					line[i * 3 + 2] = 0xFF;
				}
			}
		}
	}

	// SET THE AREA OF WRITING

	ILI_Select(hdl);
	ILI_SetWindowLocation(hdl, posx, posx + len, posy, posy + len);

	// WRITING TO THE SCREEN
	ILI_CMD_Mode(hdl);
	cmd = ILI9341_MEMORY_WRITE;
	ILI_Write(hdl, &cmd, 1);

	ILI_DATA_Mode(hdl);
	ILI_Write(hdl, line, len * 3);

	ILI_Deselect(hdl);
	// FREE SPACE

	free(line);
}

void ILI_WriteString(ILI9341_t hdl, char *string, int size, uint16_t posx, uint16_t posy)
{
    int len = 0;
    while (string[len]) len++;

    int char_width = 6;
    int char_height = 8;

    int width = len * char_width;
    int height = char_height;

    uint8_t *buffer = malloc(width * height * 3);
    if (!buffer) return;

    // couleurs (BRG)
    uint8_t fg[3] = {0x00, 0xFF, 0x00}; // vert
    uint8_t bg[3] = {0xFF, 0x00, 0xFF}; // magenta

    for (int i = 0; i < len; i++) {
        char c = string[i];

        if (c < 'a' || c > 'z') continue;

        for (int y = 0; y < 5; y++) {
            uint8_t line = my_bitmap[c - 'a'][y];

            for (int x = 0; x < 8; x++) {

                int pixel_x = i * char_width + x;
                int pixel_y = y;

                if (pixel_x >= width || pixel_y >= height) continue;

                int idx = (pixel_y * width + pixel_x) * 3;

                if (line & (1 << x)) {
                    buffer[idx]     = fg[0];
                    buffer[idx + 1] = fg[1];
                    buffer[idx + 2] = fg[2];
                } else {
                    buffer[idx]     = bg[0];
                    buffer[idx + 1] = bg[1];
                    buffer[idx + 2] = bg[2];
                }
            }
        }
    }

    // SET WINDOW
    ILI_SetWindowLocation(hdl, posx, posx + width - 1, posy, posy + height - 1);

    ILI_Select(hdl);

    uint8_t cmd = ILI9341_MEMORY_WRITE;
    ILI_CMD_Mode(hdl);
    ILI_Write(hdl, &cmd, 1);

    ILI_DATA_Mode(hdl);
    ILI_Write(hdl, buffer, width * height * 3);

    ILI_Deselect(hdl);

    free(buffer);
}

void ILI_DrawChar(ILI9341_t hdl, char character)
{
	uint8_t bitmap_a[6];

	for (int i = 0; i < 6; i++) {
		bitmap_a[i] = my_bitmap[character - 97][i];
	}


	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 8; j++) {
			if (bitmap_a[i] & (1 << j)) {
				ILI_DrawPixel(hdl, i, j);
			}
		}
	}
}

void ILI_DrawPixel(ILI9341_t hdl, uint16_t x, uint16_t y)
{
	uint8_t cmd;
	uint8_t green[3] = {0x0, 0xFF, 0x0};

	ILI_SetWindowLocation(hdl, x, x, y, y);
	ILI_Select(hdl);


	ILI_CMD_Mode(hdl);
	cmd = ILI9341_MEMORY_WRITE;
	ILI_Write(hdl, &cmd, 1);

	ILI_DATA_Mode(hdl);

	ILI_Write(hdl, green, 3);

	ILI_Deselect(hdl);
}


void ILI_DrawRed(ILI9341_t hdl)
{
	uint8_t data[3];
	uint8_t cmd;

	ILI_Select(hdl);
	ILI_CMD_Mode(hdl);
	cmd = ILI9341_COLUMN_ADDRESS_SET;
	ILI_Write(hdl, &cmd, 1);

	ILI_DATA_Mode(hdl);
	uint8_t colData[] = {0x00, 0x00, 0x00, 0xEF};
	ILI_Write(hdl, colData, 4);

	ILI_CMD_Mode(hdl);
	cmd = ILI9341_PAGE_ADDRESS_SET;
	ILI_Write(hdl, &cmd, 1);

	ILI_DATA_Mode(hdl);
	uint8_t rowData[] = {0x00, 0x00, 0x01, 0x3F};
	ILI_Write(hdl, rowData, 4);

	ILI_CMD_Mode(hdl);
	cmd = ILI9341_MEMORY_WRITE;
	ILI_Write(hdl, &cmd, 1);

	ILI_DATA_Mode(hdl);
	data[0] = 0xFF;
	data[1] = 0x0;
	data[2] = 0x0;

	uint8_t row[240 * 3];

	for (int i  = 0; i < 240 * 3; i += 3) {
		row[i] = 0x0;
		row[i + 1] = 0x0;
		row[i + 2] = 0xFF;
	}

	for (int i = 0; i < 320; i++)
	{
		ILI_Write(hdl, row, 240 * 3);
	}

	for (int i = 0; i < 240 * 320; i++)
	{
		ILI_Write(hdl, data, 3);
	}

	ILI_Deselect(hdl);

	temp_message_t a;

	printf("GET queue = %p\r\n", displayQueue);
	osStatus status = osMessageQueueGet(displayQueue, &a, NULL, osWaitForever);
	if (status != osOK) {
		printf("[ILI][ERROR]: get from queue failed\n\r");
	} else {
		printf("[ILI][OK]: from queue, temp is : %d,%d Celsius\n\r", a.real_temp / 100, a.real_temp % 100);
	}

}



