#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hardware/pwm.h"
#include "fonts.h"

#define LCD_HEIGHT 320
#define LCD_WIDTH  240

#define LCD_RST_PIN  15
#define LCD_DC_PIN    8
#define LCD_BL_PIN   13
    
#define LCD_CS_PIN    9
#define LCD_CLK_PIN  10
#define LCD_MOSI_PIN 11
#define LCD_MISO_PIN 12
    
#define SPI_PORT spi1
#define SPI_SPEED 4000000

// Note: colors are designed for the LCD to be inverted (Invert(true))
// #define RGB_565(r,g,b) (((((r)) & 0xf8) << 8) | ((((g)) & 0xfc) << 3) | (((b)) >> 3))

// Byte swapped 
// #define RGB_565(r,g,b) ((g & 0x1c) << 11) | ((b & 0xf8) << 5) | (r & 0xf8) | ((g & 0xe0) >> 5)

// Byte swapped (Invert(false))
#define RGB_565(r,g,b) (((g & 0x1c) << 11) | ((b & 0xf8) << 5) | (r & 0xf8) | ((g & 0xe0) >> 5)) ^ 0xffff

#define BLACK    RGB_565(0,0,0)
#define RED      RGB_565(127,0,0)    
#define BRED     RGB_565(255,0,0)    
#define GREEN    RGB_565(0,127,0)    
#define BGREEN   RGB_565(0,255,0)    
#define BLUE     RGB_565(0,0,127)    
#define BBLUE    RGB_565(0,0,255)    
#define CYAN     RGB_565(0,127,127)  
#define BCYAN    RGB_565(0,255,255)  
#define MAGENTA  RGB_565(127,0,127)  
#define BMAGENTA RGB_565(255,0,255)  
#define YELLOW   RGB_565(127,127,0)
#define BYELLOW  RGB_565(255,255,0)  
#define GRAY     RGB_565(127,127,127)
#define WHITE    RGB_565(255,255,255)
#define ORANGE   RGB_565(255,127,0)  
#define PINK     RGB_565(255,0,127)  
#define LGREEN   RGB_565(0,255,127)
#define LBLUE    RGB_565(0,127,255)

#define HORIZONTAL 0
#define VERTICAL   1

typedef struct{
    uint16_t width;
    uint16_t height;
    uint8_t  scanDir;
} LCD_Attributes;

extern LCD_Attributes lcd;
extern uint8_t slice_num;

// ------------------------------------------------------------------------------------------------------
// void InitHardware(void);
void InitPWM(void);
void InitLCD(uint8_t scan_dir);
void DrawChar(const uint16_t x, const uint16_t y, const sFONT *font,
    const uint16_t foreground, const uint16_t background, const uint8_t character);
void DrawString(uint16_t x, uint16_t y, const char * pString, const sFONT* font, uint16_t foreground, uint16_t background);
void DrawSeconds(uint16_t x, uint16_t y, uint16_t seconds, const sFONT *font,
                 uint16_t foreground, uint16_t background, uint16_t prev_seconds);
void Invert(bool invert);
void ClearWindow(uint16_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void DisplayOff(void);
void DisplayOn(uint8_t bl);
void SetWindow(uint16_t x, uint16_t y, uint16_t xEnd, uint16_t yEnd);
#define Clear(Color) ClearWindow(Color, 0, 0, lcd.width, lcd.height)
#define BacklightLevel(Value) pwm_set_chan_level(slice_num, PWM_CHAN_B, Value)

