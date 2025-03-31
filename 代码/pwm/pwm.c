/*
 * pwm.c
 *
 *  Created on: 2024��8��30��
 *      Author: 12630
 */
/*
 *@Note
 PWM output routine:
 TIM1_CH3 ��PC3��
    ������
*/
#include "pwm.h"



void PWM_Config(u16 Freq, u16 Duty)//(10000, 100);
{
    // ȷ��Ƶ���ں���Χ��
    if (Freq == 0) return; // ���������
    if (Duty > 100) Duty = 100; // ȷ��ռ�ձȲ�����100%

    // ����Ԥ��Ƶ�����Զ����ؼĴ���ֵ
    u16 psc = (SystemCoreClock / (100*Freq)) - 1; // ����Ԥ��Ƶ��24

    // ���ó�ʼ������
    TIM1_Init(100, psc, Duty);
}



