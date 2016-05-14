/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "algorithms\algorithm_string.h"

#include "module_ili9341.h"
#include "module_fontlib.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define LCD_SPIx                    SPI2
#define LCD_SPIx_CLK_ENABLE()       RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE)

#define LCD_LIG_PIN                 GPIO_Pin_2
#define LCD_LIG_GPIO_PORT           GPIOA

//#define BLIGHT_PWM
#ifdef BLIGHT_PWM
  #define LCD_LIG_TIMx              TIM2
  #define LCD_LIG_TIMx_CHANNEL      TIM_CHANNEL_3
  #define LCD_TIMx_CLK_ENABLE()     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE)
  #define LCD_LIG_AF                GPIO_AF1_TIM2
  #define LCD_LIG_SOURCE            GPIO_PinSource2
  #define LCD_LIG                   LCD_LIG_TIMx->CCR3
  #define BLIGHT_MAX                255
  #define BLIGHT_MIN                0
  #define BLIGHT_DEFAULT            200
#else
  #define LCD_LIG_H()               __GPIO_SET(LCD_LIG_GPIO_PORT, LCD_LIG_PIN)
  #define LCD_LIG_L()               __GPIO_RST(LCD_LIG_GPIO_PORT, LCD_LIG_PIN)
  #define BLIGHT_MAX                1
  #define BLIGHT_MIN                0
  #define BLIGHT_DEFAULT            1
#endif

#define LCD_DC_PIN                  GPIO_Pin_1
#define LCD_DC_GPIO_PORT            GPIOB
#define LCD_DC_H()                  __GPIO_SET(LCD_DC_GPIO_PORT, LCD_DC_PIN)
#define LCD_DC_L()                  __GPIO_RST(LCD_DC_GPIO_PORT, LCD_DC_PIN)

#define LCD_RST_PIN                 GPIO_Pin_3
#define LCD_RST_GPIO_PORT           GPIOA
#define LCD_RST_H()                 __GPIO_SET(LCD_RST_GPIO_PORT, LCD_RST_PIN)
#define LCD_RST_L()                 __GPIO_RST(LCD_RST_GPIO_PORT, LCD_RST_PIN)

#define LCD_CST_PIN                 GPIO_Pin_12
#define LCD_CST_GPIO_PORT           GPIOB
#define LCD_CST_H()                 __GPIO_SET(LCD_CST_GPIO_PORT, LCD_CST_PIN)
#define LCD_CST_L()                 __GPIO_RST(LCD_CST_GPIO_PORT, LCD_CST_PIN)

#define LCD_SCK_PIN                 GPIO_Pin_13
#define LCD_SCK_GPIO_PORT           GPIOB
#define LCD_SCK_AF                  GPIO_AF_SPI2
#define LCD_SCK_SOURCE              GPIO_PinSource13

#define LCD_SDO_PIN                 GPIO_Pin_14
#define LCD_SDO_GPIO_PORT           GPIOB
#define LCD_SDO_AF                  GPIO_AF_SPI2
#define LCD_SDO_SOURCE              GPIO_PinSource14

