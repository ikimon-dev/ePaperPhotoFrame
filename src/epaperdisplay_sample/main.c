
#include <math.h>
#include <stdio.h>
#include <string.h>

//
#include "e-Paper/EPD_IT8951.h"
//
// #include "Config/DEV_Config.h"
//
#include "pico/stdlib.h"
//
// #include "GUI/GUI_Paint.h"
//
// #include "GUI/GUI_BMPfile.h"
//
// #include "example.h"
//
#include "fatfs/ff.h"
//
#include "fatfs/diskio.h"

// #include "jpeg/jpeglib.h"
// #include "jpeg/jconfig.h"
// #include "jpeg/jmorecfg.h"

// epaper向け定義

UWORD VCOM;

UWORD D = 1560;
IT8951_Dev_Info Dev_Info;

UWORD Panel_Width;
UWORD Panel_Height;
UDOUBLE Init_Target_Memory_Addr;
int epd_mode = 0;  // 0: no rotate, no mirror
                   // 1: no rotate, horizontal mirror, for 10.3inch
                   // 2: no totate, horizontal mirror, for 5.17inch
                   // 3: no rotate, no mirror, isColor, for 6inch color

// fatfs向け定義

#define DEF_FATBUFF 140400
char buff_fattest[DEF_FATBUFF];

// LED向け定義

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

// BMPファイル向け定義

#pragma pack(2)
typedef struct BITMAPFILEHEADER {
  uint16_t bfType;      /**< ファイルタイプ、必ず"BM" */
  uint32_t bfSize;      /**< ファイルサイズ */
  uint16_t bfReserved1; /**< リザーブ */
  uint16_t bfReserved2; /**< リサーブ */
  uint32_t bfOffBits;   /**< 先頭から画像情報までのオフセット */
} BITMAPFILEHEADER;
#pragma pack()
typedef struct BITMAPINFOHEADER {
  uint32_t biSize;         /**< この構造体のサイズ */
  int32_t biWidth;         /**< 画像の幅 */
  int32_t biHeight;        /**< 画像の高さ */
  uint16_t biPlanes;       /**< 画像の枚数、通常1 */
  uint16_t biBitCount;     /**< 一色のビット数 */
  uint32_t biCompression;  /**< 圧縮形式 */
  uint32_t biSizeImage;    /**< 画像領域のサイズ */
  int32_t biXPelsPerMeter; /**< 画像の横方向解像度情報 */
  int32_t biYPelsPerMeter; /**< 画像の縦方向解像度情報*/
  uint32_t biClrUsed; /**< カラーパレットのうち実際に使っている色の個数 */
  uint32_t biClrImportant; /**< カラーパレットのうち重要な色の数 */
} BITMAPINFOHEADER;

BITMAPFILEHEADER bitmap_file_header;
BITMAPINFOHEADER bitmap_info_header;

// 出力画像向け定義

#define DEF_OUTPUTBUFF 46800
char buff_output[DEF_OUTPUTBUFF];

int fat_init() {
  DSTATUS ret;
  int result = 0;

  ret = disk_initialize(0);
  if (ret & STA_NOINIT) {
    result = -1;
  }

  return result;
}

int fat_read(char *buff, int bsize) {
  FRESULT ret;
  FATFS fs;
  FIL fil;
  UINT rdsz;

  ret = f_mount(&fs, "", 0);
  if (ret != FR_OK) {
    return -1;
  }
  ret = f_open(&fil, "test.bmp", FA_READ);
  if (ret != FR_OK) {
    return -2;
  }

  ret = f_read(&fil, buff, (UINT)bsize, &rdsz);
  if (ret != FR_OK) {
    return -3;
  }

  f_close(&fil);

  return (int)rdsz;
}

