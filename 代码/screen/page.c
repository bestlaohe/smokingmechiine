/*
 * page.c
 *
 *  Created on: 2024年9月21日
 *      Author: 12630
 */
#include "page.h"
#include <string.h>
#include "seting.h"
#include "encode.h"      // 添加编码器头文件引用
#include "adc.h"         // 添加ADC头文件引用
#include "ch32v00x_it.h" // 添加按键结构定义头文件

#define CHAR_HEIGHT 30 // 单个字符高度
#define Y_OFFSET 22    // 设置页面初始的y轴偏移

u8 current_setting = 0;    // 当前设置的行
u8 refreshState = 1;       // 内容刷新标志位
u8 isFirstSettingShow = 1; // 设置刷新标志
u8 isFirstBattaryShow = 1; // 电池刷新标志

// 风扇速度值
u8 fan_speed = 0; // 默认50%速度

// 照明LED亮度值
u8 led_light = 0; // 默认80%亮度

// 修改settings数组，包含屏幕亮度、风扇速度和照明LED设置
Setting settings[SETTING_COUNT] = {
    {"56", (u8 *)&TIM1->CH3CVR, NULL},
    {"78", &fan_speed, NULL},
    {"9:", &led_light, NULL}};

Page page = PAGE_SETTING;

// 辅助函数，将值限制在指定范围内
void clamp_value(int *value, int min, int max)
{
  if (*value < min)
    *value = min;
  if (*value > max)
    *value = max;
}

// 风扇速度控制函数
void FAN_SetSpeed(uint8_t percentage)
{
  uint16_t compare_value = 0;

  // 确保百分比在0-100范围内
  if (percentage > 100)
    percentage = 100;

  // 计算PWM比较值
  compare_value = (100 - percentage);

  // 设置风扇PWM输出
  TIM1->CH2CVR = compare_value;
  WS2812_show_status(1,compare_value);
}

// 照明LED亮度控制函数
void LED_SetLight(uint8_t percentage)
{
  uint16_t compare_value = 0;

  // 确保百分比在0-100范围内
  if (percentage > 100)
    percentage = 100;

  // 计算PWM比较值
  compare_value = (100 - percentage);

  // 设置照明LED PWM输出
  TIM1->CH1CVR = compare_value;
   WS2812_show_status(0,compare_value);
}

// 更新当前设置值
void update_current_setting(int value)
{
  switch (current_setting)
  {
  case SETTING_SCREEN_LIGHT:
    value = (value < 0) ? 0 : (value > 100) ? 100
                                            : value;
    TIM1->CH3CVR = value;
    break;
  case SETTING_FAN_SPEED:
    value = (value < 0) ? 0 : (value > 100) ? 100
                                            : value;
    fan_speed = value;
    FAN_SetSpeed(fan_speed);
    break;
  case SETTING_LED_LIGHT:
    value = (value < 0) ? 0 : (value > 100) ? 100
                                            : value;
    led_light = value;
    LED_SetLight(led_light);
    break;
  default:
    break;
  }

  if (settings[current_setting].update_func)
  {
    settings[current_setting].update_func();
  }
}

// 绘制设置项
void draw_setting(int index, int highlight, sFONT *Font)
{
  char strBuf[4]; // 用于存储最多3位数字和一个终止符
  UWORD bg_color = highlight ? GREEN : MY_THEME_COMPONT_COLOR;
  UWORD text_color = 0x07FF; // 青色，屏幕背光区域颜色

  // 显示设置名
  Paint_DrawString(0, index * CHAR_HEIGHT + Y_OFFSET, settings[index].name, &Font16_setting, MY_THEME_BACK_COLOR, highlight ? bg_color : text_color, '0', 999);

  // 绘制进度条
  int bar_width = 80;
  int bar_height = 10;
  int bar_x = 0;
  int bar_y = index * CHAR_HEIGHT + Y_OFFSET + 16;
  int filled_width = bar_width * (*settings[index].value) / 100;
  Paint_DrawRectangle(bar_x, bar_y, bar_x + 80, bar_y + bar_height, MY_THEME_BACK_COLOR, DOT_PIXEL_1X1, DRAW_FILL_FULL);

  // 绘制空白进度条
  Paint_DrawRectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, 0x7BEF, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

  // 绘制填充部分
  if (filled_width > 0)
  {
    Paint_DrawRectangle(bar_x, bar_y, bar_x + filled_width, bar_y + bar_height, highlight ? bg_color : text_color, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  }

  // 显示数值
  intToStr(*settings[index].value, strBuf, 3);
  Paint_DrawString(LCD_WIDTH - 4 * Font16_Num.Width, bar_y, strBuf, &Font16_Num, MY_THEME_BACK_COLOR, highlight ? bg_color : text_color, '0', 999);

  // 绘制百分比符号

  Paint_DrawChar(LCD_WIDTH - 4 * Font16_Num.Width + 3 * Font16_Num.Width, bar_y, 10, &Font16_Num, MY_THEME_BACK_COLOR, highlight ? bg_color : text_color, 0);
}
u8 needShowBattery = 1;
// 显示所有设置项
void display_settings(sFONT *Font)
{
  if (refreshState)
  {
    // 清屏
    // Screen_Clear(0, 0, LCD_WIDTH, LCD_HEIGHT, MY_THEME_BACK_COLOR);

    needShowBattery = 1;
    // 吸烟机
    Paint_DrawString(0, 0, "234", &Font16_setting, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, '0', 999);

    for (int i = 0; i < SETTING_COUNT; i++)
    {
      draw_setting(i, i == current_setting, Font);
    }
    refreshState = 0;
  }
}

// 处理设置页面的编码器和按键事件
void handle_setting_event()
{

  if (encode_struct.state == ENCODE_EVENT_UP)
  {                                                               // 编码器向上滚动
    update_current_setting(*settings[current_setting].value + 5); // 每次增加5%
    refreshState = 1;
  }
  else if (encode_struct.state == ENCODE_EVENT_DOWN)
  {                                                               // 编码器向下滚动
    update_current_setting(*settings[current_setting].value - 5); // 每次减少5%
    refreshState = 1;
  }

  if (key.event == KEY_EVENT_CLICK || key.event == KEY_EVENT_LONG_CLICK)
  { // 确认键点击
    current_setting = (current_setting + 1) % SETTING_COUNT;
    refreshState = 1;
  }
}

// 设置页面显示
void setting_page(sFONT *Font)
{
  // 显示电池信息
#if BATTERY_ENABLED
  // 使用adc.c中的show_battery函数

  show_battery(BATTERY_X, BATTERY_Y, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, &needShowBattery); // 显示电池信息

#endif

  // 显示设置项
  display_settings(Font);

  // 处理编码器和按键事件
  handle_setting_event();
}

// 主页面显示函数
void show_page()
{
  setting_page(&Font16_En);
  key.event = KEY_EVENT_NONE;
  encode_struct.state = ENCODE_EVENT_NONE;
}
