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
static uint8_t led_buffer[LED_NUM][3] = {0};

// ����һ���ֽڣ�8bit������
void WS2812_SendByte(uint8_t byte)
{
    for (int i = 7; i >= 0; i--)
    {
        if (byte & (1 << i))
        { // ���� 1
            WS2812_ENABLE;
            Delay_Us(700); // �ߵ�ƽ 0.7��s
            WS2812_DISABLE;
            Delay_Us(600); // �͵�ƽ 0.6��s
        }
        else
        { // ���� 0
            WS2812_ENABLE;
            Delay_Us(350); // �ߵ�ƽ 0.35��s
            WS2812_DISABLE;
            Delay_Us(800); // �͵�ƽ 0.8��s
        }
    }
}

// ���� RESET �źţ���50��s �͵�ƽ��
void WS2812_Reset()
{
    WS2812_DISABLE;
    Delay_Us(60); // �Դ��� 50��s
}

void WS2812_Update(void)
{
    for (int i = 0; i < LED_NUM; i++)
    {
        WS2812_SendByte(led_buffer[i][0]); // G
        WS2812_SendByte(led_buffer[i][1]); // R
        WS2812_SendByte(led_buffer[i][2]); // B
    }
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
        WS2812_SetLED(index, 0, 255, 0);
    }
    else if (percent > 30 && percent <= 60)
    {
        WS2812_SetLED(index, 0, 0, 255);
    }
    else if (percent > 60 && percent <= 100)
    {
        WS2812_SetLED(index, 255, 0, 0);
    }
     WS2812_Update();
}

//   WS2812_SetLED(1,255, 0, 0);
//     WS2812_Update();
