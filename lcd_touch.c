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


extern uint8_t id;
static TP_DEV sTP_DEV;
static TP_DRAW sTP_Draw;
/*******************************************************************************
function:
		Read the ADC of the channel
parameter:
	Channel_Cmd :	0x90: Read channel Y +, select the ADC resolution is 12 bits, set to differential mode
					0xd0: Read channel x +, select the ADC resolution is 12 bits, set to differential mode
*******************************************************************************/
static uint16_t TP_Read_ADC(uint8_t CMD)
{
    uint16_t Data = 0;

    //A cycle of at least 400ns.
    DEV_Digital_Write(TP_CS_PIN,0);

    SPI4W_Write_Byte(CMD);
    Driver_Delay_us(200);

    //	dont write 0xff, it will block xpt2046  
    //Data = SPI4W_Read_Byte(0Xff);
    Data = SPI4W_Read_Byte(0X00);
    Data <<= 8;//7bit
    Data |= SPI4W_Read_Byte(0X00);
    //Data = SPI4W_Read_Byte(0Xff);
    Data >>= 3;//5bit
    DEV_Digital_Write(TP_CS_PIN,1);
    return Data;
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
		Driver_Delay_us(200);
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
static void TP_Read_ADC_XY(uint16_t *pXCh_Adc, uint16_t  *pYCh_Adc )
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
	Driver_Delay_us(10);
  TP_Read_ADC_XY(&XCh_Adc2, &YCh_Adc2);
	Driver_Delay_us(10);
	
  //The ADC error used twice is greater than ERR_RANGE to take the average
  if( ((XCh_Adc2 <= XCh_Adc1 && XCh_Adc1 < XCh_Adc2 + ERR_RANGE) ||
        (XCh_Adc1 <= XCh_Adc2 && XCh_Adc2 < XCh_Adc1 + ERR_RANGE))
        && ((YCh_Adc2 <= YCh_Adc1 && YCh_Adc1 < YCh_Adc2 + ERR_RANGE) ||
        (YCh_Adc1 <= YCh_Adc2 && YCh_Adc2 < YCh_Adc1 + ERR_RANGE))) {
      *pXCh_Adc = (XCh_Adc1 + XCh_Adc2) / 2;
      *pYCh_Adc = (YCh_Adc1 + YCh_Adc2) / 2;
      return true;
  }

  //The ADC error used twice is less than ERR_RANGE returns failed
  return false;
}

/*******************************************************************************
function:
		Calculation
parameter:
		chCoordType:
					1 : calibration
					0 : relative position
*******************************************************************************/
static uint8_t TP_Scan(uint8_t chCoordType) {
  // In X, Y coordinate measurement, IRQ is disabled and output is low
  if (!DEV_Digital_Read(TP_IRQ_PIN)) { //Press the button to press
    // Read the physical coordinates
    if (chCoordType) {
      TP_Read_TwiceADC(&sTP_DEV.Xpoint, &sTP_DEV.Ypoint);
      // Read the screen coordinates
    } else if (TP_Read_TwiceADC(&sTP_DEV.Xpoint, &sTP_DEV.Ypoint)) {
      sTP_Draw.Xpoint = lcd.width -
                  sTP_DEV.fXfac * sTP_DEV.Xpoint -
                  sTP_DEV.iXoff;
      sTP_Draw.Ypoint = lcd.height -
                  sTP_DEV.fYfac * sTP_DEV.Ypoint -
                  sTP_DEV.iYoff;
      if (0 == (sTP_DEV.chStatus & TP_PRESS_DOWN)) {	//Not being pressed
          sTP_DEV.chStatus = TP_PRESS_DOWN | TP_PRESSED;
          sTP_DEV.Xpoint0 = sTP_DEV.Xpoint;
          sTP_DEV.Ypoint0 = sTP_DEV.Ypoint;
      }
    } else {
      if (sTP_DEV.chStatus & TP_PRESS_DOWN) {	//0x80
          sTP_DEV.chStatus &= ~(1 << 7);		//0x00
      } else {
          sTP_DEV.Xpoint0 = 0;
          sTP_DEV.Ypoint0 = 0;
          sTP_DEV.Xpoint = 0xffff;
          sTP_DEV.Ypoint = 0xffff;
      }
    }
  }
  return (sTP_DEV.chStatus & TP_PRESS_DOWN);
}

/*******************************************************************************
function:
		Use the default calibration factor
*******************************************************************************/
void TP_GetAdFac(void)
{
  // sTP_DEV.fXfac = 0.066626;
  // sTP_DEV.fYfac = 0.089779;
  // sTP_DEV.iXoff = -20;
  // sTP_DEV.iYoff = -34;

  sTP_DEV.fXfac = 0.066749;                                                                                                                                                        
  sTP_DEV.fYfac = 0.090881;                                                                                                                                                        
  sTP_DEV.iXoff = -13;                                                                                                                                                             
  sTP_DEV.iYoff = -33;
}

/*******************************************************************************
function:
		Draw Board
*******************************************************************************/
void TP_DrawBoard(void)
{
  TP_Scan(0);
  if (sTP_DEV.chStatus & TP_PRESS_DOWN) {
    //Horizontal screen
    if (sTP_Draw.Xpoint < lcd.width &&
        //Determine whether the law is legal
        sTP_Draw.Ypoint < lcd.height) {
      if(lcd.width > lcd.height) {
        printf("horizontal x:%d,y:%d\n",sTP_Draw.Xpoint,sTP_Draw.Ypoint);
      } else {
        printf("Vertical x:%d,y:%d\n",sTP_Draw.Xpoint,sTP_Draw.Ypoint);
      }
    }
    sTP_DEV.chStatus = 0;
  }
}


/*******************************************************************************
function:
		Touch pad initialization
*******************************************************************************/
void TP_Init(void)
{
  DEV_Digital_Write(TP_CS_PIN,1);
  TP_Read_ADC_XY(&sTP_DEV.Xpoint, &sTP_DEV.Ypoint);
}


