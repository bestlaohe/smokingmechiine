/*
 * Screen_api.c
 *
 *  Created on: 2024��8��30��
 *      Author: 12630
 */

#include "Screen_api.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void startup_animation()
{
    for (int s = 0, o; s <= 80; s += 2)
    {
        o = s / 2;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                Paint_DrawChar(
                    i * (LCD_WIDTH - 48 - o) + (1 - i) * o,
                    j * (LCD_HEIGHT - 48 - o) + (1 - j) * o,
                    0, &Font24_logo, BLACK, WHITE - s * 819, 0);
    }
    Screen_Clear(0, 0, 127, 127, BLACK);
    Paint_DrawChar(40, 40, 0, &Font24_logo, BLACK, WHITE, 0);
    Delay_Ms(500);
    Screen_Clear(0, 0, 127, 127, BLACK);
}

Screen_ATTRIBUTES LCD;
uint8_t lcd_gram[Y_MAX_PIXEL * X_MAX_PIXEL * 2] = {0}; ///< ����һ���ڴ�ռ䵱�Դ�ʹ��
u8 dmaXoffset, dmaYoffset = 0;
void LCD_SHOW_API_INIT()
{

    Screen_Init(VERTICAL);
    //    Delay_Ms(300);
    // DEBUG_PRINT("Set Clear and Display Funtion\r\n");
     Screen_Clear(0, 0, 128, 128, MY_THEME_BACK_COLOR);
     
    //  DEBUG_PRINT("Set Clear and Display Funtion\r\n");
    Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, ROTATE_0, WHITE);


   
    //  DEBUG_PRINT("Set Clear and Display Funtion\r\n");

    //    Screen_Init(VERTICAL);
    //      Screen_Clear(BLACK);
    //
    //      DEBUG_PRINT("Paint_NewImage\r\n");
    //      Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, ROTATE_180, WHITE);
    //
    //      DEBUG_PRINT("Set Clear and Display Funtion\r\n");
    //      //Paint_SetClearFuntion(Screen_Clear);
    //     // Paint_SetDisplayFuntion(Screen_DrawPaint);
    //
    //      DEBUG_PRINT("Paint_Clear\r\n");
    //      //Paint_Clear(BLACK);
    //
    //      DEBUG_PRINT("drawing...\r\n");
    //      Paint_SetRotate(0);

    //  DEBUG_PRINT("Set Clear and Display Funtion\r\n");
    // Paint_SetClearFuntion(Screen_Clear);
    // Paint_SetDisplayFuntion(Screen_DrawPaint);

    //  DEBUG_PRINT("Screen_Clear\r\n");

    // DEBUG_PRINT("drawing...\r\n");
    //  Paint_SetRotate(0);

    //       Paint_DrawString(5, 10, "123",        &Font24,  YELLOW, RED);
    //       Paint_DrawString(5, 34, "ABC",        &Font24,  BLUE,   CYAN);

    //  Paint_DrawFloatNum (5, 58 ,987.654321,3, &Font12,  WHITE,  BLACK);
    //  Paint_DrawString_CN(0,80, "΢ѩ����",   &Font24CN,WHITE,  RED);
    //     Paint_DrawImage(set,30,3,48,48);
    //    Delay_Ms(3000);
    //    Screen_Clear(WHITE);
    //
    ////  Paint_DrawRectangle(0, 10, 225, 58, RED     ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    ////  Paint_DrawLine  (0, 10, 225, 58,    MAGENTA ,DOT_PIXEL_2X2,LINE_STYLE_SOLID);
    ////  Paint_DrawLine  (0, 10, 125, 58,    MAGENTA ,DOT_PIXEL_2X2,LINE_STYLE_SOLID);
    //    Paint_DrawCircle(35,50,  25,        BLUE    ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    //    Paint_DrawCircle(65,50,  25,        BLACK   ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    //    Paint_DrawCircle(95,50,  25,        RED     ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    //    Paint_DrawCircle(50,75,  25,        YELLOW  ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    //    Paint_DrawCircle(80,75,  25,        GREEN   ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    //    Delay_Ms(3000);
    //
    //    DEBUG_PRINT("quit...\r\n");
}

/*****************************************************************************
 * | File        :   screen.c
 * | Author      :   Waveshare team
 * | Function    :   Hardware underlying interface
 * | Info        :
 *                Used to shield the underlying layers of each master
 *                and enhance portability
 *----------------
 * | This version:   V1.0
 * | Date        :   2018-01-11
 * | Info        :   Basic version
 *
 ******************************************************************************/

