/*
 * ws2812.c
 *
 *  Created on: 2025��5��9��
 *      Author: 12630
 */

#include "ws2812.h"

// LED����������ʵ���޸ģ�
#define LED_NUM 2

// ȫ��LED��ɫ���棨GRB��ʽ��
static volatile uint8_t led_buffer[LED_NUM][3] = {0};


void WS2812_0(void)  // ���0��
{
    /* T0H - 300ns , T0L - 600ns */
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);  // 7���ߵ�ƽʱ�� 45ns
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BCR = GPIO_Pin_4;                            // 15���͵�ƽʱ�� 40ns
    GPIOC->BCR = GPIO_Pin_4;
    GPIOC->BCR = GPIO_Pin_4;
    GPIOC->BCR = GPIO_Pin_4;
    GPIOC->BCR = GPIO_Pin_4;
    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
}

void WS2812_1(void)  // ���1��
{
    /* T1H - 600ns , T1L - 600ns */
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);  // 14���ߵ�ƽʱ�� 45ns
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BCR = GPIO_Pin_4;                            // 15���͵�ƽʱ�� 40ns
    GPIOC->BCR = GPIO_Pin_4;
    GPIOC->BCR = GPIO_Pin_4;
    GPIOC->BCR = GPIO_Pin_4;
    GPIOC->BCR = GPIO_Pin_4;
    GPIOC->BCR = GPIO_Pin_4;
    GPIOC->BCR = GPIO_Pin_4;
    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
//    GPIOC->BCR = GPIO_Pin_4;
}

void WS2812_Reset(void)  // ���Reset��
{
    /* RES �͵�ƽ280us���� */
    GPIOC->BCR = GPIO_Pin_4;

    Delay_Ms(50);
}


void Delay_Ns(uint32_t ns)
{
    // ��Ƶ 48MHz �� ÿ�� NOP Լ 21ns
    uint32_t cycles = (ns + 20) / 21; // ��������
    while (cycles--)
    {
        __NOP();
    }
}
// �߼�0���ߵ�ƽ����ʱ��ԼΪ0.25?s���͵�ƽ����ʱ��ԼΪ1?s��
// �߼�1���ߵ�ƽ����ʱ��ԼΪ1?s���͵�ƽ����ʱ��ԼΪ0.25?s��
// ��λ�źţ��͵�ƽ����ʱ������Ϊ280?s��
// ����һ���ֽڣ�8bit������
void WS2812_SendByte(uint8_t byte)
{
    for (int i = 7; i >= 0; i--)
    {

        if (byte & (1 << i))
        { 

           WS2812_1();

            //  // ���� 1
            //  WS2812_ENABLE;
            //  Delay_Ns(700); // �ߵ�ƽ 0.7��s
            //  WS2812_DISABLE;
            //  Delay_Ns(600); // �͵�ƽ 0.6��s
        }
        else
        { 

           WS2812_0();

            //   // ���� 0
            //  WS2812_ENABLE;
            //  Delay_Ns(300); // �ߵ�ƽ 0.35��s
            //  WS2812_DISABLE;
            //  Delay_Ns(600); // �͵�ƽ 0.8��s
        }
    }
}
//
//// ���� RESET �źţ���50��s �͵�ƽ��
//void WS2812_Reset()
//{
//    WS2812_DISABLE;
//    Delay_Us(280); // �Դ��� 50��s
//}

void WS2812_Update(void)
{



//    for (int i = 0; i < LED_NUM; i++)
//    {
//        WS2812_SendByte(led_buffer[i][0]); // G
//        WS2812_SendByte(led_buffer[i][1]); // R
//        WS2812_SendByte(led_buffer[i][2]); // B
//
//    }


        __disable_irq();         // ���������жϣ���գ�
        Delay_Ns(1);
            WS2812_SendByte(led_buffer[0][0]); // G
            WS2812_SendByte(led_buffer[0][1]); // R
            WS2812_SendByte(led_buffer[0][2]); // B

            WS2812_SendByte(led_buffer[1][0]); // G
              WS2812_SendByte(led_buffer[1][1]); // R
              WS2812_SendByte(led_buffer[1][2]); // B
        Delay_Ns(1);
        __enable_irq();          // �ָ��ж�






//    DEBUG_PRINT("color R=%d G=%d B=%d\r\n", led_buffer[1][0],led_buffer[1][1],led_buffer[1][2]);
//    DEBUG_PRINT("color R=%d G=%d B=%d\r\n", led_buffer[0][0],led_buffer[0][1],led_buffer[0][2]);
   WS2812_Reset();
}

void WS2812_SetLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    if (index >= LED_NUM)
        return; // ��ֹԽ��

    led_buffer[index][0] = g; // WS2812ʹ��GRB˳��
    led_buffer[index][1] = r;
    led_buffer[index][2] = b;
}
// ��������LEDΪͬһ��ɫ
void WS2812_SetAll(uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < LED_NUM; i++)
    {
        WS2812_SetLED(i, r, g, b);
    }
}

void WS2812_show_status(uint8_t index, uint8_t percent)
{
    if (percent <= 30)
    {
        WS2812_SetLED(index, 0, 255*percent/30, 0);
    }
    else if (percent > 30 && percent <= 60)
    {
        WS2812_SetLED(index, 0, 0, (percent - 30) * 255 / 30);
    }
    else if (percent > 60 && percent <= 100)
    {
        WS2812_SetLED(index, (percent - 60) * 255 / 40, 0, 0);
    }
     WS2812_Update();
}

//   WS2812_SetLED(1,255, 0, 0);
//     WS2812_Update();
