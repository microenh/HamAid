/*****************************************************************************
* | File      	:	DEV_Config.c
* | Author      :   Waveshare team
* | Function    :	GPIO Function
* | Info        :
*   Provide the hardware underlying interface	 
*----------------
* |	This version:   V1.0
* | Date        :   2018-01-11
* | Info        :   Basic version
*
******************************************************************************/
#pragma once

#include <pico/stdlib.h>
#include <hardware/spi.h>
#include <stdio.h>
#include <hardware/gpio.h>

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

extern const uint8_t SDIO_CLK_PIN;
extern const uint8_t LCD_DC_PIN;
extern const uint8_t LCD_CS_PIN;
extern const uint8_t SPI_CLK_PIN;
extern const uint8_t SPI_MOSI_PIN;
extern const uint8_t LCD_MISO_PIN;
extern const uint8_t LCD_BKL_PIN;

extern const uint8_t LCD_RST_PIN;
extern const uint8_t TP_CS_PIN;
extern const uint8_t TP_IRQ_PIN;
extern const uint8_t SDIO_CMD_PIN;
extern const uint8_t SDIO_D0_PIN;
extern const uint8_t SDIO_D1_PIN;
extern const uint8_t SDIO_D2_PIN;
extern const uint8_t SD_CS_PIN;

extern spi_inst_t *SPI_PORT;

/*------------------------------------------------------------------------------------------------------*/

// #define DEV_Digital_Write(pin, value) gpio_put(pin, value)
// #define DEV_Digital_Read(pin) gpio_get(pin)

void DEV_GPIO_Mode(UWORD Pin, UWORD Mode);
void DEV_GPIO_Init(void);

uint8_t System_Init(void);
void System_Exit(void);
// uint8_t SPI4W_Write_Byte(uint8_t value);

// #define SPI4W_Read_Byte(value) SPI4W_Write_Byte(value)
// #define Driver_Delay_ms(xms) sleep_ms(xms)
// #define Driver_Delay_us(xus) sleep_us(xus)

