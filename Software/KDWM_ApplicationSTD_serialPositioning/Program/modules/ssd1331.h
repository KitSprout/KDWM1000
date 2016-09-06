/* #include "modules\ssd1331.h" */

#ifndef __SSD1331_H
#define __SSD1331_H

#include "stm32f4xx.h"
#include "fonts\fontlib.h"
#include "algorithms\string.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define OLED_W      96
#define OLED_H      64

#define FONT8       Font8x6
#define FONT16      Font16x8

#define RGB_TO_GARY(C_R, C_G, C_B)  ((uint8_t)(0.299f * C_R + 0.587f * C_G + 0.114f * C_B))
#define ToRGB565(RGB888)            ((uint16_t)((RGB888&0xF80000>>8)|(RGB888&0x00FC00>>5)|(RGB888&0x0000F8>>3)))
#define ToRGB888(RGB565)            ((uint16_t)((RGB565&0xF800<<8)|(RGB565&0x07E0<<5)|(RGB565&0x001F<<3)))

#define RGB565_R(RGB565)            ((uint8_t)((RGB565 >> 11) & 0x1F))
#define RGB565_G(RGB565)            ((uint8_t)((RGB565 >>  4) & 0x3F))
#define RGB565_B(RGB565)            ((uint8_t)((RGB565      ) & 0x1F))

// 輸入 R G B，輸出 RGB565
#define RGB(C_R, C_G, C_B)          ((uint32_t)(((C_R<<8)&0xF800)|((C_G<<3)&0x07E0)|((C_B>>3)&0x001F)))

#define LINE(__FONT, __INDEX)       (__FONT.height * __INDEX)
#define CENTER(__FONT, __STR)       ((OLED_W - __FONT.width * lenOfStr(__STR)) >> 1)


#define RED         ((uint16_t)0xF800)  /* 紅色 */
#define GREEN       ((uint16_t)0x07E0)  /* 綠色 */
#define BLUE        ((uint16_t)0x001F)  /* 藍色 */
#define MAGENTA     ((uint16_t)0xF81F)  /* 紅紫色，洋紅色 */
#define GRED        ((uint16_t)0xFFE0)
#define GBLUE       ((uint16_t)0x07FF) 
#define BRED        ((uint16_t)0XF81F)
#define BROWN       ((uint16_t)0xBC40)  /* 棕色 */
#define BRRED       ((uint16_t)0xFC07)  /* 棕紅色 */
#define CYAN        ((uint16_t)0x7FFF)  /* 藍綠色，青色 */
#define BLACK       ((uint16_t)0x0000)  /* 黑色 */
#define GRAY        ((uint16_t)0X8430)  /* 灰色 */
#define WHITE       ((uint16_t)0xFFFF)  /* 白色 */
#define YELLOW      ((uint16_t)0xFFE0)  /* 黃色 */
#define DARKBLUE    ((uint16_t)0x01CF)  /* 深藍色 */
#define LIGHTBLUE   ((uint16_t)0x7D7C)  /* 淺藍色 */
#define GRAYBLUE    ((uint16_t)0x5458)  /* 灰藍色 */
#define LIGHTGREEN  ((uint16_t)0X841F)  /* 淺綠色 */
#define LGRAY       ((uint16_t)0XC618)  /* 淺灰色 */
#define LGRAYBLUE   ((uint16_t)0XA651)  /* 淺灰藍色 */
#define LBBLUE      ((uint16_t)0X2B12)  /* 淺棕藍色 */
/*====================================================================================================*/
/*====================================================================================================*/
void SSD1331_Config( void );
void SSD1331_Init( void );

void OLED_Display( uint8_t onoff );
void OLED_Clear( uint16_t color );
void OLED_SetWindow( uint8_t posX1, uint8_t posY1, uint8_t posX2, uint8_t posY2 );

void OLED_DrawPixel( uint8_t posX, uint8_t posY, uint16_t color );
void OLED_DrawLineX( uint8_t posX, uint8_t posY, uint8_t length, uint16_t color );
void OLED_DrawLineY( uint8_t posX, uint8_t posY, uint8_t length, uint16_t color );
//void OLED_DrawLine( uint8_t posX1, uint8_t posY1, uint8_t posX2, uint8_t posY2, uint16_t color );
void OLED_DrawRect( uint8_t posX, uint8_t posY, uint8_t width, uint8_t height, uint16_t color );
void OLED_DrawRectFill( uint8_t posX, uint8_t posY, uint8_t width, uint8_t height, uint16_t color );
void OLED_DrawCircle( uint8_t posX, uint8_t posY, uint8_t radius, uint16_t color );
void OLED_DrawPic( uint8_t posX, uint8_t posY, uint8_t width, uint8_t height, const uint16_t *pic );

void OLED_PutChar( uint8_t posX, uint8_t posY, sFONT *pFont, uint8_t word, uint16_t fontColor, uint16_t backColor );
void OLED_PutStr( uint8_t posX, uint8_t posY, sFONT *pFont, char *pString, uint16_t fontColor, uint16_t backColor );
void OLED_PutNum( uint8_t posX, uint8_t posY, sFONT *pFont, StringType type, uint8_t lens, int32_t number, uint16_t fontColor, uint16_t backColor );

void OLED_TestColoBar( void );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
