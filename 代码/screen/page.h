/*
 * page.h
 *
 *  Created on: 2024年9月21日
 *      Author: 12630
 */

#ifndef PAGE_H_
#define PAGE_H_

#include "screen_api.h"
#include "fonts.h"
#include <stdio.h>
#include "ch32v00x.h"
#include "seting.h"

// 定义页面类型
typedef enum {
    PAGE_SETTING = 0,  // 设置页面，简化为只有一个页面
} Page;

// 定义按钮状态,可开关的选项
typedef enum {
    OFF = 0,
    ON = 1,
} BUTTON_STATE;

// 设置选项索引
typedef enum {
    SETTING_SCREEN_LIGHT = 0,  // 屏幕亮度
    SETTING_FAN_SPEED = 1,     // 风扇速度
    SETTING_LED_LIGHT = 2,     // 照明LED亮度
    SETTING_COUNT = 3          // 设置总数
} SettingIndex;

// 设置结构体定义
typedef struct {
    char *name;            // 设置名称
    u8 *value;            // 设置值的指针
    void (*update_func)(void); // 更新函数指针
} Setting;

// 变量声明
extern u8 current_setting;               // 当前设置项
extern u8 refreshState;                 // 刷新标志
extern u8 isFirstSettingShow;           // 首次设置显示标志
extern u8 isFirstBattaryShow;           // 首次电池显示标志
extern Page page;                       // 当前页面
extern Setting settings[SETTING_COUNT]; // 设置数组
extern u8 fan_speed;                    // 风扇速度值
extern u8 led_light;                    // 照明LED亮度值

// 函数声明
void show_page(void);
void setting_page(sFONT *Font);
void update_current_setting(int value);
void clamp_value(int *value, int min, int max);
void FAN_SetSpeed(uint8_t percentage);
void LED_SetLight(uint8_t percentage);
void show_battery(int x, int y, UWORD back_color, UWORD front_color, u8 *isFirstBattaryShow);

#endif /* PAGE_H_ */
