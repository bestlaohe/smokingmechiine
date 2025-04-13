/*
 * time.c
 *
 *  Created on: 2024��9��1��
 *      Author: 12630
 */

#include "time_app.h"
/**
 * @brief   ��ʼ�� TIM1 ��ʱ������������ PWM �����
 *
 * @param   arr   ��ʱ���Զ���װ�ؼĴ��� (ARR) ��ֵ������ PWM �źŵ����ڡ�
 * @param   psc   ��ʱ��Ԥ��Ƶ�� (PSC) ��ֵ��������ʱ������Ƶ�ʡ�
 * @param   ccp   �ȽϼĴ��� (CCR) ��ֵ������ ��ʼ PWM �źŵ�ռ�ձȡ�
 *
 * @note    ���� TIM1 Ϊ PWM ���ģʽ��ʹ�ö��ͨ����
 *          - TIM1_CH1N (PD0) ��������LED����
 *          - TIM1_CH2N (PA2) ���ڷ��ȿ���
 *          - TIM1_CH3 (PC3) ������Ļ�������
 *          TIM1 ������Ϊ���ϼ���ģʽ��PWM ģʽ 2������ߵ�ƽʱ��Ч��
 *          ����ͨ����ʹ����ͬ�� ARR �� PSC ֵ���г�ʼ����
 *
 * @retval  None
 */

// ��ʼ��TIM1��������·PWM���
void TIM1_Init(u16 arr, u16 psc, u16 ccp)
{
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // ʹ��TIM1ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // ����TIM1��������
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // ͨ��PWM���� 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    
    // ����ͨ��1N (����LED - PD0)
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
    
    // ����ͨ��2N (���� - PA2)
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
    
    // ����ͨ��3 (��Ļ���� - PC3)
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; // CH3û�л������
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);

    // ʹ��ARRԤ���ؼĴ���
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    
    // ʹ��TIM1�����
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    
    // ʹ��TIM1
    TIM_Cmd(TIM1, ENABLE);

    // ���ö�ʱ���ж�
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); // ʹ�ܸ����ж�

    // ���� NVIC �ж�
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;        // �ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // ʹ���ж�

    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
}
void TIM1_DeInit(void)
{
    // ���ö�ʱ��
    TIM_Cmd(TIM1, DISABLE);

    // ���ö�ʱ���ж�
    TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);

    // ��λ��ʱ��
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, DISABLE);

    // �����ʱ�����±�־
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
}

// ���ڱ�����
void TIM2_Init(u16 arr, u16 psc)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = psc; // Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Period = arr;    // �Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // �ظ�������ֵ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    // ���� TIM1 ���벶��d3d4
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_2; // �������벶��ͨ��Ϊͨ�� 1 ��ͨ�� 3N
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;      // �������벶��ļ���Ϊ�����ش���
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  // ѡ��ֱ�����루TI��
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;            // �������벶��Ԥ��Ƶ��Ϊ 1������Ƶ��
    TIM_ICInitStructure.TIM_ICFilter = 10;                           // �������벶���˲����Ĳ�������Ϊ 10�������˲�������
    TIM_ICInit(TIM2, &TIM_ICInitStructure);                          // 3��4�����ҽ�����

    // ��������жϱ�־
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);

    TIM_SetCounter(TIM2, 0);

    TIM_Cmd(TIM2, ENABLE);
}
void TIM2_DeInit(void)
{
    // ���ö�ʱ��
    TIM_Cmd(TIM2, DISABLE);

    // �����ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);

    // ��λ��ʱ��
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);

    // ������±�־
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}
/*********************************************************************
 * @fn      IWDG_Feed_Init
 *
 * @brief   ��ʼ�� IWDG���������Ź�����
 *
 * @param   prer: ָ�� IWDG ��Ԥ��Ƶ��ֵ��
 *            IWDG_Prescaler_4: IWDG Ԥ��Ƶ������Ϊ 4��
 *            IWDG_Prescaler_8: IWDG Ԥ��Ƶ������Ϊ 8��
 *            IWDG_Prescaler_16: IWDG Ԥ��Ƶ������Ϊ 16��
 *            IWDG_Prescaler_32: IWDG Ԥ��Ƶ������Ϊ 32��
 *            IWDG_Prescaler_64: IWDG Ԥ��Ƶ������Ϊ 64��
 *            IWDG_Prescaler_128: IWDG Ԥ��Ƶ������Ϊ 128��
 *            IWDG_Prescaler_256: IWDG Ԥ��Ƶ������Ϊ 256��
 *          rlr: ָ�� IWDG ����װ��ֵ��
 *            �ò��������ǽ��� 0 �� 0x0FFF ֮���һ����ֵ��
 *
 * @return  none
 */
void IWDG_Feed_Init(u16 prer, u16 rlr)
{

    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); // ʹ�� IWDG д���ʣ��Ա�������� IWDG �����üĴ���
    IWDG_SetPrescaler(prer);                      // ���� IWDG ��Ԥ��Ƶ��ֵ
    IWDG_SetReload(rlr);                          // ���� IWDG ����װ��ֵ�����ֵ������ IWDG �ĳ�ʱʱ��
    IWDG_ReloadCounter();                         // ���¼��� IWDG ����������ʹ���������µ���װ��ֵ��ʼ
    IWDG_Enable();                                // ���� IWDG��ʹ�俪ʼ����
}

//128khz
// ��EVT����10240��ƵΪ����10240��Ƶ֮���Լ��12.5Hz��
// ���⣬AWU��һ�����Ѵ��ڼĴ����������ô���ֵ����ֵ��6λ�ģ�����������0x3F������63���������õ���25��
// 10240��Ƶ֮����12.5Hz������һ�ε�ʱ��Ϊ1/12.5Hz������ֵ������25������ʱ��Ϊ25/12.5��Լ��2s���ҡ�
// ��Ҫ�Ӵ���ʱ�������ɿ�֪���61440��Ƶ��Ȼ�󴰿�ֵ����Ϊ���63.
void AWU_Init()
{

    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
        ;
    PWR_AWU_SetPrescaler(PWR_AWU_Prescaler_10240);
    PWR_AWU_SetWindowValue(25);
    PWR_AutoWakeUpCmd(ENABLE);
}