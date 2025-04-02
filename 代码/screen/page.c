/*
 * page.c
 *
 *  Created on: 2024��9��21��
 *      Author: 12630
 */
#include "page.h"
#include <string.h>
#include "seting.h"

#define CHAR_HEIGHT 18      // �����ַ��߶�
#define Y_OFFSET 22         // ����ҳ���ʼ��y��ƫ��

u8 current_setting = 0;     // ��ǰ���õ���
u8 refreshState = 1;        // ����ˢ�±�־λ
u8 isFirstSettingShow = 1;  // ����ˢ�±�־
u8 isFirstBattaryShow = 1;  // ���ˢ�±�־

// �������Ļ���Ⱥͷ����ٶ�
typedef enum {
    SETTING_SCREEN_LIGHT = 0,  // ��Ļ����
    SETTING_FAN_SPEED = 1,     // �����ٶ�
    SETTING_LED_LIGHT = 2,     // ����LED����
    SETTING_COUNT = 3          // ��������
} SettingIndex;

// �����ٶ�ֵ
u8 fan_speed = 50;  // Ĭ��50%�ٶ�

// ����LED����ֵ
u8 led_light = 80;  // Ĭ��80%����

// �޸�settings���飬������Ļ���ȡ������ٶȺ�����LED����
Setting settings[SETTING_COUNT] = {
    {"����", (u8 *)&TIM1->CH3CVR, NULL},
    {"����", &fan_speed, NULL},
    {"����", &led_light, NULL}
};

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
    compare_value = percentage;
    
    // ���÷���PWM���
    TIM1->CH2CVR = compare_value;
}

// ����LED���ȿ��ƺ���
void LED_SetLight(uint8_t percentage)
{
    uint16_t compare_value = 0;
    
    // ȷ���ٷֱ���0-100��Χ��
    if (percentage > 100)
        percentage = 100;
    
    // ����PWM�Ƚ�ֵ
    compare_value = percentage;
    
    // ��������LED PWM���
    TIM1->CH1CVR = compare_value;
}

// ���µ�ǰ����ֵ
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

// ����������
void draw_setting(int index, int highlight, sFONT *Font)
{
  char strBuf[4]; // ���ڴ洢���3λ���ֺ�һ����ֹ��
  UWORD bg_color = highlight ? GREEN : MY_THEME_COMPONT_COLOR;
  UWORD text_color;
  
  // Ϊ��ͬ������ѡ��ͬ����ɫ
  switch(index) {
    case SETTING_SCREEN_LIGHT:
      text_color = 0x07FF; // ��ɫ����Ļ����������ɫ
      break;
    case SETTING_FAN_SPEED:
      text_color = 0x07E0; // ��ɫ������������ɫ
      break;
    case SETTING_LED_LIGHT:
      text_color = 0xFFE0; // ��ɫ������������ɫ
      break;
    default:
      text_color = MY_THEME_COMPONT_COLOR;
  }

  Paint_DrawString(0, index * CHAR_HEIGHT + Y_OFFSET, settings[index].name, Font, MY_THEME_BACK_COLOR, highlight ? bg_color : text_color, 'a', 999);
  
  // ��ʾ��ֵ
  intToStr(*settings[index].value, strBuf, 3);
  Paint_DrawString(Font->Width * (strlen(settings[index].name) + 3), index * CHAR_HEIGHT + Y_OFFSET, strBuf, &Font16_Num, MY_THEME_BACK_COLOR, highlight ? bg_color : text_color, '0', 999);
  
  // ���ưٷֱȷ���
  Paint_DrawChar(Font->Width * (strlen(settings[index].name) + 3) + 3 * Font16_Num.Width, index * CHAR_HEIGHT + Y_OFFSET, '%', Font, MY_THEME_BACK_COLOR, highlight ? bg_color : text_color, 0);
  
  // ���ƽ�����
  int bar_width = 88;
  int bar_height = 10;
  int bar_x = 20;
  int bar_y = index * CHAR_HEIGHT + Y_OFFSET + 16;
  int filled_width = bar_width * (*settings[index].value) / 100;
  
  // ���ƿհ׽�����
  Paint_DrawRectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, 0x7BEF, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  
  // ������䲿��
  if (filled_width > 0) {
    Paint_DrawRectangle(bar_x, bar_y, bar_x + filled_width, bar_y + bar_height, highlight ? bg_color : text_color, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  }
}

// ��ʾ����������
void display_settings(sFONT *Font)
{
  if (refreshState)
  {
    // ����
    Screen_Clear(0, 0, LCD_WIDTH, LCD_HEIGHT, MY_THEME_BACK_COLOR);
    
    // ��ʾ����͵����Ϣ
    Paint_DrawString(32, 2, "���̻�", &Font16_En, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, 'a', 999);
    
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
  { // ���������Ϲ���
    update_current_setting(*settings[current_setting].value + 5); // ÿ������5%
    refreshState = 1;
  }
  else if (encode_struct.state == ENCODE_EVENT_DOWN)
  { // ���������¹���
    update_current_setting(*settings[current_setting].value - 5); // ÿ�μ���5%
    refreshState = 1;
  }
  
  if (key.event == KEY_EVENT_CLICK)
  { // ȷ�ϼ����
    current_setting = (current_setting + 1) % SETTING_COUNT;
    refreshState = 1;
  }
}

// ��ʾ�����Ϣ
void show_battery(int x, int y, UWORD back_color, UWORD front_color, u8 *isFirstBattaryShow)
{
#if BATTERY_ENABLED
  if (*isFirstBattaryShow)
  {
    *isFirstBattaryShow = 0;
  }
  
  // �����ذٷֱ�
  float battery_percent = ((float)Battery_ADC_Average / 4095) * 100;
  if (battery_percent > 100)
    battery_percent = 100;
  
  // �����ɫ���ݵ����仯
  UWORD battery_color = (battery_percent < 20) ? RED : 0x07E0; // ��ɫ������20%���
  
  // ���Ƶ��ͼ��Ͱٷֱ�
  char strBuf[4];
  intToStr((int)battery_percent, strBuf, 3);
  Paint_DrawString(x, y, strBuf, &Font12_En, back_color, battery_color, '0', 999);
  Paint_DrawChar(x + 18, y, '%', &Font12_En, back_color, battery_color, 0);
  
  // �����ص�ѹ������ο���ѹΪ3.3V��
  float battery_voltage = 3.3f * (float)Battery_ADC_Average / 4095.0f;
  // ��ӵ�ѹϵ�������ݵ�·��Ƶ�����
  battery_voltage = battery_voltage * 3.03f; // �����ѹ��ѹ��
  
  // ��ʾ��ص�ѹ
  char voltBuf[5];
  sprintf(voltBuf, "%.1fV", battery_voltage);
  Paint_DrawString(x, y + 15, voltBuf, &Font12_En, back_color, battery_color, '0', 999);
#endif
}

// ����ҳ����ʾ
void setting_page(sFONT *Font)
{
  // ��ʾ�����Ϣ
#if BATTERY_ENABLED
  show_battery(90, 2, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, &isFirstBattaryShow);
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

  // �������¼�������¼�
  key.event = KEY_EVENT_NONE;
  encode_struct.state = ENCODE_EVENT_NONE;
}
