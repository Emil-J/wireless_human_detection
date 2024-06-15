#ifndef LCD1602_I2C_H_
#define LCD1602_I2C_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Command */
#define CLEARDISPLAY 0x01
#define RETURNHOME 0x02
#define ENTRYMODESET 0x04
#define DISPLAYCONTROL 0x08
#define FUNCTIONSET 0x20
#define SETDDRAMADDR 0x80

/* Entry Mode */
#define ENTRYLEFT 0x02
#define ENTRYSHIFTDECREMENT 0x00

/* Display On/Off */
#define DISPLAYON 0x04
#define DISPLAYOFF 0x00
#define CURSORON 0x02
#define CURSOROFF 0x00
#define BLINKOFF 0x00

/* Function Set */
#define FOURBITMODE 0x00
#define TWOLINE 0x08
#define FIVE_EIGHTDOTS 0x00

/* Backlight */
#define BACKLIGHT 0x08
#define NOBACKLIGHT 0x00

/* Enable Bit */
#define ENABLEBIT 0x04

/* Register Select Bit */
#define RS 0x01

/* Device I2C Address */
#define DEV_ADDR     (0x27 << 1)

void LCD1602_Init();
void LCD1602_Clear();
void LCD1602_Home();
void LCD1602_SetCursor(uint8_t col, uint8_t row);
void LCD1602_Backlight();
void LCD1602_NoBacklight();
void LCD1602_NoDisplay();
void LCD1602_Display();
void LCD1602_Cursor();
void LCD1602_PrintStr(const char[]);

#endif /* LCD1602_I2C_H_ */
