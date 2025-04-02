/*
 * page.h
 *
 *  Created on: 2024��9��21��
 *      Author: 12630
 */

#ifndef PAGE_H_
#define PAGE_H_

#include "screen_api.h"
#include "fonts.h"
#include <stdio.h>
#include "ch32v00x.h"
#include "seting.h"

// ����ҳ������
typedef enum {
    PAGE_SETTING = 0,  // ����ҳ�棬��Ϊֻ��һ��ҳ��
} Page;

// ���尴ť״̬,�ɿ��ص�ѡ��
typedef enum {
    OFF = 0,
    ON = 1,
} BUTTON_STATE;

// ����ѡ������
typedef enum {
    SETTING_SCREEN_LIGHT = 0,  // ��Ļ����
    SETTING_FAN_SPEED = 1,     // �����ٶ�
    SETTING_LED_LIGHT = 2,     // ����LED����
    SETTING_COUNT = 3          // ��������
} SettingIndex;

// ���ýṹ�嶨��
typedef struct {
    char *name;            // ��������
    u8 *value;            // ����ֵ��ָ��
    void (*update_func)(void); // ���º���ָ��
} Setting;

// ��������
extern u8 current_setting;               // ��ǰ������
extern u8 refreshState;                 // ˢ�±�־
extern u8 isFirstSettingShow;           // �״�������ʾ��־
extern u8 isFirstBattaryShow;           // �״ε����ʾ��־
extern Page page;                       // ��ǰҳ��
extern Setting settings[SETTING_COUNT]; // ��������
extern u8 fan_speed;                    // �����ٶ�ֵ
extern u8 led_light;                    // ����LED����ֵ

// ��������
void show_page(void);
void setting_page(sFONT *Font);
void update_current_setting(int value);
void clamp_value(int *value, int min, int max);
void FAN_SetSpeed(uint8_t percentage);
void LED_SetLight(uint8_t percentage);
void show_battery(int x, int y, UWORD back_color, UWORD front_color, u8 *isFirstBattaryShow);

#endif /* PAGE_H_ */