#define LCD_SDI_PIN                 GPIO_Pin_15
#define LCD_SDI_GPIO_PORT           GPIOB
#define LCD_SDI_AF                  GPIO_AF_SPI2
#define LCD_SDI_SOURCE              GPIO_PinSource15
/*====================================================================================================*/
/*====================================================================================================*
**函數 : ILI9341_Config
**功能 : ILI9341 Config
**輸入 : None
**輸出 : None
**使用 : ILI9341_Config();
**====================================================================================================*/
/*====================================================================================================*/
void ILI9341_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;

  /* SPI Clk ******************************************************************/
  LCD_SPIx_CLK_ENABLE();

  /* SPI AF *******************************************************************/
  GPIO_PinAFConfig(LCD_SCK_GPIO_PORT, LCD_SCK_SOURCE, LCD_SCK_AF);
  GPIO_PinAFConfig(LCD_SDO_GPIO_PORT, LCD_SDO_SOURCE, LCD_SDO_AF);    
  GPIO_PinAFConfig(LCD_SDI_GPIO_PORT, LCD_SDI_SOURCE, LCD_SDI_AF);

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = LCD_DC_PIN;
  GPIO_Init(LCD_DC_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = LCD_RST_PIN;
  GPIO_Init(LCD_RST_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = LCD_CST_PIN;
  GPIO_Init(LCD_CST_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = LCD_SCK_PIN;
  GPIO_Init(LCD_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = LCD_SDO_PIN;
  GPIO_Init(LCD_SDO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = LCD_SDI_PIN;
  GPIO_Init(LCD_SDI_GPIO_PORT, &GPIO_InitStruct);

  LCD_CST_H();  // LOW ENABLE

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
  SPI_Init(LCD_SPIx, &SPI_InitStruct);

  SPI_Cmd(LCD_SPIx, ENABLE);

  ILI9341_LigConfig();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : ILI9341_LigConfig
**功能 : ILI9341 Light Config
**輸入 : None
**輸出 : None
**使用 : ILI9341_LigConfig();
**====================================================================================================*/
/*====================================================================================================*/
static void ILI9341_LigConfig( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

#ifdef BLIGHT_PWM
  TIM_HandleTypeDef TIM_InitStruct;
  TIM_OC_InitTypeDef TIM_OC_InitStruct;

  /* PWM Clk Init *************************************************************/
  LCD_TIMx_CLK_ENABLE();

  /* LIG GPIO  */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Alternate = LCD_LIG_AF;
  GPIO_InitStruct.Pin = LCD_LIG_PIN;
  HAL_GPIO_Init(LCD_LIG_GPIO_PORT, &GPIO_InitStruct);

  /************************** PWM Output **************************************/
  /* TIMx Time Base */
  TIM_InitStruct.Instance               = LCD_LIG_TIMx;
  TIM_InitStruct.Init.Prescaler         = 90 - 1;         // 90 MHz / 72 = 1 MHz
  TIM_InitStruct.Init.Period            = BLIGHT_MAX - 1; // PWM Freq = 10 kHz
  TIM_InitStruct.Init.ClockDivision     = 0;
  TIM_InitStruct.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TIM_InitStruct.Init.RepetitionCounter = 0;
  HAL_TIM_PWM_Init(&TIM_InitStruct);

  /* TIMx OC2 */
  TIM_OC_InitStruct.OCMode      = TIM_OCMODE_PWM2;
  TIM_OC_InitStruct.OCFastMode  = TIM_OCFAST_DISABLE;
  TIM_OC_InitStruct.OCPolarity  = TIM_OCPOLARITY_LOW;
  TIM_OC_InitStruct.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  TIM_OC_InitStruct.OCIdleState = TIM_OCIDLESTATE_SET;
  TIM_OC_InitStruct.OCNIdleState= TIM_OCNIDLESTATE_RESET;

  /* Enable */
  TIM_OC_InitStruct.Pulse = BLIGHT_MIN;
  HAL_TIM_PWM_ConfigChannel(&TIM_InitStruct, &TIM_OC_InitStruct, LCD_LIG_TIMx_CHANNEL);
  HAL_TIM_PWM_Start(&TIM_InitStruct, LCD_LIG_TIMx_CHANNEL);

#else
  /* LIG Pin *******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = LCD_LIG_PIN;
  GPIO_Init(LCD_LIG_GPIO_PORT, &GPIO_InitStruct);

  LCD_LIG_L();

#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_WriteCmd
**功能 : Write Command or Address
**輸入 : writeCmd
**輸出 : None
**使用 : LCD_WriteCmd(0xCB);
**====================================================================================================*/
/*====================================================================================================*/
static void LCD_WriteCmd( uint8_t writeCmd )
{
  LCD_CST_L();
  LCD_DC_L();
  SPI_RW(LCD_SPIx, writeCmd);
  LCD_CST_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_WriteData
**功能 : Write Data
**輸入 : writeData
**輸出 : None
**使用 : LCD_WriteData(Byte8H(Color));
**====================================================================================================*/
/*====================================================================================================*/
static void LCD_WriteData( uint16_t writeData )
{
  LCD_CST_L();
  LCD_DC_H();
  SPI_RW(LCD_SPIx, writeData);
  LCD_CST_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_WriteColor
**功能 : Write Color
**輸入 : color
**輸出 : None
**使用 : LCD_WriteColor(BLACK);
**====================================================================================================*/
/*====================================================================================================*/
static void LCD_WriteColor( uint16_t color )
{
  LCD_CST_L();
  LCD_DC_H();
  SPI_RW(LCD_SPIx, Byte8H(color));
  SPI_RW(LCD_SPIx, Byte8L(color));
  LCD_CST_H();
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : ILI9341_Init
**功能 : ILI9341 Init
**輸入 : None
**輸出 : None
**使用 : ILI9341_Init();
**====================================================================================================*/
/*====================================================================================================*/
#include "kdwm1000.h"
void ILI9341_Init( void )
{
  // reset
  LCD_RST_L();
  delay_ms(100);
  LCD_RST_H();
  delay_ms(50);

  LCD_WriteCmd(0xCB);
  LCD_WriteData(0x39);
  LCD_WriteData(0x2C);
  LCD_WriteData(0x00);
  LCD_WriteData(0x34);
  LCD_WriteData(0x02);

  LCD_WriteCmd(0xCF);
  LCD_WriteData(0x00);
  LCD_WriteData(0XC1);
  LCD_WriteData(0X30);

  LCD_WriteCmd(0xE8);
  LCD_WriteData(0x85);
  LCD_WriteData(0x00);
  LCD_WriteData(0x78);

  LCD_WriteCmd(0xEA);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);

  LCD_WriteCmd(0xED);
  LCD_WriteData(0x64);
  LCD_WriteData(0x03);
  LCD_WriteData(0X12);
  LCD_WriteData(0X81);

  LCD_WriteCmd(0xF7);
  LCD_WriteData(0x20);

  LCD_WriteCmd(0xC0);   // Power control
  LCD_WriteData(0x23);  // VRH[5:0]

  LCD_WriteCmd(0xC1);   // Power control
  LCD_WriteData(0x10);  // SAP[2:0];BT[3:0]

  LCD_WriteCmd(0xC5);   // VCM control
  LCD_WriteData(0x3e);  // 
  LCD_WriteData(0x28); 

  LCD_WriteCmd(0xC7);   // VCM control2
  LCD_WriteData(0x86);  // --

  LCD_WriteCmd(0x36);   // Memory Access Control
#ifdef H_VIEW
  LCD_WriteData(0x48);  // H
#else
  LCD_WriteData(0xE8);  // V
#endif

  LCD_WriteCmd(0x3A);
  LCD_WriteData(0x55);

  LCD_WriteCmd(0xB1);
  LCD_WriteData(0x00);
  LCD_WriteData(0x18);

  LCD_WriteCmd(0xB6);   // Display Function Control
  LCD_WriteData(0x08);
  LCD_WriteData(0x82);
  LCD_WriteData(0x27);

  LCD_WriteCmd(0xF2);   // 3Gamma Function Disable
  LCD_WriteData(0x00);

  LCD_WriteCmd(0x26);   // Gamma curve selected
  LCD_WriteData(0x01);

  LCD_WriteCmd(0xE0);   // Set Gamma
  LCD_WriteData(0x0F);
  LCD_WriteData(0x31);
  LCD_WriteData(0x2B);
  LCD_WriteData(0x0C);
  LCD_WriteData(0x0E);
  LCD_WriteData(0x08);
  LCD_WriteData(0x4E);
  LCD_WriteData(0xF1);
  LCD_WriteData(0x37);
  LCD_WriteData(0x07);
  LCD_WriteData(0x10);
  LCD_WriteData(0x03);
  LCD_WriteData(0x0E);
  LCD_WriteData(0x09);
  LCD_WriteData(0x00);

  LCD_WriteCmd(0XE1);   // Set Gamma
  LCD_WriteData(0x00);
  LCD_WriteData(0x0E);
  LCD_WriteData(0x14);
  LCD_WriteData(0x03);
  LCD_WriteData(0x11);
  LCD_WriteData(0x07);
  LCD_WriteData(0x31);
  LCD_WriteData(0xC1);
  LCD_WriteData(0x48);
  LCD_WriteData(0x08);
  LCD_WriteData(0x0F);
  LCD_WriteData(0x0C);
  LCD_WriteData(0x31);
  LCD_WriteData(0x36);
  LCD_WriteData(0x0F);

  LCD_WriteCmd(0x11);   // Exit Sleep
  delay_ms(120);

  LCD_WriteCmd(0x29);   // Display on
  LCD_WriteCmd(0x2C);

  LCD_Clear(BLACK);
  LCD_SetBackLight(BLIGHT_DEFAULT);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_Clear
**功能 : Clear Windows
**輸入 : color
**輸出 : None
**使用 : LCD_Clear(BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_Clear( uint16_t color )
{
  uint32_t point = LCD_W * LCD_H;

  LCD_SetWindow(0, 0, LCD_W - 1, LCD_H - 1);

  while(point--)
    LCD_WriteColor(color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_SetCursor
**功能 : Set Cursor
**輸入 : coordiX, coordiY
**輸出 : None
**使用 : LCD_SetCursor(StartX, StartY);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_SetCursor( uint16_t coordiX, uint16_t coordiY )
{
  LCD_WriteCmd(0x2A);
  LCD_WriteData(Byte8H(coordiX));
  LCD_WriteData(Byte8L(coordiX));
  LCD_WriteCmd(0x2B);
  LCD_WriteData(Byte8H(coordiY));
  LCD_WriteData(Byte8L(coordiY));
  LCD_WriteCmd(0x2C);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_SetWindow
**功能 : Set Window
**輸入 : startX, startY, endX, endY
**輸出 : None
**使用 : LCD_SetWindow(X0, Y0, X1, Y1);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_SetWindow( uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY )
{
  LCD_WriteCmd(0x2A);
  LCD_WriteData(Byte8H(startX));
  LCD_WriteData(Byte8L(startX));
  LCD_WriteData(Byte8H(endX));
  LCD_WriteData(Byte8L(endX));
  LCD_WriteCmd(0x2B);
  LCD_WriteData(Byte8H(startY));
  LCD_WriteData(Byte8L(startY));
  LCD_WriteData(Byte8H(endY));
  LCD_WriteData(Byte8L(endY));
  LCD_WriteCmd(0x2C);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_SetBackLight
**功能 : Set LCD BackLight
**輸入 : backLight
**輸出 : None
**使用 : LCD_SetBackLight(BLIGHT_DEFAULT);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_SetBackLight( int16_t backLight )
{
#ifdef BLIGHT_PWM
  if(backLight <= BLIGHT_MIN)       LCD_LIG = BLIGHT_MIN;
  else if(backLight >= BLIGHT_MAX)  LCD_LIG = BLIGHT_MAX;
  else                              LCD_LIG = backLight;

#else
  if(backLight == 0)  LCD_LIG_L();
  else                LCD_LIG_H();

#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawPixel
**功能 : Draw a Pixel
**輸入 : coordiX, coordiY, color
**輸出 : None
**使用 : LCD_DrawPixel(coordiX, coordiY, color);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawPixel( uint16_t coordiX, uint16_t coordiY, uint16_t color )
{
  LCD_SetCursor(coordiX, coordiY);
  LCD_WriteColor(color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawLineX
**功能 : Draw X-Axis Line
**輸入 : coordiX, coordiY, length, color
**輸出 : None
**使用 : LCD_DrawLine(coordiX, coordiY, length, color)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawLineX( uint16_t coordiX, uint16_t coordiY, uint16_t length, uint16_t color )
{
  for(uint16_t i = 0; i < length; i++)
    LCD_DrawPixel(coordiX + i, coordiY, color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawLineY
**功能 : Draw Y-Axis Line
**輸入 : coordiX, coordiY, length, color
**輸出 : None
**使用 : LCD_DrawLine(coordiX, coordiY, length, color)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawLineY( uint16_t coordiX, uint16_t coordiY, uint16_t length, uint16_t color )
{
  for(uint16_t i = 0; i < length; i++)
    LCD_DrawPixel(coordiX, coordiY + i, color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawRectangle
**功能 : Draw Rectangle
**輸入 : coordiX, coordiY, width, height, color
**輸出 : None
**使用 : LCD_DrawRectangle(coordiX, coordiY, width, height, color)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawRectangle( uint16_t coordiX, uint16_t coordiY, uint16_t width, uint16_t height, uint16_t color )
{
  uint16_t i;

  for(i = 0; i <= height; i++) {
    LCD_DrawPixel(coordiX + i, coordiY, color);
    LCD_DrawPixel(coordiX + i, coordiY + width, color);
  }
  for(i = 1; i < width; i++) {
    LCD_DrawPixel(coordiX, coordiY + i, color);
    LCD_DrawPixel(coordiX + height, coordiY + i, color);
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawRectangleFill
**功能 : Draw Rectangle
**輸入 : coordiX, coordiY, width, height, color
**輸出 : None
**使用 : LCD_DrawRectangleFill(coordiX, coordiY, width, height, color)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawRectangleFill( uint16_t coordiX, uint16_t coordiY, uint16_t width, uint16_t height, uint16_t color )
{
  uint16_t i, j;

  for(i = 0; i < width; i++)
    for(j = 0; j < height; j++)
      LCD_DrawPixel(coordiX + j, coordiY + i, color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawCircle
**功能 : Draw Circle
**輸入 : coordiX, coordiY, radius, color
**輸出 : None
**使用 : LCD_DrawCircle(coordiX, coordiY, radius, color)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawCircle( uint16_t coordiX, uint16_t coordiY, uint16_t radius, uint16_t color )
{
  int a, b;
  int di;

  a = 0;
  b = radius;
  di = 3 - (radius << 1);

  while(a <= b) {
    LCD_DrawPixel(coordiX-b, coordiY-a, color); //3
    LCD_DrawPixel(coordiX+b, coordiY-a, color); //0
    LCD_DrawPixel(coordiX-a, coordiY+b, color); //1
    LCD_DrawPixel(coordiX-b, coordiY-a, color); //7
    LCD_DrawPixel(coordiX-a, coordiY-b, color); //2
    LCD_DrawPixel(coordiX+b, coordiY+a, color); //4
    LCD_DrawPixel(coordiX+a, coordiY-b, color); //5
    LCD_DrawPixel(coordiX+a, coordiY+b, color); //6
    LCD_DrawPixel(coordiX-b, coordiY+a, color);
    a++;
    // Bresenham
    if(di < 0)
      di += 4 * a + 6;
    else {
      di += 10 + 4* (a - b);
      b--;
    }
    LCD_DrawPixel(coordiX+a, coordiY+b, color);
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawPicture
**功能 : Draw Picture
**輸入 : coordiX, coordiY, width, height, pic
**輸出 : None
**使用 : LCD_DrawPicture(coordiX, coordiY, width, height, pic);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawPicture( uint16_t coordiX, uint16_t coordiY, uint16_t width, uint16_t height, const uint8_t *pic )
{
  uint16_t *readPixel = (uint16_t*)pic;
  uint32_t i = 0, j = height * width;

  LCD_SetWindow(coordiX, coordiY, coordiX + width - 1, coordiY + height - 1);

  for(i = 0; i < j; i++)
    LCD_WriteColor(readPixel[i]);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_PutChar
**功能 : Put Char
**輸入 : coordiX, coordiY, chWord, fontColor, backColor
**輸出 : None
**使用 : LCD_PutChar(x, y, 'C', WHITE, BLACK)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_PutChar( uint16_t coordiX, uint16_t coordiY, char *chWord, uint16_t fontColor, uint16_t backColor )
{
  uint8_t tmpChar = 0;
  uint16_t i = 0, j = 0;

  for(i = 0; i < 16; i++) {
    tmpChar = ASCII_16x8[*chWord - 32][i];
    for(j = 0; j < 8; j++) {
      if(((tmpChar >> (7 - j)) & 0x01) == 0x01)
        LCD_DrawPixel(coordiX + j, coordiY + i, fontColor);
      else
        LCD_DrawPixel(coordiX + j, coordiY + i, backColor);
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_PutStr
**功能 : Put String
**輸入 : coordiX, coordiY, chWord, fontColor, backColor
**輸出 : None
**使用 : LCD_PutStr(10, 10, "TFT LCD TEST ... ", WHITE, BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_PutStr( uint16_t coordiX, uint16_t coordiY, char *chWord, uint16_t fontColor, uint16_t backColor )
{
  uint16_t offsetX = 0;

  while(*chWord) {
    LCD_PutChar(coordiX + offsetX, coordiY, chWord, fontColor, backColor);
    chWord++;
    offsetX += 8;
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_PutNum
**功能 : Put Number
**輸入 : oordiX, coordiY, type, length, numData, fontColor, backColor
**輸出 : None
**使用 : LCD_PutNum(coordiX, coordiY, Type_D, length, number, WHITE, BLACK)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_PutNum( uint16_t coordiX, uint16_t coordiY, StringType type, uint8_t length, uint32_t number, uint16_t fontColor, uint16_t backColor )
{
  char tmpStr[16] = {0};

  num2Str(type, length, tmpStr, number);
  LCD_PutStr(coordiX, coordiY, tmpStr, fontColor, backColor);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_TestColoBar
**功能 : Draw Color Bar
**輸入 : None
**輸出 : None
**使用 : LCD_TestColoBar();
**====================================================================================================*/
/*====================================================================================================*/
#define COLOR_TYPE 20
void LCD_TestColoBar( void )
{
  uint32_t i = 0, j = 0;
  uint16_t drawColor[COLOR_TYPE] = {
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
    GRAYBLUE,
    LIGHTGREEN,
    LGRAY,
    LGRAYBLUE,
    LBBLUE
  };

  LCD_SetWindow(0, 0, LCD_W - 1, LCD_H - 1);

  for(i = 0; i < 20; i++) {
    j = (LCD_H / 20) * LCD_W;
    while(j--)
      LCD_WriteColor(drawColor[i]);
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
