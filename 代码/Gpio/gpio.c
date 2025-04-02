/*
 * gpio.c
 *
 *  Created on: 2024年9月1日
 *      Author: 12630
 */

#include "gpio.h"
void GPIO_Init_Pin(GPIO_TypeDef *GPIOx, uint16_t pin, GPIOMode_TypeDef mode)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz; // 假设大多数情况下速度都一样
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void My_GPIO_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    // PA1 - 电池电量检测 (ADC输入)
    GPIO_Init_Pin(GPIOA, GPIO_Pin_1, GPIO_Mode_AIN);
    
    // PA2 - 风扇PWM输出 (TIM1_CH2N输出，控制风扇转速)
    GPIO_Init_Pin(GPIOA, GPIO_Pin_2, GPIO_Mode_AF_PP);
    
    // PC0 - 显示屏RST (显示屏复位信号，低电平有效)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_0, GPIO_Mode_Out_PP);
    
    // PC1 - 充电状态检测 (GPIO输入，检测充电状态)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_1, GPIO_Mode_IPU);
    
    // PC2 - 显示屏DC (数据/命令选择线)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_2, GPIO_Mode_Out_PP);
    
    // PC3 - 屏幕背光PWM控制 (TIM1_CH3输出，控制背光PWM)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_3, GPIO_Mode_AF_PP);
    
    // PC4 - 显示屏SPI_CS (SPI片选信号)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP);
    
    // PC5 - 显示屏SPI_SCK (SPI时钟信号)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_5, GPIO_Mode_AF_PP);
    
    // PC6 - 显示屏SPI_MOSI (SPI数据输出)
    GPIO_Init_Pin(GPIOC, GPIO_Pin_6, GPIO_Mode_AF_PP);
    
    // PD0 - 照明LED PWM输出 (TIM1_CH1N输出，控制LED亮度)
    GPIO_Init_Pin(GPIOD, GPIO_Pin_0, GPIO_Mode_AF_PP);
    
    // PD1 - 调试/烧录接口 (烧录和调试用SDIO接口)
    // 此引脚不需要额外配置，由调试系统自动处理
    
    // PD2 - 功能按键输入 (外部中断，检测按键按下)
    GPIO_Init_Pin(GPIOD, GPIO_Pin_2, GPIO_Mode_IPU);
    
    // PD3 - 旋转编码器B相 (GPIO输入，检测旋转方向)
    GPIO_Init_Pin(GPIOD, GPIO_Pin_3, GPIO_Mode_IPU);
    
    // PD4 - 旋转编码器A相 (外部中断，检测编码器旋转)
    GPIO_Init_Pin(GPIOD, GPIO_Pin_4, GPIO_Mode_IPU);
    
    // PD5 - 串口TX (UART数据发送线)
    GPIO_Init_Pin(GPIOD, GPIO_Pin_5, GPIO_Mode_AF_PP);

    // 初始化完成后禁用SPI片选
    LCD_CS_DISABLE;
}

void My_GPIO_DeInit()
{
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOD);
}
