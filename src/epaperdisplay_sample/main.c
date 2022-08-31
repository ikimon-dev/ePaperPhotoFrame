
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"

#include "./e-Paper/EPD_IT8951.h"
#include "./GUI/GUI_Paint.h"
#include "./GUI/GUI_BMPfile.h"
#include "./e-Paper/EPD_IT8951.h"
#include "./Config/DEV_Config.h"

#include "fatfs/ff.h"
#include "fatfs/diskio.h"

// epaper向け定義

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

// fatfs向け定義

#define DEF_FATBUFF 1024
char buff_fattest[DEF_FATBUFF];

// LED向け定義

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

int fat_init()
{
    DSTATUS ret;
    int result = 0;

    ret = disk_initialize(0);
    if (ret & STA_NOINIT)
    {
        result = -1;
    }

    return result;
}

int fat_read(char *buff, int bsize)
{
    FRESULT ret;
    FATFS fs;
    FIL fil;
    UINT rdsz;

    ret = f_mount(&fs, "", 0);
    if (ret != FR_OK)
    {
        return -1;
    }
    ret = f_open(&fil, "test.bmp", FA_READ);
    if (ret != FR_OK)
    {
        return -2;
    }

    ret = f_read(&fil, buff, (UINT)bsize, &rdsz);
    if (ret != FR_OK)
    {
        return -3;
    }

    f_close(&fil);

    return (int)rdsz;
}

int main()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // epaperの初期化

    DEV_Module_Init();
    VCOM = (UWORD)(fabs(D) * 1000);

    Dev_Info = EPD_IT8951_Init(VCOM);

    Panel_Width = Dev_Info.Panel_W;
    Panel_Height = Dev_Info.Panel_H;

    Init_Target_Memory_Addr = Dev_Info.Memory_Addr_L | (Dev_Info.Memory_Addr_H << 16);

    A2_Mode = 6;

    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);

    int ret = fat_init();
    ret = fat_read(buff_fattest, DEF_FATBUFF);

    return 0;
}
