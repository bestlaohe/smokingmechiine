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
    // �����
    GPIO_Init_Pin(GPIOC, GPIO_Pin_1, GPIO_Mode_IPD);
    // ��Ļ�ĸ�λ
    GPIO_Init_Pin(GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP);
    // pc0��lora��nss
    GPIO_Init_Pin(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP);
    // pc1��lora�ĸ�λ
    GPIO_Init_Pin(GPIOC, GPIO_Pin_0, GPIO_Mode_Out_PP);

    // pd2��ȷ�ϰ�ť
    GPIO_Init_Pin(GPIOD, GPIO_Pin_2, GPIO_Mode_IPU);
    // pd6��lora���ж�
    GPIO_Init_Pin(GPIOD, GPIO_Pin_6, GPIO_Mode_IPD);
    // ��loraͨ��miso
    GPIO_Init_Pin(GPIOC, GPIO_Pin_7, GPIO_Mode_IN_FLOATING);

    // pA2��ģ���
    GPIO_Init_Pin(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP);
    // pA1��ص����ɼ�
    GPIO_Init_Pin(GPIOA, GPIO_Pin_1, GPIO_Mode_AIN);
    // pc3�����pwm
    GPIO_Init_Pin(GPIOC, GPIO_Pin_3, GPIO_Mode_AF_PP);
    // �����������ģʽ��LCD_DC_1   LCD_CS
    GPIO_Init_Pin(GPIOC, GPIO_Pin_2 | GPIO_Pin_4, GPIO_Mode_Out_PP);

    // ��������io��d3d4
    GPIO_Init_Pin(GPIOD, GPIO_Pin_3 | GPIO_Pin_4, GPIO_Mode_IPU);
    // LCD_SCK_1;  SPI_SDA_1;
    GPIO_Init_Pin(GPIOC, GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_AF_PP);

    MOTOR_OFF; // �رյ��
    LCD_CS_DISABLE;
    SX1278_NSS_DISABLE;
}
void My_GPIO_DeInit()
{
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOD);
}

u8 shake_mode = 1; // ��ʼ��ģʽֵ
void MOTOR_SET(int state)
{
    // DEBUG_PRINT("MOTOR_SET=%d,%d\r\n", shake_mode, state);
    if (shake_mode)
    {
        if (state)
        {
            MOTOR_ON;
        }else
            MOTOR_OFF;
    }
}
