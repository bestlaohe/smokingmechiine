/*
 * seting.h
 *
 *  Created on: 2024��11��4��
 *      Author: heyj
 */

#ifndef USER_SETING_H_
#define USER_SETING_H_

#define THEME 1

#if THEME == 1 // ��ɫ����

#define MY_THEME_BACK_COLOR BLACK    // ������ɫ
#define MY_THEME_COMPONT_COLOR WHITE // �����ɫ
#define MY_SCREEN_COLOR MY_GRAY      // Ļ����ɫ


#else // ��ɫ����

#define MY_THEME_BACK_COLOR WHITE     // ������ɫ
#define MY_THEME_COMPONT_COLOR BLACK  // �����ɫ
#define MY_SCREEN_COLOR MY_WHITE_GRAY // Ļ����ɫ

#endif

// ����������Ŀ��
#define Font_WIDTH 7  // ������
#define Font_HEIGH 12 // ����߶�
#define BATTERY_X 97  // ���x����
#define BATTERY_Y 3   // ���y����

// PWM�������
#define PWM_FRE         150000   // PWMƵ��(Hz)�����ڷ��ȿ��ƺ�����LED
#define PWM_Duty        20      // ��ʼռ�ձ�(0-100%)

// PWM���Ŷ���:
// TIM1_CH1N (PD0): ����LED����
// TIM1_CH2N (PA2): ���ȿ���
// TIM1_CH3 (PC3): ��Ļ�������

#define ADC_CONUT       15      // �������64����Ϊu16/1024=64

// ȥ�����ͳ������ĳ���
#define DEBOUNCE_TIME 50        // ȥ����ʱ�䣬��λ��ms
#define HOLD_TIME 3000          // ����ʱ�䣬��λ��100us

#define SLEEP_TIME 30*10000     // ��Լ30s����һ��˯��

// ������رյ�����Ϣ���
#define DEBUG_ENABLED 0

#if DEBUG_ENABLED == 2
    #define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
    #define SLEEP 0  //�Ƿ�Ҫ����//ǧ��ǵ����ߵ�ʱ�򴮿ڱ�����ʼ���ˣ���Ҫ��ӡ��������Ȼ�Ῠ��

#elif DEBUG_ENABLED == 1
    #define DEBUG_PRINT(fmt, ...) my_uart_print(fmt)
    #define SLEEP 0   //�Ƿ�Ҫ����//ǧ��ǵ����ߵ�ʱ�򴮿ڱ�����ʼ���ˣ���Ҫ��ӡ��������Ȼ�Ῠ��
  
#else
    #define DEBUG_PRINT(fmt, ...)   my_uart_print(fmt)
    #define SLEEP 1   //�Ƿ�Ҫ����

#endif

// struct Flags {
//     unsigned int refreshState : 1;
//     unsigned int flag2 : 1;
//     unsigned int flag3 : 1;
//     unsigned int flag4 : 1;
//     unsigned int flag5 : 1;
//     unsigned int flag6 : 1;
//     unsigned int flag7 : 1;
//     unsigned int flag8 : 1;
// };
// struct Flags myFlags = {0};

// ���ʹ������
// 1��ʾ���ã�0��ʾ����
#define LORA_ENABLED     0  // LoRa���ʹ��
#define SCREEN_ENABLED   1  // ��Ļ���ʹ��
#define ENCODER_ENABLED  1  // ���������ʹ��
#define BATTERY_ENABLED  0  // ������ʹ��

#endif /* USER_SETING_H_ */
