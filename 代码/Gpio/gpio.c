/*
 * gpio.c
 *
 *  Created on: 2024��9��1��
 *      Author: 12630
 */

#include "gpio.h"
void GPIO_Init_Pin(GPIO_TypeDef *GPIOx, uint16_t pin, GPIOMode_TypeDef mode)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz; // ��������������ٶȶ�һ��
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void My_GPIO_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    // PA1 - ��ص������ (ADC����)
    GPIO_Init_Pin(GPIOA, GPIO_Pin_1, GPIO_Mode_AIN);
    
    // PA2 - ����PWM��� (TIM1_CH2N��������Ʒ���ת��)
    GPIO_Init_Pin(GPIOA, GPIO_Pin_2, GPIO_Mode_AF_PP);
    
    // PC0 - ��ʾ��RST (��ʾ����λ�źţ��͵�ƽ��Ч)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_0, GPIO_Mode_Out_PP);
    
    // PC1 - ���״̬��� (GPIO���룬�����״̬)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_1, GPIO_Mode_IPU);
    
    // PC2 - ��ʾ��DC (����/����ѡ����)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_2, GPIO_Mode_Out_PP);
    
    // PC3 - ��Ļ����PWM���� (TIM1_CH3��������Ʊ���PWM)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_3, GPIO_Mode_AF_PP);
    
    // PC4 - ��ʾ��SPI_CS (SPIƬѡ�ź�)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP);
    
    // PC5 - ��ʾ��SPI_SCK (SPIʱ���ź�)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_5, GPIO_Mode_AF_PP);
    
    // PC6 - ��ʾ��SPI_MOSI (SPI�������)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_6, GPIO_Mode_AF_PP);
    
    // PD0 - ����LED PWM��� (TIM1_CH1N���������LED����)
    GPIO_Init_Pin(GPIOD, GPIO_Pin_0, GPIO_Mode_AF_PP);
    
    // PD1 - ����/��¼�ӿ� (��¼�͵�����SDIO�ӿ�)
    // �����Ų���Ҫ�������ã��ɵ���ϵͳ�Զ�����
    
    // PD2 - ���ܰ������� (�ⲿ�жϣ���ⰴ������)
    GPIO_Init_Pin(GPIOD, GPIO_Pin_2, GPIO_Mode_IPU);
    
    // PD3 - ��ת������B�� (GPIO���룬�����ת����)
    GPIO_Init_Pin(GPIOD, GPIO_Pin_3, GPIO_Mode_IPU);
    
    // PD4 - ��ת������A�� (�ⲿ�жϣ�����������ת)
    GPIO_Init_Pin(GPIOD, GPIO_Pin_4, GPIO_Mode_IPU);
    
    // PD5 - ����TX (UART���ݷ�����)
    GPIO_Init_Pin(GPIOD, GPIO_Pin_5, GPIO_Mode_AF_PP);

    // ��ʼ����ɺ����SPIƬѡ
    LCD_CS_DISABLE;
}

void My_GPIO_DeInit()
{
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOD);
}
