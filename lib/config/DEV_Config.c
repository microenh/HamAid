/*****************************************************************************
* | File      	:	DEV_Config.c
* | Author      :   Waveshare team
* | Function    :	Show SDcard BMP picto LCD 
* | Info        :
*   Provide the hardware underlying interface	 
*----------------
* |	This version:   V1.0
* | Date        :   2018-01-11
* | Info        :   Basic version
*
******************************************************************************/
#include "DEV_Config.h"

// per documentation
const uint8_t SDIO_CLK_PIN = 5;			// [mee] added
const uint8_t LCD_DC_PIN = 8;
const uint8_t LCD_CS_PIN = 9;
const uint8_t SPI_CLK_PIN = 10;
const uint8_t SPI_MOSI_PIN = 11;
const uint8_t LCD_MISO_PIN = 12;
const uint8_t LCD_BKL_PIN = 13;

const uint8_t LCD_RST_PIN = 15;
const uint8_t TP_CS_PIN = 16;
const uint8_t TP_IRQ_PIN = 17;
const uint8_t SDIO_CMD_PIN = 18;		// [mee] added
const uint8_t SDIO_D0_PIN = 19;			// [mee] added
const uint8_t SDIO_D1_PIN = 20;			// [mee] added
const uint8_t SDIO_D2_PIN =  21;		// [mee] added
const uint8_t SD_CS_PIN = 22; 			// also SD_D3

spi_inst_t * SPI_PORT = (spi_inst_t *) spi1_hw;


#if 0
void DEV_Digital_Write(UWORD Pin, UBYTE Value)
{
	gpio_put(Pin, Value);
}

UBYTE DEV_Digital_Read(UWORD Pin)
{
	return gpio_get(Pin);
}
#endif

/**
 * GPIO Mode
**/
void DEV_GPIO_Mode(UWORD Pin, UWORD Mode)
{
    gpio_init(Pin);
	gpio_set_dir(Pin, (Mode == 0 || Mode == GPIO_IN) ? GPIO_IN : GPIO_OUT);
	#if 0
    if (Mode == 0 || Mode == GPIO_IN) {
        gpio_set_dir(Pin, GPIO_IN);
    } else {
        gpio_set_dir(Pin, GPIO_OUT);
    }
	#endif
}

void DEV_GPIO_Init(void)
{
	DEV_GPIO_Mode(LCD_RST_PIN,GPIO_OUT);
    DEV_GPIO_Mode(LCD_DC_PIN, GPIO_OUT);
    DEV_GPIO_Mode(LCD_BKL_PIN, GPIO_OUT);
    DEV_GPIO_Mode(LCD_CS_PIN, GPIO_OUT);
    DEV_GPIO_Mode(TP_CS_PIN,GPIO_OUT);
    DEV_GPIO_Mode(TP_IRQ_PIN,GPIO_IN);
    DEV_GPIO_Mode(SD_CS_PIN,GPIO_OUT);
	gpio_set_pulls(TP_IRQ_PIN,true,false);

    DEV_Digital_Write(TP_CS_PIN, 1);
    DEV_Digital_Write(LCD_CS_PIN, 1);
    DEV_Digital_Write(LCD_BKL_PIN, 1);
    DEV_Digital_Write(SD_CS_PIN, 1);
}


/********************************************************************************
function:	System Init
note:
	Initialize the communication method
********************************************************************************/
uint8_t System_Init(void)
{
	stdio_init_all();
	DEV_GPIO_Init();
	spi_init(SPI_PORT,4000000);
	gpio_set_function(SPI_CLK_PIN,GPIO_FUNC_SPI);
	gpio_set_function(SPI_MOSI_PIN,GPIO_FUNC_SPI);
	gpio_set_function(LCD_MISO_PIN,GPIO_FUNC_SPI);

    return 0;
}

void System_Exit(void)
{

}

/*********************************************
function:	Hardware interface
note:
	SPI4W_Write_Byte(value) : 
		Register hardware SPI
*********************************************/	
uint8_t SPI4W_Write_Byte(uint8_t value)                                    
{   
	uint8_t rxDat;
	spi_write_read_blocking(spi1, &value, &rxDat, 1);
    return rxDat;
}

#if 0
uint8_t SPI4W_Read_Byte(uint8_t value)                                    
{
	return SPI4W_Write_Byte(value);
}
#endif

#if 0
/********************************************************************************
function:	Delay function
note:
	Driver_Delay_ms(xms) : Delay x ms
	Driver_Delay_us(xus) : Delay x us
********************************************************************************/
void Driver_Delay_ms(uint32_t xms)
{
	sleep_ms(xms);
}


void Driver_Delay_us(uint32_t xus)
{
	sleep_us(xus);
	// int j;
    // for(j=xus; j > 0; j--);
}
#endif
