/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-09-17
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of theex Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"

#include "DEV_Config.h"
#include <fcntl.h>

/******************************************************************************
function:	GPIO Write
parameter:
Info:
******************************************************************************/
void DEV_Digital_Write(UWORD Pin, UBYTE Value)
{
	gpio_put(Pin, Value == 0 ? 0 : 1);
}

/******************************************************************************
function:	GPIO Read
parameter:
Info:
******************************************************************************/
UBYTE DEV_Digital_Read(UWORD Pin)
{
	return gpio_get(Pin);
}

/******************************************************************************
function:	SPI Write
parameter:
Info:
******************************************************************************/
void DEV_SPI_WriteByte(UBYTE Value)
{
	spi_write_blocking(SPI_PORT, &Value, 1);
}

/******************************************************************************
function:	SPI Read
parameter:
Info:
******************************************************************************/
UBYTE DEV_SPI_ReadByte()
{
	UBYTE Read_Value = 0x00;

	spi_read_blocking(SPI_PORT, 0x00, &Read_Value, 1);

	return Read_Value;
}

/******************************************************************************
function:	Time delay for ms
parameter:
Info:
******************************************************************************/
void DEV_Delay_ms(UDOUBLE xms)
{
	sleep_ms(xms);
}

/******************************************************************************
function:	Time delay for us
parameter:
Info:
******************************************************************************/
void DEV_Delay_us(UDOUBLE xus)
{
	sleep_us(xus);
}

/**
 * GPIO Mode
 **/
static void DEV_GPIO_Mode(UWORD Pin, UWORD Mode)
{

	gpio_init(Pin);

	if (Mode == 0 || Mode == GPIO_IN)
	{
		gpio_set_dir(Pin, GPIO_IN);
	}
	else
	{
		gpio_set_dir(Pin, GPIO_OUT);
	}
}

/**
 * GPIO Init
 **/
static void DEV_GPIO_Init(void)
{
	DEV_GPIO_Mode(EPD_RST_PIN, GPIO_OUT);
	gpio_pull_down(EPD_RST_PIN);
	DEV_GPIO_Mode(EPD_CS_PIN, GPIO_OUT);
	gpio_pull_down(EPD_CS_PIN);
	DEV_GPIO_Mode(EPD_BUSY_PIN, GPIO_IN);
	gpio_pull_down(EPD_BUSY_PIN);

	DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function:	Module Initialize, the library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
UBYTE DEV_Module_Init(void)
{
	// Debug("/***********************************/ \r\n");

	// if (!bcm2835_init())
	// {
	// 	Debug("bcm2835 init failed  !!! \r\n");
	// 	return 1;
	// }
	// else
	// {
	// 	Debug("bcm2835 init success !!! \r\n");
	// }

	// This example will use SPI0 at 10MHz.
	spi_init(SPI_PORT, 10 * 1000 * 1000);

	gpio_set_function(PIN_CLK, GPIO_FUNC_SPI);
	gpio_pull_down(PIN_CLK);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
	gpio_pull_down(PIN_MOSI);
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
	gpio_pull_down(PIN_MISO);

	bi_decl(bi_3pins_with_func(PIN_MOSI, PIN_MISO, PIN_CLK, GPIO_FUNC_SPI));

	// bcm2835_spi_begin();									 // Start spi interface, set spi pin for the reuse function
	// bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); // High first transmission
	// bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);				 // spi mode 0
	// bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16);   //For RPi3/3B/3B+
	// bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32); // For RPi 4
	/* SPI clock reference link：*/
	/*http://www.airspayce.com/mikem/bcm2835/group__constants.html#gaf2e0ca069b8caef24602a02e8a00884e*/

	// GPIO Config
	DEV_GPIO_Init();

	// Debug("/***********************************/ \r\n");
	return 0;
}

/******************************************************************************
function:	Module exits, closes SPI and BCM2835 library
parameter:
Info:
******************************************************************************/
void DEV_Module_Exit(void)
{
	DEV_Digital_Write(EPD_CS_PIN, 0);
	DEV_Digital_Write(EPD_RST_PIN, 0);
}