// Screen_ATTRIBUTES LCD;

/******************************************************************************
function :  send command
parameter:
     Reg : Command register
******************************************************************************/
void Screen_SendCommand(UBYTE Reg)
{
    LCD_DC_0;
    LCD_CS_ENABLE;
    Screen_spi_write(Reg);
    LCD_CS_DISABLE;
}

/******************************************************************************
function :  send data
parameter:
    Data : Write data
******************************************************************************/
static void Screen_SendData_8Bit(UBYTE Data)
{

    LCD_DC_1;
    LCD_CS_ENABLE;
    Screen_spi_write(Data);
    LCD_CS_DISABLE;
}

/******************************************************************************
function :  send data
parameter:
    Data : Write data
******************************************************************************/
#if !USE_DMA
static void Screen_SendData_16Bit(UWORD Data)
{
    LCD_DC_1;
    LCD_CS_ENABLE;
    Screen_spi_write((Data >> 8) & 0xFF);
    Screen_spi_write(Data & 0xFF);
    LCD_CS_DISABLE;
}
#endif
int LCD_Drive_Init(void)
{
    SPI_FullDuplex_Init();
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

#if USE_DMA
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE); // ����SPI��DMA��������
    DMA_Cmd(DMA1_Channel3, ENABLE);                  // ����DMAͨ��
    NVIC_EnableIRQ(DMA1_Channel3_IRQn);              // ����DMA�ж�
#endif
    Screen_ExitSleepMode();
    return 0;
}
int LCD_Drive_DeInit(void)
{
    Screen_EnterSleepMode();
    // ���� PWM ���
    TIM_CtrlPWMOutputs(TIM1, DISABLE);
    // ���� DMA ͨ��
#if USE_DMA
    DMA_Cmd(DMA1_Channel3, DISABLE);                  // ����DMAͨ��
    NVIC_DisableIRQ(DMA1_Channel3_IRQn);              // ����DMA�ж�
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE); // ����SPI��DMA��������
#endif
    // �ر� SPI
    SPI_Cmd(SPI1, DISABLE);                               // ����SPI����
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE); // ���� SPI1 ʱ��

    return 0;
}
/******************************************************************************
function :  Initialize the lcd register
parameter:
******************************************************************************/

// ��������洢��ʼ�����������
static const uint8_t init_cmds[] = {
    0xB0, 0xC0,                                                                               // ���õ�Դ���ƼĴ��������õ�Դ��ز���
    0xB2, 0x2F,                                                                               // ����֡�ʿ��ƼĴ���������֡�ʻ�ˢ����
    0xB3, 0x03,                                                                               // ������ʾģʽ��ʱ����ƼĴ�����������ʾģʽ
    0xB6, 0x19,                                                                               // ������ʾ�����ɨ��ģʽ��������ʾ����
    0xB7, 0x01,                                                                               // ������ʾ���ƼĴ�����������ر�ĳЩ��ʾ����
    0xAC, 0xCB,                                                                               // ���õ�Դ���ƻ��ѹ���ڼĴ�����������Դ���ѹ����
    0xAB, 0x0E,                                                                               // ������ʾ���Ȼ�Աȶȿ��ƼĴ������������Ȼ�Աȶ�
    0xB4, 0x04,                                                                               // ������ʾ���ƼĴ�����������ʾ���Ʋ���
    0xA8, 0x19,                                                                               // ������ʾģʽ��ʱ����ƼĴ�����������ʾģʽ
    0x3A, 0x05,                                                                               // �������ظ�ʽ������ RGB 565 �� RGB 666
    0xB8, 0x08,                                                                               // ������ʾ���ƼĴ�����������ʾ���Ʋ���
    0xE8, 0x24,                                                                               // ���õ�Դ���ƻ��ѹ���ڼĴ�����������Դ���ѹ����
    0xE9, 0x48,                                                                               // ���õ�Դ���ƻ��ѹ���ڼĴ�����������Դ���ѹ����
    0xEA, 0x22,                                                                               // ���õ�Դ���ƻ��ѹ���ڼĴ�����������Դ���ѹ����
    0xC6, 0x30,                                                                               // ������ʾ���ƼĴ�����������ʾ���Ʋ���
    0xC7, 0x18,                                                                               // ������ʾ���ƼĴ�����������ʾ���Ʋ���
    0xF0, 0x1F, 0x28, 0x04, 0x3E, 0x2A, 0x2E, 0x20, 0x00, 0x0C, 0x06, 0x00, 0x1C, 0x1F, 0x0F, // ���� Gamma У���Ĵ��������� Gamma ����
    0xF1, 0x00, 0x2D, 0x2F, 0x3C, 0x6F, 0x1C, 0x0B, 0x00, 0x00, 0x00, 0x07, 0x0D, 0x11, 0x0F, // ���� Gamma У���Ĵ��������� Gamma ����
    0x21,                                                                                     // ������ʾ��ת���ܣ�Display Inversion ON��
    0x11,                                                                                     // �˳�˯��ģʽ��Sleep Out����ʹ LCD ������������״̬
    0x29                                                                                      // ������ʾ��Display ON����ʹ��Ļ��ʼ��ʾͼ��
};

