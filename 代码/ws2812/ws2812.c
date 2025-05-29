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
static volatile uint8_t led_buffer[LED_NUM][3] = {0};


void WS2812_0(void)  // 输出0码
{
    /* T0H - 300ns , T0L - 600ns */
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);  // 7个高电平时间 45ns
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);
    GPIOC->BCR = GPIO_Pin_4;                            // 15个低电平时间 40ns
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

void WS2812_1(void)  // 输出1码
{
    /* T1H - 600ns , T1L - 600ns */
    GPIOC->BSHR = (GPIO_Pin_4 & (uint32_t)0x0000FFFF);  // 14个高电平时间 45ns
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
    GPIOC->BCR = GPIO_Pin_4;                            // 15个低电平时间 40ns
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

void WS2812_Reset(void)  // 输出Reset码
{
    /* RES 低电平280us以上 */
    GPIOC->BCR = GPIO_Pin_4;

    Delay_Ms(50);
}


void Delay_Ns(uint32_t ns)
{
    // 主频 48MHz ≈ 每个 NOP 约 21ns
    uint32_t cycles = (ns + 20) / 21; // 四舍五入
    while (cycles--)
    {
        __NOP();
    }
}
// 逻辑0：高电平持续时间约为0.25?s，低电平持续时间约为1?s。
// 逻辑1：高电平持续时间约为1?s，低电平持续时间约为0.25?s。
// 复位信号：低电平持续时间至少为280?s。
// 发送一个字节（8bit）数据
void WS2812_SendByte(uint8_t byte)
{
    for (int i = 7; i >= 0; i--)
    {

        if (byte & (1 << i))
        { 

           WS2812_1();

            //  // 发送 1
            //  WS2812_ENABLE;
            //  Delay_Ns(700); // 高电平 0.7μs
            //  WS2812_DISABLE;
            //  Delay_Ns(600); // 低电平 0.6μs
        }
        else
        { 

           WS2812_0();

            //   // 发送 0
            //  WS2812_ENABLE;
            //  Delay_Ns(300); // 高电平 0.35μs
            //  WS2812_DISABLE;
            //  Delay_Ns(600); // 低电平 0.8μs
        }
    }
}
//
//// 发送 RESET 信号（≥50μs 低电平）
//void WS2812_Reset()
//{
//    WS2812_DISABLE;
//    Delay_Us(280); // 稍大于 50μs
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


        __disable_irq();         // 屏蔽所有中断（最保险）
        Delay_Ns(1);
            WS2812_SendByte(led_buffer[0][0]); // G
            WS2812_SendByte(led_buffer[0][1]); // R
            WS2812_SendByte(led_buffer[0][2]); // B

            WS2812_SendByte(led_buffer[1][0]); // G
              WS2812_SendByte(led_buffer[1][1]); // R
              WS2812_SendByte(led_buffer[1][2]); // B
        Delay_Ns(1);
        __enable_irq();          // 恢复中断






//    DEBUG_PRINT("color R=%d G=%d B=%d\r\n", led_buffer[1][0],led_buffer[1][1],led_buffer[1][2]);
//    DEBUG_PRINT("color R=%d G=%d B=%d\r\n", led_buffer[0][0],led_buffer[0][1],led_buffer[0][2]);
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
