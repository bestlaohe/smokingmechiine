/*
 * page.c
 *
 *  Created on: 2024年9月21日
 *      Author: 12630
 */
#include "page.h"
#include <string.h>
#include "seting.h"

#define CHAR_HEIGHT 18      // 单个字符高度
#define Y_OFFSET 22         // 设置页面初始的y轴偏移

u8 current_setting = 0;     // 当前设置的行
u8 refreshState = 1;        // 内容刷新标志位
u8 isFirstSettingShow = 1;  // 设置刷新标志
u8 isFirstBattaryShow = 1;  // 电池刷新标志

// 设置项：屏幕亮度和风扇速度
typedef enum {
    SETTING_SCREEN_LIGHT = 0,  // 屏幕亮度
    SETTING_FAN_SPEED = 1,     // 风扇速度
    SETTING_LED_LIGHT = 2,     // 照明LED亮度
    SETTING_COUNT = 3          // 设置总数
} SettingIndex;

// 风扇速度值
u8 fan_speed = 50;  // 默认50%速度

// 照明LED亮度值
u8 led_light = 80;  // 默认80%亮度

// 修改settings数组，包含屏幕亮度、风扇速度和照明LED设置
Setting settings[SETTING_COUNT] = {
    {"背光", (u8 *)&TIM1->CH3CVR, NULL},
    {"风扇", &fan_speed, NULL},
    {"照明", &led_light, NULL}
};

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
    compare_value = percentage;
    
    // 设置风扇PWM输出
    TIM1->CH2CVR = compare_value;
}

// 照明LED亮度控制函数
void LED_SetLight(uint8_t percentage)
{
    uint16_t compare_value = 0;
    
    // 确保百分比在0-100范围内
    if (percentage > 100)
        percentage = 100;
    
    // 计算PWM比较值
    compare_value = percentage;
    
    // 设置照明LED PWM输出
    TIM1->CH1CVR = compare_value;
}

// 更新当前设置值
void update_current_setting(int value)
{
  switch (current_setting)
  {
  case SETTING_SCREEN_LIGHT:
    value = (value < 0) ? 0 : (value > 100) ? 100 : value;
    TIM1->CH3CVR = value;
    break;
  case SETTING_FAN_SPEED:
    value = (value < 0) ? 0 : (value > 100) ? 100 : value;
    fan_speed = value;
    FAN_SetSpeed(fan_speed);
    break;
  case SETTING_LED_LIGHT:
    value = (value < 0) ? 0 : (value > 100) ? 100 : value;
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
  UWORD text_color;
  
  // 为不同设置项选择不同的颜色
  switch(index) {
    case SETTING_SCREEN_LIGHT:
      text_color = 0x07FF; // 青色，屏幕背光区域颜色
      break;
    case SETTING_FAN_SPEED:
      text_color = 0x07E0; // 绿色，风扇区域颜色
      break;
    case SETTING_LED_LIGHT:
      text_color = 0xFFE0; // 黄色，照明区域颜色
      break;
    default:
      text_color = MY_THEME_COMPONT_COLOR;
  }

  Paint_DrawString(0, index * CHAR_HEIGHT + Y_OFFSET, settings[index].name, Font, MY_THEME_BACK_COLOR, highlight ? bg_color : text_color, 'a', 999);
  
  // 显示数值
  intToStr(*settings[index].value, strBuf, 3);
  Paint_DrawString(Font->Width * (strlen(settings[index].name) + 3), index * CHAR_HEIGHT + Y_OFFSET, strBuf, &Font16_Num, MY_THEME_BACK_COLOR, highlight ? bg_color : text_color, '0', 999);
  
  // 绘制百分比符号
  Paint_DrawChar(Font->Width * (strlen(settings[index].name) + 3) + 3 * Font16_Num.Width, index * CHAR_HEIGHT + Y_OFFSET, '%', Font, MY_THEME_BACK_COLOR, highlight ? bg_color : text_color, 0);
  
  // 绘制进度条
  int bar_width = 88;
  int bar_height = 10;
  int bar_x = 20;
  int bar_y = index * CHAR_HEIGHT + Y_OFFSET + 16;
  int filled_width = bar_width * (*settings[index].value) / 100;
  
  // 绘制空白进度条
  Paint_DrawRectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, 0x7BEF, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  
  // 绘制填充部分
  if (filled_width > 0) {
    Paint_DrawRectangle(bar_x, bar_y, bar_x + filled_width, bar_y + bar_height, highlight ? bg_color : text_color, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  }
}

// 显示所有设置项
void display_settings(sFONT *Font)
{
  if (refreshState)
  {
    // 清屏
    Screen_Clear(0, 0, LCD_WIDTH, LCD_HEIGHT, MY_THEME_BACK_COLOR);
    
    // 显示标题和电池信息
    Paint_DrawString(32, 2, "吸烟机", &Font16_En, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, 'a', 999);
    
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
  { // 编码器向上滚动
    update_current_setting(*settings[current_setting].value + 5); // 每次增加5%
    refreshState = 1;
  }
  else if (encode_struct.state == ENCODE_EVENT_DOWN)
  { // 编码器向下滚动
    update_current_setting(*settings[current_setting].value - 5); // 每次减少5%
    refreshState = 1;
  }
  
  if (key.event == KEY_EVENT_CLICK)
  { // 确认键点击
    current_setting = (current_setting + 1) % SETTING_COUNT;
    refreshState = 1;
  }
}

// 显示电池信息
void show_battery(int x, int y, UWORD back_color, UWORD front_color, u8 *isFirstBattaryShow)
{
#if BATTERY_ENABLED
  if (*isFirstBattaryShow)
  {
    *isFirstBattaryShow = 0;
  }
  
  // 计算电池百分比
  float battery_percent = ((float)Battery_ADC_Average / 4095) * 100;
  if (battery_percent > 100)
    battery_percent = 100;
  
  // 电池颜色根据电量变化
  UWORD battery_color = (battery_percent < 20) ? RED : 0x07E0; // 绿色，低于20%变红
  
  // 绘制电池图标和百分比
  char strBuf[4];
  intToStr((int)battery_percent, strBuf, 3);
  Paint_DrawString(x, y, strBuf, &Font12_En, back_color, battery_color, '0', 999);
  Paint_DrawChar(x + 18, y, '%', &Font12_En, back_color, battery_color, 0);
  
  // 计算电池电压（假设参考电压为3.3V）
  float battery_voltage = 3.3f * (float)Battery_ADC_Average / 4095.0f;
  // 添加电压系数（根据电路设计调整）
  battery_voltage = battery_voltage * 3.03f; // 假设电压分压比
  
  // 显示电池电压
  char voltBuf[5];
  sprintf(voltBuf, "%.1fV", battery_voltage);
  Paint_DrawString(x, y + 15, voltBuf, &Font12_En, back_color, battery_color, '0', 999);
#endif
}

// 设置页面显示
void setting_page(sFONT *Font)
{
  // 显示电池信息
#if BATTERY_ENABLED
  show_battery(90, 2, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, &isFirstBattaryShow);
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

  // 处理完事件后清除事件
  key.event = KEY_EVENT_NONE;
  encode_struct.state = ENCODE_EVENT_NONE;
}
