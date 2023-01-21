/*****************************************************************************
* | File      	:	LCD_Touch.c
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
#include "lcd_touch.h"
#include "lcd.h"
#include <stdlib.h>


int16_t highlight_grid = -1;
int16_t clear_grid = -1;
int16_t press_grid = -1;
int16_t hold_grid = -1;

// extern uint8_t id;
static TP_DEV sTP_DEV;
static TP_DRAW sTP_Draw;
/*******************************************************************************
function:
		Read the ADC of the channel
parameter:
	Channel_Cmd :	0x90: Read channel Y +, select the ADC resolution is 12 bits, set to differential mode
					0xd0: Read channel x +, select the ADC resolution is 12 bits, set to differential mode
*******************************************************************************/
static uint16_t TP_Read_ADC(uint8_t cmd)
{
    // A cycle of at least 400ns.
    uint8_t data, data1;

    gpio_put(TP_CS_PIN, 0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    // sleep_us(200);
    spi_read_blocking(SPI_PORT, 0, (uint8_t *) &data, 2);
    gpio_put(TP_CS_PIN, 1);
    return ((data << 5) | (data >> (16 - 5)));
}

/*******************************************************************************
function:
		Read the 5th channel value and exclude the maximum and minimum returns the average
parameter:
	Channel_Cmd :	0x90 :Read channel Y +
					0xd0 :Read channel x +
*******************************************************************************/
#define READ_TIMES  5	//Number of readings
#define LOST_NUM    1	//Discard value
static uint16_t TP_Read_ADC_Average(uint8_t Channel_Cmd)
{
  uint8_t i, j;
  uint16_t Read_Buff[READ_TIMES];
  uint16_t Read_Sum = 0, Read_Temp = 0;
  //LCD SPI speed = 3 MHz
  spi_set_baudrate(SPI_PORT,3000000);
  //Read and save multiple samples
  for(i = 0; i < READ_TIMES; i++){
		Read_Buff[i] = TP_Read_ADC(Channel_Cmd);
		//sleep_us(200);
	}
  //LCD SPI speed = 18 MHz
  spi_set_baudrate(SPI_PORT,18000000);
  //Sort from small to large
  for (i = 0; i < READ_TIMES  -  1; i ++) {
    for (j = i + 1; j < READ_TIMES; j ++) {
      if (Read_Buff[i] > Read_Buff[j]) {
        Read_Temp = Read_Buff[i];
        Read_Buff[i] = Read_Buff[j];
        Read_Buff[j] = Read_Temp;
      }
    }
  }

  //Exclude the largest and the smallest
  for (i = LOST_NUM; i < READ_TIMES - LOST_NUM; i ++)
      Read_Sum += Read_Buff[i];

  //Averaging
  Read_Temp = Read_Sum / (READ_TIMES - 2 * LOST_NUM);

  return Read_Temp;
}

/*******************************************************************************
function:
		Read X channel and Y channel AD value
parameter:
	Channel_Cmd :	0x90 :Read channel Y +
					0xd0 :Read channel x +
*******************************************************************************/
void TP_Read_ADC_XY(uint16_t *pXCh_Adc, uint16_t  *pYCh_Adc )
{
    *pXCh_Adc = TP_Read_ADC_Average(0xD0);
    *pYCh_Adc = TP_Read_ADC_Average(0x90);
}

/*******************************************************************************
function:
		2 times to read the touch screen IC, and the two can not exceed the deviation,
		ERR_RANGE, meet the conditions, then that the correct reading, otherwise the reading error.
parameter:
	Channel_Cmd :	pYCh_Adc = 0x90 :Read channel Y +
					pXCh_Adc = 0xd0 :Read channel x +
*******************************************************************************/
#define ERR_RANGE 50	//tolerance scope
static bool TP_Read_TwiceADC(uint16_t *pXCh_Adc, uint16_t  *pYCh_Adc )
{
  uint16_t XCh_Adc1, YCh_Adc1, XCh_Adc2, YCh_Adc2;

  //Read the ADC values Read the ADC values twice
  TP_Read_ADC_XY(&XCh_Adc1, &YCh_Adc1);
	//sleep_us(10);
  TP_Read_ADC_XY(&XCh_Adc2, &YCh_Adc2);
	//sleep_us(10);
	
  //The ADC error used twice is greater than ERR_RANGE to take the average
  // if( ((XCh_Adc2 <= XCh_Adc1 && XCh_Adc1 < XCh_Adc2 + ERR_RANGE) ||
  //       (XCh_Adc1 <= XCh_Adc2 && XCh_Adc2 < XCh_Adc1 + ERR_RANGE))
  //       && ((YCh_Adc2 <= YCh_Adc1 && YCh_Adc1 < YCh_Adc2 + ERR_RANGE) ||
  //       (YCh_Adc1 <= YCh_Adc2 && YCh_Adc2 < YCh_Adc1 + ERR_RANGE))) {

  if ((abs(XCh_Adc2 - XCh_Adc1) < ERR_RANGE) && (abs(YCh_Adc2 - YCh_Adc1) < ERR_RANGE)) {
    *pXCh_Adc = (XCh_Adc1 + XCh_Adc2) / 2;
    *pYCh_Adc = (YCh_Adc1 + YCh_Adc2) / 2;
    return true;
  }

  //The ADC error used twice is less than ERR_RANGE returns failed
  return false;
}

TP_XY slope;
TP_XY offset;

// returns -1 (no touch)
//    0 .. 299 16x16 pixel grid location
//    0: top left
//   19: top right
//  280: bottom left
//  299: bottom right 
static int16_t TP_Scan(void) {
  // In X, Y coordinate measurement, IRQ is disabled and output is low
  uint16_t x;
  uint16_t y;
  if (!gpio_get(TP_IRQ_PIN)  &&  TP_Read_TwiceADC(&y, &x)) { 
    return ((((long)(x - offset.x) * 10l) / (long) slope.x) >> 4)
       + ((((long)(y - offset.y) * 10l) / (long) slope.y) >> 4) * 20;
  } else {
    return -1;
  }
}

bool raw_touch(TP_XY *touch) {
  return (gpio_get(TP_IRQ_PIN)) ? false : TP_Read_TwiceADC(&(touch->y), &(touch->x));
}

void calibrate(TP_XY raw[4]) {
  // calculate calibration

  // we average two readings and divide them by half and store them as scaled integers 10 times their actual, fractional value
  // the x points are located at 20 and 300 on x axis, hence, the delta x is 280, we take 28 instead, to preserve fractional value,
  // there are two readings (x1,x2) and (x3, x4). Hence, we have to divide by 28 * 2 = 56 
  slope.x = ((raw[3].x  -  raw[2].x) + (raw[1].x - raw[0].x)) / 56;

  // the y points are located at 20 and 220 on the y axis, hence, the delta is 200. we take it as 20 instead, to preserve the fraction value 
  // there are two readings (y1, y2) and (y3, y4). Hence we have to divide by 20 * 2 = 40
  slope.y = ((raw[3].y - raw[0].y) + (raw[3].y - raw[1].y)) / 40;

  // x0, y0 is at 20,20 pixels
  offset.x = raw[0].x - ((20 * slope.x) / 10);
  offset.y = raw[0].y - ((20 * slope.y) / 10);

  // for (uint8_t i=0; i < 4; i++) {
  //   printf("raw[%d] (%d,%d)\r\n", i, raw[i].x, raw[i].y);
  // }
  printf("\r\n");
  printf("offset.x %d, slope.x %d\r\n", offset.x, slope.x);
  printf("offset.y %d, slope.y %d\r\n", offset.y, slope.y);

}

/*******************************************************************************
function:
		Use the default calibration factor
*******************************************************************************/
static void TP_GetAdFac(void)
{
  offset.x = 322;                                                                                                                                                             
  slope.x = 111;                                                                                                                                                        
  slope.y = -151;                                                                                                                                                        
  offset.y = 3768;
}

/*******************************************************************************
function:
		Touch pad initialization
*******************************************************************************/
void TP_Init(void)
{
  uint8_t cmd = 0;
  gpio_put(TP_CS_PIN, 0);
  spi_write_blocking(SPI_PORT, &cmd, 1);
  gpio_put(TP_CS_PIN, 1);

  TP_GetAdFac();
}

uint16_t current_grid = -1;
uint8_t press_counter = 0;
bool flashed = false;

const uint8_t HOLD_CT = 8;
const uint8_t PRESS_CT = 1;

void TP_Update(void) {
  if (flashed) {
    Invert(false);
    flashed = false;
  }
  int16_t down_grid = TP_Scan();
  if (down_grid > -1) {
    if (down_grid == current_grid) {
      if (press_counter < 255) {
        press_counter++;
        if (press_counter == HOLD_CT ) {
          Invert(true);
          flashed = true;
        }
      }
    } else {
      if (current_grid > -1) {
        clear_grid = current_grid;
      }
      highlight_grid = down_grid;
      current_grid = down_grid;
      press_counter = 0;
    }
  } else {
    if (current_grid > -1) {
      clear_grid = current_grid;
      if (press_counter >= HOLD_CT) {
        hold_grid = current_grid;
      } else /* if (press_counter >= PRESS_CT) */ {
        press_grid = current_grid;
      }
    }
    current_grid = -1;
    press_counter = 0;
  }
}