static void Screen_InitReg(void)
{

    for (int8_t i = 0; i < sizeof(init_cmds);)
    {
        uint8_t cmd = init_cmds[i++];
        Screen_SendCommand(cmd);

        // �����޲�������
        if (cmd == 0x21 || cmd == 0x11 || cmd == 0x29)
            continue;

        // ��������(1����14���ֽ�)
        uint8_t len = (cmd == 0xF0 || cmd == 0xF1) ? 14 : 1;
        while (len--)
            Screen_SendData_8Bit(init_cmds[i++]);
    }
}

void Screen_EnterSleepMode(void)
{
    // ����˯��ģʽ���� (0x10)
    Screen_SendCommand(0x10);

    // ���������ֲᣬ������Ҫ�ȴ�һ��ʱ�䣨���� 5ms����ȷ��������Ч
    // ����ȴ�ʱ����ο� LCD �������������ֲ�
    Delay_Ms(5); // ʹ�� HAL �����ʱ��������ʱ 5 ����
}
void Screen_ExitSleepMode(void)
{
    // �����˳�˯��ģʽ���� (0x11)
    Screen_SendCommand(0x11);

    // ���������ֲᣬ������Ҫ�ȴ�һ��ʱ�䣨���� 120ms����ȷ����ʾ�ȶ�
    Delay_Ms(120); // ʹ�� HAL �����ʱ��������ʱ 120 ����
}
/********************************************************************************
function:   Set the resolution and scanning method of the screen
parameter:
        Scan_dir:   Scan direction
********************************************************************************/
static void Screen_SetAttributes(UBYTE Scan_dir)
{
    // Get the screen scan direction
    LCD.SCAN_DIR = Scan_dir;
    UBYTE MemoryAccessReg = 0x00;

    // Get GRAM and LCD width and height
    // if (Scan_dir == HORIZONTAL)
    // {
    //     LCD.HEIGHT = LCD_HEIGHT;
    //     LCD.WIDTH = LCD_WIDTH;
    //     MemoryAccessReg = 0X70;
    // }
    // else
    // {
    LCD.HEIGHT = LCD_WIDTH;
    LCD.WIDTH = LCD_HEIGHT;
    MemoryAccessReg = 0XC8;
    // }

    // Set the read / write scan direction of the frame memory
    Screen_SendCommand(0x36);              // MX, MY, RGB mode
    Screen_SendData_8Bit(MemoryAccessReg); // 0x08 set RGB
}

static void Screen_Reset(void)
{

    LCD_RST_0;
    Delay_Ms(20);
    LCD_RST_1;
    Delay_Ms(20);
}

/********************************************************************************
function :  Initialize the lcd
parameter:
********************************************************************************/
void Screen_Init(UBYTE Scan_dir)
{

    // Hardware reset
    Screen_Reset();

    // Set the resolution and scanning method of the screen
    Screen_SetAttributes(Scan_dir);

    // Set the initialization register
    Screen_InitReg();
}

/********************************************************************************
function:   Sets the start position and size of the display area
parameter:
        Xstart  :   X direction Start coordinates
        Ystart  :   Y direction Start coordinates
        Xend    :   X direction end coordinates
        Yend    :   Y direction end coordinates
********************************************************************************/
void Screen_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{

    dmaXoffset = 0;
    dmaYoffset = 0;

    Ystart = Ystart + 1;
    Yend = Yend + 1;

    Xstart = Xstart + 2;
    Xend = Xend + 2;

    // set the X coordinates
    Screen_SendCommand(0x2A);
    Screen_SendData_8Bit((Xstart >> 8) & 0xFF);
    Screen_SendData_8Bit(Xstart & 0xFF);
    Screen_SendData_8Bit(((Xend) >> 8) & 0xFF);
    Screen_SendData_8Bit((Xend) & 0xFF);

    // set the Y coordinates
    Screen_SendCommand(0x2B);
    Screen_SendData_8Bit((Ystart >> 8) & 0xFF);
    Screen_SendData_8Bit(Ystart & 0xFF);
    Screen_SendData_8Bit(((Yend) >> 8) & 0xFF);
    Screen_SendData_8Bit((Yend) & 0xFF);

    Screen_SendCommand(0X2C);
}