int main() {
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  int ret = fat_init();

  // epaperの初期化

  DEV_Module_Init();
  VCOM = (UWORD)(fabs(D) * 1000);

  Dev_Info = EPD_IT8951_Init(VCOM);

  Panel_Width = Dev_Info.Panel_W;
  Panel_Height = Dev_Info.Panel_H;

  Init_Target_Memory_Addr =
      Dev_Info.Memory_Addr_L | (Dev_Info.Memory_Addr_H << 16);

  A2_Mode = 6;

  // EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);
  
  EPD_IT8951_Display_Area(0, 0, Dev_Info.Panel_W, Dev_Info.Panel_H, INIT_Mode);


  sleep_ms(1000);

  // Display_ColorPalette_Example(10, 250, Init_Target_Memory_Addr);

  // sleep_ms(100);

  while (ret != 0) {
    ret = fat_init();
  }

  FRESULT f_ret;
  FATFS fs;
  FIL fil;
  UINT rdsz;

  f_ret = f_mount(&fs, "", 0);
  if (f_ret != FR_OK) {
    return -1;
  }

  f_ret = FR_NOT_READY;

  DIR dir;
  static FILINFO fno;
  for (;;) {
    f_ret = f_opendir(&dir, "image"); /* Open the directory */
    if (f_ret == FR_OK) {
      for (;;) {
        f_ret = f_readdir(&dir, &fno); /* Read a directory item */
        if (f_ret != FR_OK || fno.fname[0] == 0) {
          break; /* Break on error or end of dir */
        }
        if (!(fno.fattrib & AM_DIR)) { /* It is a file. */

          char filepath[255] = "image/";

          strcat(filepath, fno.fname);

          do {
            f_ret = f_open(&fil, filepath, FA_READ);
          } while (f_ret != FR_OK);

          f_ret = f_read(&fil, &bitmap_file_header, sizeof(BITMAPFILEHEADER),
                         &rdsz);
          f_ret = f_read(&fil, &bitmap_info_header, sizeof(BITMAPINFOHEADER),
                         &rdsz);

          f_ret = f_read(
              &fil, buff_fattest,
              (UINT)(bitmap_file_header.bfOffBits - sizeof(BITMAPFILEHEADER) -
                     sizeof(BITMAPINFOHEADER)),
              &rdsz);

          for (int i = 0; i < Panel_Height / 25; i++) {
            f_ret = f_read(&fil, buff_fattest, Panel_Width * 25 * 3, &rdsz);

            if (f_ret != FR_OK) {
              break;
            }

            double red = 0;
            double green = 0;
            double blue = 0;

            for (int j = 1; j <= 25; j++) {
              for (int k = 0; k < Panel_Width; k++) {  // RGB
                red =
                    0.2126 * buff_fattest[(Panel_Width * (j - 1) + k) * 3 + 2];
                green =
                    0.7152 * buff_fattest[(Panel_Width * (j - 1) + k) * 3 + 1];
                blue = 0.0722 * buff_fattest[(Panel_Width * (j - 1) + k) * 3];
                buff_output[Panel_Width * (25 - j) + k] = (red + green + blue);
              }
            }

            // Paint_NewImage(buff_output, (UINT)Panel_Width, 25, ROTATE_0,
            // WHITE);

            // EPD_IT8951_8bp_Refresh(
            //     buff_output, 0, (Panel_Height / 25 - 1 - i) * 25,
            //     Panel_Width, 25, true, Init_Target_Memory_Addr);

            IT8951_Load_Img_Info Load_Img_Info;
            IT8951_Area_Img_Info Area_Img_Info;

            EPD_IT8951_WaitForDisplayReady();

            Load_Img_Info.Source_Buffer_Addr = (UDOUBLE)buff_output;
            Load_Img_Info.Endian_Type = IT8951_LDIMG_L_ENDIAN;
            Load_Img_Info.Pixel_Format = IT8951_8BPP;
            Load_Img_Info.Rotate = IT8951_ROTATE_0;
            Load_Img_Info.Target_Memory_Addr = Init_Target_Memory_Addr;

            Area_Img_Info.Area_X = 0;
            Area_Img_Info.Area_Y = (Panel_Height / 25 - 1 - i) * 25;
            Area_Img_Info.Area_W = Panel_Width;
            Area_Img_Info.Area_H = 25;

            UWORD *Source_Buffer = (UWORD *)Load_Img_Info.Source_Buffer_Addr;
            EPD_IT8951_SetTargetMemoryAddr(Load_Img_Info.Target_Memory_Addr);
            EPD_IT8951_LoadImgAreaStart(&Load_Img_Info, &Area_Img_Info);

            UWORD Source_Buffer_Width, Source_Buffer_Height;

            Source_Buffer_Width = (Area_Img_Info.Area_W * 8 / 8) / 2;
            Source_Buffer_Height = Area_Img_Info.Area_H;

            for (UDOUBLE j = 0; j < Source_Buffer_Height; j++) {
              for (UDOUBLE k = 0; k < Source_Buffer_Width; k++) {
                EPD_IT8951_WriteData(*Source_Buffer);
                Source_Buffer++;
              }
            }

            sleep_ms(1);
          }

          EPD_IT8951_Display_Area(0, 0, Dev_Info.Panel_W, Dev_Info.Panel_H,
                                  INIT_Mode);

          sleep_ms(1000);

          EPD_IT8951_Display_Area(0, 0, Dev_Info.Panel_W, Dev_Info.Panel_H,
                                  GC16_Mode);

          sleep_ms(1000);

          EPD_IT8951_LoadImgEnd();

          f_close(&fil);

          sleep_ms(10000);
        }
      }
      f_closedir(&dir);
    }
  }

  return 0;
}
