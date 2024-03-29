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
#include "example.h"

#define BACK_LIGHT_PIN

UWORD VCOM;

UWORD D = 1560;
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

    // Enhance_Driving_Capability();

    A2_Mode = 6;

    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);

    // Show 16 grayscale
    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);
    Display_ColorPalette_Example(250, 1404, Init_Target_Memory_Addr);
    // EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);

    UWORD WIDTH;
    if (Four_Byte_Align == true)
    {
        WIDTH = 200 - (200 % 32);
    }
    else
    {
        WIDTH = 200;
    }
    UWORD HEIGHT = 200;

    UDOUBLE Imagesize;

    Imagesize = ((WIDTH * BitsPerPixel_4 % 8 == 0) ? (WIDTH * BitsPerPixel_4 / 8) : (WIDTH * BitsPerPixel_4 / 8 + 1)) * HEIGHT;
    if ((Refresh_Frame_Buf = (UBYTE *)malloc(Imagesize)) == NULL)
    {
        // Debug("Failed to apply for black memory...\r\n");
        return -1;
    }

    Paint_NewImage(Refresh_Frame_Buf, WIDTH, HEIGHT, 0, BLACK);
    Paint_SelectImage(Refresh_Frame_Buf);
    // Epd_Mode(0);
    Paint_SetBitsPerPixel(BitsPerPixel_4);
    Paint_Clear(WHITE);

    uint8_t buf[40000] = {128};

    UBYTE SixteenColorPattern[16] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00};

    for (int i = 0; i < 16; i++)
    {
        memset(buf, SixteenColorPattern[i], 40000);

        EPD_IT8951_4bp_Refresh(buf, 200, 200, WIDTH, HEIGHT, false, Init_Target_Memory_Addr, false);

        sleep_ms(1000);
    }
    // Show some character and pattern
    // Display_CharacterPattern_Example(Panel_Width, Panel_Height, Init_Target_Memory_Addr, BitsPerPixel_4);
    // EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, GC16_Mode);

    // Show a bmp file
    // 1bp use A2 mode by default, before used it, refresh the screen with WHITE
    // Display_BMP_Example(Panel_Width, Panel_Height, Init_Target_Memory_Addr, BitsPerPixel_1);
    // Display_BMP_Example(Panel_Width, Panel_Height, Init_Target_Memory_Addr, BitsPerPixel_2);
    // Display_BMP_Example(Panel_Width, Panel_Height, Init_Target_Memory_Addr, BitsPerPixel_4);
    // EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, GC16_Mode);

    // Show A2 mode refresh effect
    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);
    Dynamic_Refresh_Example(Dev_Info, Init_Target_Memory_Addr);
    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);
    // EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, GC16_Mode);

    // Show how to display a gif, not works well on 6inch e-Paper HAT, 9.7inch e-Paper HAT, others work well
    // EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, A2_Mode);
    // Dynamic_GIF_Example(Panel_Width, Panel_Height, Init_Target_Memory_Addr);
    // EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, A2_Mode);
    // EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, GC16_Mode);

    // Show how to test frame rate, test it individually,which is related to refresh area size and refresh mode
    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);
    Check_FrameRate_Example(500, 500, Init_Target_Memory_Addr, BitsPerPixel_4);
    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);
    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);

    // We recommended refresh the panel to white color before storing in the warehouse.
    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);

    // EPD_IT8951_Standby();
    EPD_IT8951_Sleep();

    // In case RPI is transmitting image in no hold mode, which requires at most 10s
    DEV_Delay_ms(5000);

    DEV_Module_Exit();

    while (true)
    {
        DEV_Digital_Write(LED_PIN, 1);

        sleep_ms(100);
        DEV_Digital_Write(LED_PIN, 0);
        sleep_ms(1500);
    }
}
