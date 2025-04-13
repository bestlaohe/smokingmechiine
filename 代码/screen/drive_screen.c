/*
 * drive_screen.c
 *
 *  Created on: 2024��8��30��
 *      Author: 12630
 */
#include "drive_screen.h"

/*********************************************************************
 * @fn      SPI_FullDuplex_Init
 *
 * @brief   Configuring the SPI for full-duplex communication.
 *
 * @return  none
 */
void SPI_FullDuplex_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure = {0}; // SPI ��ʼ���ṹ��

    // ʹ�� GPIOC �� SPI1 ��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    // ���� SPI ����
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // ����Ϊȫ˫��ģʽ
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      // ����Ϊ��ģʽ
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  // ���ݴ�С����Ϊ 16 λ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                         // ʱ�Ӽ�������Ϊ��**************************һ��ʼ���ø�Ŷ����Ϊlora�����޸�
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                       // ʱ����λ����Ϊ�ڶ�����Ե*************һ��ʼ����λ�ڶ�����Ϊlora�����޸�
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          // NSS �ܽ�����ΪӲ������
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // ������Ԥ��Ƶ����Ϊ 64
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 // ����λ˳������Ϊ MSB ����

    SPI_InitStructure.SPI_CRCPolynomial = 7; // CRC ����ʽ����Ϊ 7
    SPI_Init(SPI1, &SPI_InitStructure);      // ��ʼ�� SPI1
    SPI_Cmd(SPI1, ENABLE);                   // ʹ�� SPI1
}

void SPI_DMA_Tx_Init(DMA_Channel_TypeDef *DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize, uint32_t mode, uint32_t MemoryInc)
{
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);

    DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = bufsize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = MemoryInc;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = mode;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHx, &DMA_InitStructure);

    // ʹ��DMA��������ж�
    DMA_ITConfig(DMA_CHx, DMA_IT_TC, ENABLE);

    // ����NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
    ���׵�us������ӳٺ���
    �ҵ�оƬʱ����48Mhz
    һ��ʱ��������1/48M=0.020833us,һ���������ڰ���12��ʱ�����ڣ�����һ������������12*0.020833=0.25us
    ��������i++����һ����0.25us��1us���������4��
*/
void delay_us(uint16_t num)
{
    for (uint16_t j = 0; j < num; j++)
    {
        for (uint16_t i = 0; i < 2; i++)
        {
        } // ���ݲ�ͬ��оƬ�����޸�i����ֵ
    }
}

u8 Screen_spi_write(uint8_t _dat)
{

    u8 retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
    {
        retry++;
        if (retry > 200)
        {
            DEBUG_PRINT("send spi outtime!\r\n");
            return 0;
        }

    } // ��û����
    SPI_I2S_SendData(SPI1, (uint16_t)_dat);

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
}




