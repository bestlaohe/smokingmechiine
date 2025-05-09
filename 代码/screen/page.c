/*
 * page.c
 *
 *  Created on: 2024��9��21��
 *      Author: 12630
 */
#include "page.h"
#include <string.h>
#include "seting.h"
#include "encode.h"      // ��ӱ�����ͷ�ļ�����
#include "adc.h"         // ���ADCͷ�ļ�����
#include "ch32v00x_it.h" // ��Ӱ����ṹ����ͷ�ļ�

#define CHAR_HEIGHT 30 // �����ַ��߶�
#define Y_OFFSET 22    // ����ҳ���ʼ��y��ƫ��

u8 current_setting = 0;    // ��ǰ���õ���
u8 refreshState = 1;       // ����ˢ�±�־λ
u8 isFirstSettingShow = 1; // ����ˢ�±�־
u8 isFirstBattaryShow = 1; // ���ˢ�±�־

// �����ٶ�ֵ
u8 fan_speed = 0; // Ĭ��50%�ٶ�

// ����LED����ֵ
u8 led_light = 0; // Ĭ��80%����

// �޸�settings���飬������Ļ���ȡ������ٶȺ�����LED����
Setting settings[SETTING_COUNT] = {
    {"56", (u8 *)&TIM1->CH3CVR, NULL},
    {"78", &fan_speed, NULL},
    {"9:", &led_light, NULL}};

Page page = PAGE_SETTING;

// ������������ֵ������ָ����Χ��
void clamp_value(int *value, int min, int max)
{
  if (*value < min)
    *value = min;
  if (*value > max)
    *value = max;
}

// �����ٶȿ��ƺ���
void FAN_SetSpeed(uint8_t percentage)
{
  uint16_t compare_value = 0;

  // ȷ���ٷֱ���0-100��Χ��
  if (percentage > 100)
    percentage = 100;

  // ����PWM�Ƚ�ֵ
  compare_value = (100 - percentage);

  // ���÷���PWM���
  TIM1->CH2CVR = compare_value;
  WS2812_show_status(1,compare_value);
}

// ����LED���ȿ��ƺ���
void LED_SetLight(uint8_t percentage)
{
  uint16_t compare_value = 0;

  // ȷ���ٷֱ���0-100��Χ��
  if (percentage > 100)
    percentage = 100;

  // ����PWM�Ƚ�ֵ
  compare_value = (100 - percentage);

  // ��������LED PWM���
  TIM1->CH1CVR = compare_value;
   WS2812_show_status(0,compare_value);
}

// ���µ�ǰ����ֵ
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

// ����������
void draw_setting(int index, int highlight, sFONT *Font)
{
  char strBuf[4]; // ���ڴ洢���3λ���ֺ�һ����ֹ��
  UWORD bg_color = highlight ? GREEN : MY_THEME_COMPONT_COLOR;
  UWORD text_color = 0x07FF; // ��ɫ����Ļ����������ɫ

  // ��ʾ������
  Paint_DrawString(0, index * CHAR_HEIGHT + Y_OFFSET, settings[index].name, &Font16_setting, MY_THEME_BACK_COLOR, highlight ? bg_color : text_color, '0', 999);

  // ���ƽ�����
  int bar_width = 80;
  int bar_height = 10;
  int bar_x = 0;
  int bar_y = index * CHAR_HEIGHT + Y_OFFSET + 16;
  int filled_width = bar_width * (*settings[index].value) / 100;
  Paint_DrawRectangle(bar_x, bar_y, bar_x + 80, bar_y + bar_height, MY_THEME_BACK_COLOR, DOT_PIXEL_1X1, DRAW_FILL_FULL);

  // ���ƿհ׽�����
  Paint_DrawRectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, 0x7BEF, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

  // ������䲿��
  if (filled_width > 0)
  {
    Paint_DrawRectangle(bar_x, bar_y, bar_x + filled_width, bar_y + bar_height, highlight ? bg_color : text_color, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  }

  // ��ʾ��ֵ
  intToStr(*settings[index].value, strBuf, 3);
  Paint_DrawString(LCD_WIDTH - 4 * Font16_Num.Width, bar_y, strBuf, &Font16_Num, MY_THEME_BACK_COLOR, highlight ? bg_color : text_color, '0', 999);

  // ���ưٷֱȷ���

  Paint_DrawChar(LCD_WIDTH - 4 * Font16_Num.Width + 3 * Font16_Num.Width, bar_y, 10, &Font16_Num, MY_THEME_BACK_COLOR, highlight ? bg_color : text_color, 0);
}
u8 needShowBattery = 1;
// ��ʾ����������
void display_settings(sFONT *Font)
{
  if (refreshState)
  {
    // ����
    // Screen_Clear(0, 0, LCD_WIDTH, LCD_HEIGHT, MY_THEME_BACK_COLOR);

    needShowBattery = 1;
    // ���̻�
    Paint_DrawString(0, 0, "234", &Font16_setting, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, '0', 999);

    for (int i = 0; i < SETTING_COUNT; i++)
    {
      draw_setting(i, i == current_setting, Font);
    }
    refreshState = 0;
  }
}

// ��������ҳ��ı������Ͱ����¼�
void handle_setting_event()
{

  if (encode_struct.state == ENCODE_EVENT_UP)
  {                                                               // ���������Ϲ���
    update_current_setting(*settings[current_setting].value + 5); // ÿ������5%
    refreshState = 1;
  }
  else if (encode_struct.state == ENCODE_EVENT_DOWN)
  {                                                               // ���������¹���
    update_current_setting(*settings[current_setting].value - 5); // ÿ�μ���5%
    refreshState = 1;
  }

  if (key.event == KEY_EVENT_CLICK || key.event == KEY_EVENT_LONG_CLICK)
  { // ȷ�ϼ����
    current_setting = (current_setting + 1) % SETTING_COUNT;
    refreshState = 1;
  }
}

// ����ҳ����ʾ
void setting_page(sFONT *Font)
{
  // ��ʾ�����Ϣ
#if BATTERY_ENABLED
  // ʹ��adc.c�е�show_battery����

  show_battery(BATTERY_X, BATTERY_Y, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, &needShowBattery); // ��ʾ�����Ϣ

#endif

  // ��ʾ������
  display_settings(Font);

  // ����������Ͱ����¼�
  handle_setting_event();
}

// ��ҳ����ʾ����
void show_page()
{
  setting_page(&Font16_En);
  key.event = KEY_EVENT_NONE;
  encode_struct.state = ENCODE_EVENT_NONE;
}
