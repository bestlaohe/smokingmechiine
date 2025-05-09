/*
 * ws2812.h
 *
 *  Created on: 2025Äê5ÔÂ9ÈÕ
 *      Author: 12630
 */
#include "gpio.h"
#include "debug.h"
#ifndef WS2812_WS2812_H_
#define WS2812_WS2812_H_
void WS2812_Reset();
void WS2812_SendRGB(uint8_t r, uint8_t g, uint8_t b);
void WS2812_Update(void);
void WS2812_SetAll(uint8_t r, uint8_t g, uint8_t b);
void WS2812_SetLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void WS2812_show_status(uint8_t index, uint8_t percent);
#endif /* WS2812_WS2812_H_ */