/******************************************************************************
function :  Clear screen
parameter:
******************************************************************************/
// 0��128
u8 Screen_Clear(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color)
{

#if USE_DMA

    int dma_circular = 0;
    u8 retry = 0;
    Screen_SetWindows(Xstart, Ystart, Xend + 0, Yend + 0);
    int index = 0; // ���ڸ���lcd_gram���������

    // ���ڼ�����ʱ����ռ�õ��ڴ�
    uint8_t high_byte = (Color >> 8) & 0xff; // ���ֽ�
    uint8_t low_byte = Color & 0xff;         // ���ֽ�

    for (int i = 0; i < X_MAX_PIXEL; i++)
    {
        for (int j = 0; j < Y_MAX_PIXEL; j++)
        {
            lcd_gram[index++] = high_byte; // ���ֽ�
            lcd_gram[index++] = low_byte;  // ���ֽ�
        }
    }

    LCD_DC_1;
    LCD_CS_ENABLE;

    // ��ʼ��DMA����
    SPI_DMA_Tx_Init(DMA1_Channel3, (u32)&SPI1->DATAR, (u32)lcd_gram, Y_MAX_PIXEL * X_MAX_PIXEL * 2, DMA_Mode_Circular, DMA_MemoryInc_Enable);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
    DMA_Cmd(DMA1_Channel3, ENABLE);

    // �����ܴ������
    int total_dma_transfers = ((Yend - Ystart) * (Xend - Xstart) / (X_MAX_PIXEL * Y_MAX_PIXEL)) + 1;

    while (dma_circular <= total_dma_transfers)
    {
        if (dmaTransferComplete)
        {
            dmaTransferComplete = 0;
            dma_circular++;

            // ��ӡ��ǰ�����������������
            // DEBUG_PRINT("Current DMA transfers: %d/%d\r\n", dma_circular, total_dma_transfers);
        }
    }

    // ���ü�����
    dma_circular = 0;

    // �ȴ�SPI���ͻ�����Ϊ��
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
    {
        // ���Կ����ڴ˴���ӳ�ʱ�߼����Է���ѭ��
        //  DEBUG_PRINT("Waiting for SPI transmit buffer to be empty...\r\n"); // ��ӡ�ȴ���Ϣ
    }

    // // ����DMA��SPI
     DMA_Cmd(DMA1_Channel3, DISABLE);
     SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
    DMA_ClearITPendingBit(DMA1_IT_TC3);

    LCD_CS_DISABLE;
    // DEBUG_PRINT("Screen_Clear OK\r\n"); // �ȴ�SPI���ͻ�����Ϊ��

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
    {
        retry++;
        if (retry > 200)
        {
            DEBUG_PRINT("rec spi outtime!\r\n");
            return 0;
        }
    } // ��û����

    return (uint8_t)SPI_I2S_ReceiveData(SPI1);

#else
    UWORD i, j;
    Screen_SetWindows(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    LCD_DC_1;
    for (i = 0; i < LCD_WIDTH; i++)
    {
        for (j = 0; j < LCD_HEIGHT; j++)
        {
            LCD_DC_1;
            LCD_CS_ENABLE;
            Screen_spi_write((Color >> 8) & 0xff);
            Screen_spi_write(Color & 0xff);
            LCD_CS_DISABLE;
        }
    }
#endif
}

/******************************************************************************
function :  Sends the image buffer in RAM to displays
parameter:
******************************************************************************/
void Screen_Display(UWORD *Image)
{
    UWORD i, j;
    Screen_SetWindows(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    LCD_DC_1;
    for (i = 0; i < LCD_WIDTH; i++)
    {
        for (j = 0; j < LCD_HEIGHT; j++)
        {
            Screen_spi_write((*(Image + i * LCD_WIDTH + j) >> 8) & 0xff);
            Screen_spi_write(*(Image + i * LCD_WIDTH + j));
        }
    }
}

void Screen_DisplayWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD *Image)
{
    // display
    UDOUBLE Addr = 0;

    UWORD i, j;
    Screen_SetWindows(Xstart, Ystart, Xend - 1, Yend - 1);
    LCD_DC_1;
    for (i = Ystart; i < Yend - 1; i++)
    {
        Addr = Xstart + i * LCD_WIDTH;
        for (j = Xstart; j < Xend - 1; j++)
        {
            Screen_spi_write((*(Image + Addr + j) >> 8) & 0xff);
            Screen_spi_write(*(Image + Addr + j));
        }
    }
}

u8 LCD_Refrsh_DMA(int pic_size)
{

#if USE_DMA

    // ���������ݰ���һ�ε�DMA
    LCD_DC_1;
    LCD_CS_ENABLE;

    // DEBUG_PRINT("��ʼˢ��\r\n");
    dmaTransferComplete = 0;
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
    DMA_Cmd(DMA1_Channel3, DISABLE);

    SPI_DMA_Tx_Init(DMA1_Channel3, (u32)&SPI1->DATAR, (u32)lcd_gram, pic_size, DMA_Mode_Normal, DMA_MemoryInc_Enable);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
    DMA_Cmd(DMA1_Channel3, ENABLE);

    while (!dmaTransferComplete)
    {
        //  DEBUG_PRINT("wait normoldma ok %d\r\n", dmaTransferComplete); // �ȴ�ͨ��3������ɱ�־
    }
    dmaTransferComplete = 0;
    DMA_ClearFlag(DMA1_FLAG_TC3); // ���ͨ��3������ɱ�־

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
        ;

    DMA_Cmd(DMA1_Channel3, DISABLE);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
    DMA_ClearITPendingBit(DMA1_IT_TC3);

    u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
    {
        retry++;
        if (retry > 200)
        {
            // DEBUG_PRINT("rec spi outtime!\r\n");
            return 0;
        }
    } // ��û����

    LCD_CS_DISABLE;

    memset(lcd_gram, 0xff, pic_size);

    return (uint8_t)SPI_I2S_ReceiveData(SPI1);

#endif
}
/******************************************************************************
function: Draw a point
parameter   :
        X   :   Set the X coordinate
        Y   :   Set the Y coordinate
      Color :   Set the color
******************************************************************************/
void Screen_DrawPaint(UWORD x, UWORD y, UWORD Color)
{

    //
    //   DEBUG_PRINT("x = %d, y = %d,dmaXpoint:%d,dmaYpoint:%d\r\n",x,y,dmaXpoint,dmaYpoint);
    //      DEBUG_PRINT("dmaFont->Width:%d\r\n", dmaFont->Width);
    //

#if USE_DMA

    ///< ʹ��DMA�Ļ����ӶԵ�ˢ�������Դ�����д�����ݣ�DMA�������ݵ�ʱ����ͳһ���д���
    int index = ((y - dmaYpoint) * (dmaFont->Width) + (x - dmaXpoint)) * 2; // �µ��������ת����012345
    // DEBUG_PRINT("index��ʼƫ��ǰ:%d\r\n", index );

    index = index - X_MAX_PIXEL * Y_MAX_PIXEL * 2 * dmaXoffset / X_MAX_PIXEL; // ƫ��
    lcd_gram[index] = (Color >> 8) & 0xff;                                    // ���ֽ�
    lcd_gram[index + 1] = Color & 0xFF;                                       // ���ֽ�
                                                                              //  DEBUG_PRINT("��ʼƫ��:%d\r\n", index + 1);

    if ((index + 1) == (Y_MAX_PIXEL * X_MAX_PIXEL * 2 - 1))
    {

        LCD_Refrsh_DMA(Y_MAX_PIXEL * X_MAX_PIXEL * 2);
        dmaXoffset = X_MAX_PIXEL + dmaXoffset;
        dmaYoffset = Y_MAX_PIXEL + dmaYoffset;
        // DEBUG_PRINT("dma ok\r\n");
    }

#else
    Screen_SetWindows(x, y, x, y);
    Screen_SendData_16Bit(Color);
#endif
}

/*******************************************************************************
function:
    Setting backlight
parameter   :
      value : Range 0~1000   Duty cycle is value/1000
*******************************************************************************/
void Screen_SetBackLight(UWORD Value)
{
    DEV_Set_PWM(Value);
}
