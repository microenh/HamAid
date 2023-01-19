/*****************************************************************************
* | File      	:	LCD_Touch.h
* | Author      :   Waveshare team
* | Function    :	LCD Touch Pad Driver and Draw
* | Info        :
*   Image scanning
*      Please use progressive scanning to generate images or fonts
*----------------
* |	This version:   V1.0
* | Date        :   2017-08-16
* | Info        :   Basic version
*
******************************************************************************/
#pragma once

#include "DEV_Config.h"
#include <math.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/float.h"


#define TP_PRESS_DOWN 0x80
#define TP_PRESSED    0x40

//Touch screen structure
typedef struct {
	uint16_t Xpoint0;
	uint16_t Ypoint0;
	uint16_t Xpoint;
	uint16_t Ypoint;
	uint8_t chStatus;
	uint8_t chType;
	int16_t iXoff;
	int16_t iYoff;
	float fXfac;
	float fYfac;
	//Select the coordinates of the XPT2046 touch \
	  screen relative to what scan direction
} TP_DEV;

//Brush structure
typedef struct{
	uint16_t Xpoint;
	uint16_t Ypoint;
	uint16_t Color;
} TP_DRAW;

typedef struct {
  int16_t x;
  int16_t y;
} TP_XY;


bool TP_Scan(TP_XY *calibrated);
bool raw_touch(TP_XY *touch);
void calibrate(TP_XY raw[4]);
void TP_Init(void);
