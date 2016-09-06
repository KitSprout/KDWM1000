/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "algorithms\string.h"
#include "modules\ssd1331.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define OLED_SPIx                   SPI2
#define OLED_SPIx_CLK_ENABLE()      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE)

#define OLED_DC_PIN                 GPIO_Pin_14
#define OLED_DC_GPIO_PORT           GPIOB
#define OLED_DC_H()                 __GPIO_SET(OLED_DC_GPIO_PORT, OLED_DC_PIN)
#define OLED_DC_L()                 __GPIO_RST(OLED_DC_GPIO_PORT, OLED_DC_PIN)

#define OLED_RST_PIN                GPIO_Pin_2
#define OLED_RST_GPIO_PORT          GPIOB
#define OLED_RST_H()                __GPIO_SET(OLED_RST_GPIO_PORT, OLED_RST_PIN)
#define OLED_RST_L()                __GPIO_RST(OLED_RST_GPIO_PORT, OLED_RST_PIN)

#define OLED_CS_PIN                 GPIO_Pin_12
#define OLED_CS_GPIO_PORT           GPIOB
#define OLED_CS_H()                 __GPIO_SET(OLED_CS_GPIO_PORT, OLED_CS_PIN)
#define OLED_CS_L()                 __GPIO_RST(OLED_CS_GPIO_PORT, OLED_CS_PIN)

#define OLED_SCK_PIN                GPIO_Pin_13
#define OLED_SCK_GPIO_PORT          GPIOB
#define OLED_SCK_AF                 GPIO_AF_SPI2
#define OLED_SCK_SOURCE             GPIO_PinSource13

