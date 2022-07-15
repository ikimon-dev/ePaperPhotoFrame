/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>

#include "pico/stdlib.h"

// #include "DEV_Config.h"

#include "./e-Paper/EPD_IT8951.h"
#include "./GUI/GUI_Paint.h"
#include "./GUI/GUI_BMPfile.h"
#include "./e-Paper/EPD_IT8951.h"
#include "./Config/DEV_Config.h"

#define BACK_LIGHT_PIN

UWORD VCOM;

UWORD D = 2510;
IT8951_Dev_Info Dev_Info;

UWORD Panel_Width;
UWORD Panel_Height;
UDOUBLE Init_Target_Memory_Addr;
int epd_mode = 0; // 0: no rotate, no mirror
                  // 1: no rotate, horizontal mirror, for 10.3inch
                  // 2: no totate, horizontal mirror, for 5.17inch
                  // 3: no rotate, no mirror, isColor, for 6inch color

int main()
{

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    DEV_Module_Init();
    VCOM = (UWORD)(fabs(D) * 1000);

    Dev_Info = EPD_IT8951_Init(VCOM);

    Panel_Width = Dev_Info.Panel_W;
    Panel_Height = Dev_Info.Panel_H;

    Init_Target_Memory_Addr = Dev_Info.Memory_Addr_L | (Dev_Info.Memory_Addr_H << 16);

    while (true)
    {
        DEV_Digital_Write(LED_PIN, 1);

        sleep_ms(250);
        DEV_Digital_Write(LED_PIN, 0);
        sleep_ms(250);
    }
}
