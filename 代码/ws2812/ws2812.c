/*
 * ws2812.c
 *
 *  Created on: 2025年5月9日
 *      Author: 12630
 */

#include "ws2812.h"

// LED数量（根据实际修改）
#define LED_NUM 2

// 全局LED颜色缓存（GRB格式）
static uint8_t led_buffer[LED_NUM][3] = {0};

// 发送一个字节（8bit）数据
void WS2812_SendByte(uint8_t byte)
{
    for (int i = 7; i >= 0; i--)
    {
        if (byte & (1 << i))
        { // 发送 1
            WS2812_ENABLE;
            Delay_Us(700); // 高电平 0.7μs
            WS2812_DISABLE;
            Delay_Us(600); // 低电平 0.6μs
        }
        else
        { // 发送 0
            WS2812_ENABLE;
            Delay_Us(350); // 高电平 0.35μs
            WS2812_DISABLE;
            Delay_Us(800); // 低电平 0.8μs
        }
    }
}

// 发送 RESET 信号（≥50μs 低电平）
void WS2812_Reset()
{
    WS2812_DISABLE;
    Delay_Us(60); // 稍大于 50μs
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
        return; // 防止越界

    led_buffer[index][0] = g; // WS2812使用GRB顺序
    led_buffer[index][1] = r;
    led_buffer[index][2] = b;
}
// 设置所有LED为同一颜色
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