#define OLED_SDI_PIN                GPIO_Pin_15
#define OLED_SDI_GPIO_PORT          GPIOB
#define OLED_SDI_AF                 GPIO_AF_SPI2
#define OLED_SDI_SOURCE             GPIO_PinSource15
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SSD1331_Config
**功能 : GPIO & SPI Config
**輸入 : None
**輸出 : None
**使用 : SSD1331_Config();
**====================================================================================================*/
/*====================================================================================================*/
void SSD1331_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;

  /* SPI Clk ******************************************************************/
  OLED_SPIx_CLK_ENABLE();

  /* SPI AF *******************************************************************/
  GPIO_PinAFConfig(OLED_SCK_GPIO_PORT, OLED_SCK_SOURCE, OLED_SCK_AF);
  GPIO_PinAFConfig(OLED_SDI_GPIO_PORT, OLED_SDI_SOURCE, OLED_SDI_AF);

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = OLED_DC_PIN;
  GPIO_Init(OLED_DC_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = OLED_RST_PIN;
  GPIO_Init(OLED_RST_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = OLED_CS_PIN;
  GPIO_Init(OLED_CS_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = OLED_SCK_PIN;
  GPIO_Init(OLED_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = OLED_SDI_PIN;
  GPIO_Init(OLED_SDI_GPIO_PORT, &GPIO_InitStruct);

  OLED_CS_H();  // LOW ENABLE

  /* SPI Init ****************************************************************/
  SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
  SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High;
  SPI_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial     = 7;
  SPI_Init(OLED_SPIx, &SPI_InitStruct);

  SPI_Cmd(OLED_SPIx, ENABLE);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_WriteCmd
**功能 : Write Command or Address
**輸入 : writeCmd
**輸出 : None
**使用 : OLED_WriteCmd(0xCB);
**====================================================================================================*/
/*====================================================================================================*/
static void OLED_WriteCmd( uint8_t writeCmd )
{
  OLED_CS_L();
  OLED_DC_L();
  SPI_RW(OLED_SPIx, writeCmd);
  OLED_CS_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_WriteData
**功能 : Write Data
**輸入 : writeData
**輸出 : None
**使用 : OLED_WriteData(Byte8H(Color));
**====================================================================================================*/
/*====================================================================================================*/
//static void OLED_WriteData( uint8_t writeData )
//{
//  OLED_CST_L();
//  OLED_DC_H();
//  SPI_RW(OLED_SPIx, writeData);
//  OLED_CST_H();
//}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_WriteColor
**功能 : Write Color
**輸入 : color
**輸出 : None
**使用 : OLED_WriteColor(BLACK);
**====================================================================================================*/
/*====================================================================================================*/
static void OLED_WriteColor( uint16_t color )
{
  OLED_CS_L();
  OLED_DC_H();
  SPI_RW(OLED_SPIx, Byte8H(color));
  SPI_RW(OLED_SPIx, Byte8L(color));
  OLED_CS_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SSD1331_Init
**功能 : SSD1331 Init
**輸入 : None
**輸出 : None
**使用 : SSD1331_Init();
**====================================================================================================*/
/*====================================================================================================*/
void SSD1331_Init( void )
{
  // Hardware Reset
  OLED_RST_L();
  delay_ms(15);
  OLED_RST_H();
  delay_ms(50);

  // Device Init
  OLED_Display(DISABLE); // Display Off

  OLED_WriteCmd(0x81);   // Set Contrast for Color A
  OLED_WriteCmd(0x91);   // 145
  OLED_WriteCmd(0x82);   // Set Contrast for Color B
  OLED_WriteCmd(0x50);   // 80
  OLED_WriteCmd(0x83);   // Set Contrast for Color C
  OLED_WriteCmd(0x7D);   // 125

	OLED_WriteCmd(0x87);   // Master Contrast Current Control
	OLED_WriteCmd(0x06);   // 6

  OLED_WriteCmd(0x8A);   // Set Second Pre-change Speed for Color A
  OLED_WriteCmd(0x64);   // 100
  OLED_WriteCmd(0x8B);   // Set Second Pre-change Speed for Color B
  OLED_WriteCmd(0x78);   // 120
  OLED_WriteCmd(0x8C);   // Set Second Pre-change Speed for Color C
  OLED_WriteCmd(0x64);   // 100

	OLED_WriteCmd(0xA0);   // Set Remap & Color Depth
	OLED_WriteCmd(0x72);   // 0x72

	OLED_WriteCmd(0xA1);   // Set Display Start Line
	OLED_WriteCmd(0x00);   // 0

	OLED_WriteCmd(0xA2);   // Set Display Offset
	OLED_WriteCmd(0x00);   // 0

	OLED_WriteCmd(0xA4);   // Set Display Mode

	OLED_WriteCmd(0xA8);   // Set Multiplex Ratio
	OLED_WriteCmd(0x3F);   // 0x3F

	OLED_WriteCmd(0xAD);   // Set  Master Configuration
	OLED_WriteCmd(0x8E);   // 0x8E

  OLED_WriteCmd(0xB0);   // Set Power Save Mode
	OLED_WriteCmd(0x00);   // 0x00

	OLED_WriteCmd(0xB1);   // Phase 1 and 2 Period Adjustment
	OLED_WriteCmd(0x31);   // 0x31

	OLED_WriteCmd(0xB3);   // Set Display Clock Divider / Oscillator Frequency
	OLED_WriteCmd(0xF0);   // 0xF0

	OLED_WriteCmd(0xBB);   // Set Pre-change Level
	OLED_WriteCmd(0x3A);   // 0x3A

	OLED_WriteCmd(0xBE);   // Set vcomH
	OLED_WriteCmd(0x3E);   // 0x3E

	OLED_WriteCmd(0x2E);   // Disable Scrolling

  OLED_Display(ENABLE);
  OLED_Clear(WHITE);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_Display
**功能 : OLED Display
**輸入 : onoff
**輸出 : None
**使用 : LCD_Display(ENABLE);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_Display( uint8_t onoff )
{
  if(onoff == ENABLE)
    OLED_WriteCmd(0xAF);
  else
    OLED_WriteCmd(0xAE);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_Clear
**功能 : Clear Window
**輸入 : color
**輸出 : None
**使用 : OLED_Clear(BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_Clear( uint16_t color )
{
  uint32_t point = OLED_W * OLED_H;

  OLED_SetWindow(0, 0, OLED_W - 1, OLED_H - 1);

  while(point--)
    OLED_WriteColor(color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_SetWindow
**功能 : Set Window
**輸入 : posX1, posY1, posX2, posY2
**輸出 : None
**使用 : OLED_SetWindow(X1, Y1, X2, Y2);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_SetWindow( uint8_t posX1, uint8_t posY1, uint8_t posX2, uint8_t posY2 )
{
  OLED_WriteCmd(0x15);
  OLED_WriteCmd(posX1);
  OLED_WriteCmd(posX2);
  OLED_WriteCmd(0x75);
  OLED_WriteCmd(posY1);
  OLED_WriteCmd(posY2);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawPixel
**功能 : Draw a Pixel
**輸入 : posX, posY, color
**輸出 : None
**使用 : OLED_DrawPixel(posX, posY, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawPixel( uint8_t posX, uint8_t posY, uint16_t color )
{
  OLED_SetWindow(posX, posY, posX, posY);
  OLED_WriteColor(color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawLineX
**功能 : Draw X-Axis Line
**輸入 : posX, posY, length, color
**輸出 : None
**使用 : OLED_DrawLineX(posX, posY, length, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawLineX( uint8_t posX, uint8_t posY, uint8_t length, uint16_t color )
{
  OLED_SetWindow(posX, posY, posX + length - 1, posY);

  while(length--)
    OLED_WriteColor(color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawLineY
**功能 : Draw Y-Axis Line
**輸入 : posX, posY, length, color
**輸出 : None
**使用 : OLED_DrawLineY(posX, posY, length, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawLineY( uint8_t posX, uint8_t posY, uint8_t length, uint16_t color )
{
  OLED_SetWindow(posX, posY, posX, posY + length - 1);

  while(length--)
    OLED_WriteColor(color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawLine
**功能 : Draw Line
**輸入 : posX1, posY1, posX2, posY2, color
**輸出 : None
**使用 : OLED_DrawLine(posX1, posY1, posX2, posY2, color);
**====================================================================================================*/
/*====================================================================================================*/
//void OLED_DrawLine( uint8_t posX1, uint8_t posY1, uint8_t posX2, uint8_t posY2, uint16_t color )
//{
//}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawRect
**功能 : Draw Rectangle
**輸入 : posX, posY, width, height, color
**輸出 : None
**使用 : OLED_DrawRect(posX, posY, width, height, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawRect( uint8_t posX, uint8_t posY, uint8_t width, uint8_t height, uint16_t color )
{
  OLED_DrawLineX(posX,             posY,              width,  color);
  OLED_DrawLineX(posX,             posY + height - 1, width,  color);
  OLED_DrawLineY(posX,             posY,              height, color);
  OLED_DrawLineY(posX + width - 1, posY,              height, color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawRectFill
**功能 : Draw Rectangle
**輸入 : posX, posY, Length, width, height, color
**輸出 : None
**使用 : OLED_DrawRectFill(posX, posY, width, height, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawRectFill( uint8_t posX, uint8_t posY, uint8_t width, uint8_t height, uint16_t color )
{
  uint32_t point = width * height;

  OLED_SetWindow(posX, posY, posX + width - 1, posY + height - 1);

  while(point--)
    OLED_WriteColor(color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawCircle
**功能 : Draw Circle
**輸入 : posX, posY, radius, color
**輸出 : None
**使用 : OLED_DrawCircle(posX, posY, radius, color);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawCircle( uint8_t posX, uint8_t posY, uint8_t radius, uint16_t color )
{
  int32_t D;
  uint32_t curX;
  uint32_t curY;

  D = 3 - (radius << 1);
  curX = 0;
  curY = radius;

  while(curX <= curY) {
    OLED_DrawPixel(posX + curX, posY - curY, color);
    OLED_DrawPixel(posX - curX, posY - curY, color);
    OLED_DrawPixel(posX + curY, posY - curX, color);
    OLED_DrawPixel(posX - curY, posY - curX, color);
    OLED_DrawPixel(posX + curX, posY + curY, color);
    OLED_DrawPixel(posX - curX, posY + curY, color);
    OLED_DrawPixel(posX + curY, posY + curX, color);
    OLED_DrawPixel(posX - curY, posY + curX, color);

    if(D < 0) {
      D += (curX << 2) + 6;
    }
    else {
      D += ((curX - curY) << 2) + 10;
      curY--;
    }
    curX++;
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_DrawPic
**功能 : Draw Picture
**輸入 : posX, posY, width, height, *pPic
**輸出 : None
**使用 : OLED_DrawPic(posX, posY, width, height, pic);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_DrawPic( uint8_t posX, uint8_t posY, uint8_t width, uint8_t height, const uint16_t *pic )
{
  uint32_t point = height * width;

  OLED_SetWindow(posX, posY, posX + width - 1, posY + height - 1);

  for(uint32_t i = 0; i < point; i++)
    OLED_WriteColor(pic[i]);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_PutChar
**功能 : Put Char
**輸入 : posX, posY, *pFont, word, fontColor, backColor
**輸出 : None
**使用 : OLED_PutChar(posX, posY, pFont, word, WHITE, BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_PutChar( uint8_t posX, uint8_t posY, sFONT *pFont, uint8_t word, uint16_t fontColor, uint16_t backColor )
{
  uint8_t tmp = 0;
  uint8_t font_h = pFont->height;
  uint8_t font_w = pFont->width;
  uint16_t point = 0;

  if(pFont->type == dirH) {
    point = (word - ' ') * font_h;
    for(uint8_t i = 0; i < font_h; i++) {
      tmp = pFont->table[point + i];
      for(uint8_t j = 0; j < font_w; j++) {
        if((tmp << j) & 0x80)
          OLED_DrawPixel(posX + j, posY + i, fontColor);
        else
          OLED_DrawPixel(posX + j, posY + i, backColor);
      }
    }
  }
  else {
    point = (word - ' ') * font_w;
    for(uint8_t i = 0; i < font_w; i++) {
      tmp = pFont->table[point + i];
      for(uint8_t j = 0; j < font_h; j++) {
        if((tmp >> j) & 0x01)
          OLED_DrawPixel(posX + i, posY + j, fontColor);
        else
          OLED_DrawPixel(posX + i, posY + j, backColor);
      }
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_PutStr
**功能 : Put String
**輸入 : posX, posY, pString, fontColor, backColor
**輸出 : None
**使用 : OLED_PutStr(10, 10, FONT8, "OLED TEST ... ", WHITE, BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_PutStr( uint8_t posX, uint8_t posY, sFONT *pFont, char *pString, uint16_t fontColor, uint16_t backColor )
{
  uint16_t offsetX = 0;

  while(*pString) {
    OLED_PutChar(posX + offsetX, posY, pFont, *pString, WHITE, BLACK);
    pString++;
    offsetX += pFont->width;
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_PutNum
**功能 : Put Number
**輸入 : posX, posY, *pFont, type, lens, number, fontColor, backColor
**輸出 : None
**使用 : OLED_PutNum(posX, posY, pFont, Type_D, lens, number, WHITE, BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void OLED_PutNum( uint8_t posX, uint8_t posY, sFONT *pFont, StringType type, uint8_t lens, int32_t number, uint16_t fontColor, uint16_t backColor )
{
  char strBuf[16] = {0};

  num2Str(type, lens, strBuf, number);
  OLED_PutStr(posX, posY, pFont, strBuf, fontColor, backColor);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : OLED_TestColoBar
**功能 : Draw Color Bar
**輸入 : None
**輸出 : None
**使用 : OLED_TestColoBar();
**====================================================================================================*/
/*====================================================================================================*/
#define COLOR_NUMBER      (16)
#define COLOR_BAR_HEIGHT  (OLED_W / COLOR_NUMBER)  // 96/16 = 6

void OLED_TestColoBar( void )
{
  const uint16_t drawColor[COLOR_NUMBER] = {
    WHITE,
    RED,
    GREEN,
    BLUE,
    MAGENTA,
    GRED,
    GBLUE,
    BRED,
    BROWN,
    BRRED,
    CYAN,
    GRAY,
    YELLOW,
    DARKBLUE,
    LIGHTBLUE,
    GRAYBLUE
  };

  OLED_SetWindow(0, 0, OLED_W - 1, OLED_H - 1);

  for(uint32_t i = 0; i < COLOR_NUMBER; i++) {
    uint32_t j = 4 * OLED_W;
    while(j--)
      OLED_WriteColor(drawColor[i]);
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
