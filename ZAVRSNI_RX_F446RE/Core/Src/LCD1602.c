#include "LCD1602.h"

uint8_t configFunctionSet;
uint8_t displayBacklight;
uint8_t displayControl;
uint8_t displayMode;
uint8_t displayRows;

static void sendCMD(uint8_t);
static void sendChar(uint8_t);
static void send(uint8_t, uint8_t);
static void write4bits(uint8_t);
static void writeExpander(uint8_t);
static void enPulse(uint8_t);

uint8_t special1[8] = {
        0b00000,
        0b11001,
        0b11011,
        0b00110,
        0b01100,
        0b11011,
        0b10011,
        0b00000
};

uint8_t special2[8] = {
        0b11000,
        0b11000,
        0b00110,
        0b01001,
        0b01000,
        0b01001,
        0b00110,
        0b00000
};

void LCD1602_Init()
{
  configFunctionSet = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
  displayBacklight = LCD_NOBACKLIGHT;

  /* Wait for initialization */
  HAL_Delay(50);

  HAL_Delay(1000);

  /* 4bit Mode */
  write4bits(0x03 << 4);
  delay_us(4500);

  write4bits(0x03 << 4);
  delay_us(4500);

  write4bits(0x03 << 4);
  delay_us(4500);

  write4bits(0x02 << 4);
  delay_us(100);

  /* Display Control */
  sendCMD(LCD_FUNCTIONSET | configFunctionSet);

  displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  LCD1602_Display();
  LCD1602_Clear();

  /* Display Mode */
  displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  sendCMD(LCD_ENTRYMODESET | displayMode);
  delay_us(4500);

  LCD1602_CreateSpecialChar(0, special1);
  LCD1602_CreateSpecialChar(1, special2);
  writeExpander(displayBacklight);
  HAL_Delay(1000);
  LCD1602_Home();
}

void LCD1602_Clear()
{
  sendCMD(LCD_CLEARDISPLAY);
  delay_us(2000);
}

void LCD1602_Home()
{
  sendCMD(LCD_RETURNHOME);
  delay_us(2000);
}

void LCD1602_SetCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  sendCMD(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LCD1602_NoDisplay()
{
  displayControl &= LCD_DISPLAYOFF;
  sendCMD(LCD_DISPLAYCONTROL | displayControl);
}

void LCD1602_Display()
{
  displayControl |= LCD_DISPLAYON;
  sendCMD(LCD_DISPLAYCONTROL | displayControl);
}

void LCD1602_Cursor()
{
  displayControl |= LCD_CURSORON;
  sendCMD(LCD_DISPLAYCONTROL | displayControl);
}

void LCD1602_CreateSpecialChar(uint8_t location, uint8_t charmap[])
{
  location &= 0x7;
  sendCMD(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++)
  {
    sendChar(charmap[i]);
  }
}

void LCD1602_Backlight()
{
	displayBacklight = LCD_BACKLIGHT;
	writeExpander(displayBacklight);
}

void LCD1602_NoBacklight()
{
	displayBacklight = LCD_NOBACKLIGHT;
	writeExpander(displayBacklight);
}

void LCD1602_PrintStr(const char c[])
{
  while(*c && *c != '0') sendChar(*c++);
}

static void sendCMD(uint8_t cmd)
{
  send(cmd, 0);
}

static void sendChar(uint8_t ch)
{
  send(ch, RS);
}

static void send(uint8_t value, uint8_t mode)
{
  uint8_t highnib = value & 0xF0;
  uint8_t lownib = (value<<4) & 0xF0;
  write4bits((highnib)|mode);
  delay_us(45);
  write4bits((lownib)|mode);
  delay_us(45);
}

static void write4bits(uint8_t value)
{
  writeExpander(value);
  enPulse(value);
}

static void writeExpander(uint8_t data)
{
  data = data | displayBacklight;
  HAL_I2C_Master_Transmit(&hi2c2, DEVICE_ADDR, (uint8_t*)&data, 1, 10);
}

static void enPulse(uint8_t data)
{
  writeExpander(data | ENABLE);
  delay_us(20);

  writeExpander(data & ~ENABLE);
  delay_us(20);
}

