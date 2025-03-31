/*
 * page.h
 *
 *  Created on: 2024��9��21��
 *      Author: 12630
 */

#ifndef SCREEN_PAGE_H_
#define SCREEN_PAGE_H_
#include "debug.h"
#include "encode.h"
#include "adc.h"
#include "seting.h"

// #include "SX1278.h"

extern Key key;

extern Encode encode_struct;
extern volatile u8 lora_receive_len;
extern volatile u8 lora_receive_flag;

extern char lora_receive_buf[145];
extern u16 Battery_ADC_Average;

#define ON 1
#define OFF 0

// Lora������Χ����
#define LORAFREQ_MIN 1
#define LORAFREQ_MAX 11
#define LORAPOWER_MIN 11
#define LORAPOWER_MAX 20
#define LORASpreadFactor_MIN 7
#define LORASpreadFactor_MAX 12
#define LORABANDWIDTH_MIN 0
#define LORABANDWIDTH_MAX 9

// �������ò���
typedef enum
{
  SETTING_SCREEN_LIGHT,
  SETTING_SHAKE_MODE,
  SETTING_LORA_FREQ,
  SETTING_LORA_POWER,
  SETTING_LORA_BANDWIDTH,
  SETTING_LORA_SPREAD_FACTOR,
  SETTING_COUNT
} SettingIndex;

extern u8 shake_mode;        // ��ģʽ��0Ϊoff��1Ϊon
extern u8 Lora_Freq;         // Ĭ��Ƶ������
extern u8 Lora_Power;        // �����������
extern u8 Lora_BandWide;     // ��������
extern u8 Lora_SpreadFactor; // ��Ƶ����������7~12= 7

typedef struct
{
  const char *name;
  u8 *value;
  void (*update_func)(void);
} Setting;
// ���尴���¼�
typedef enum
{
  PAGE_SEND, // ����ҳ��
  PAGE_PERPARE_SETTING, // ׼������ҳ��
  PAGE_HISTROY_CHAT,

  PAGE_SETTING, // ����ҳ��
  PAGE_INFO,    // ��Ϣҳ��
} Page;

void chat_page(sFONT *Font);
void show_history_data(sFONT *Font);
void chat_history_page(sFONT *Font);
void perpare_setting_page(sFONT *Font);
void setting_page(sFONT *Font);
void show_page();
void info_page();
#endif /* SCREEN_PAGE_H_ */
