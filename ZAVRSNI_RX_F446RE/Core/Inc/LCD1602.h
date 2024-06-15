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
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

/* Entry Mode */
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTDECREMENT 0x00

/* Display On/Off */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKOFF 0x00

/* Function Set */
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_5x8DOTS 0x00

/* Backlight */
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

/* Enable Bit */
#define ENABLE 0x04

/* Register Select Bit */
#define RS 0x01

/* Device I2C Address */
#define DEVICE_ADDR     (0x27 << 1)

void LCD1602_Init();
void LCD1602_Clear();
void LCD1602_Home();
void LCD1602_SetCursor(uint8_t col, uint8_t row);
void LCD1602_Backlight();
void LCD1602_NoBacklight();
void LCD1602_NoDisplay();
void LCD1602_Display();
void LCD1602_Cursor();
void LCD1602_CreateSpecialChar(uint8_t, uint8_t[]);
void LCD1602_PrintStr(const char[]);

#endif /* LCD1602_I2C_H_ */
