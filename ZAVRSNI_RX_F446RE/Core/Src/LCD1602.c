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

void LCD1602_Init()
{
  configFunctionSet = FOURBITMODE | TWOLINE | FIVE_EIGHTDOTS;
  displayBacklight = NOBACKLIGHT;

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
  sendCMD(FUNCTIONSET | configFunctionSet);

  displayControl = DISPLAYON | CURSOROFF | BLINKOFF;
  LCD1602_Display();
  LCD1602_Clear();

  /* Display Mode */
  displayMode = ENTRYLEFT | ENTRYSHIFTDECREMENT;
  sendCMD(ENTRYMODESET | displayMode);
  delay_us(4500);

  writeExpander(displayBacklight);
  HAL_Delay(1000);
  LCD1602_Home();
}

void LCD1602_Clear()
{
  sendCMD(CLEARDISPLAY);
  delay_us(2000);
}

void LCD1602_Home()
{
  sendCMD(RETURNHOME);
  delay_us(2000);
}

void LCD1602_SetCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  sendCMD(SETDDRAMADDR | (col + row_offsets[row]));
}

void LCD1602_NoDisplay()
{
  displayControl &= DISPLAYOFF;
  sendCMD(DISPLAYCONTROL | displayControl);
}

void LCD1602_Display()
{
  displayControl |= DISPLAYON;
  sendCMD(DISPLAYCONTROL | displayControl);
}

void LCD1602_Cursor()
{
  displayControl |= CURSORON;
  sendCMD(DISPLAYCONTROL | displayControl);
}

void LCD1602_Backlight()
{
	displayBacklight = BACKLIGHT;
	writeExpander(displayBacklight);
}

void LCD1602_NoBacklight()
{
	displayBacklight = NOBACKLIGHT;
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
  HAL_I2C_Master_Transmit(&hi2c2, DEV_ADDR, (uint8_t*)&data, 1, 10);
}

static void enPulse(uint8_t data)
{
  writeExpander(data | ENABLEBIT);
  delay_us(20);

  writeExpander(data & ~ENABLEBIT);
  delay_us(20);
}

