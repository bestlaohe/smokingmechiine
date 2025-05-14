/*
 * adc.c
 *
 *  Created on: 2024��9��7��
 *      Author: 12630
 */

#include "adc.h"
#include "seting.h"


u16 BattaryBuf[ADC_CONUT];
// �ؼ���� ADC ֵ�Ͷ�Ӧ�ĵ�ذٷֱ�
#define NUM_POINTS 5
const uint16_t adc_points[NUM_POINTS] ={421, 477, 558, 609, 645}; // ʾ���ؼ���
const uint8_t percent_points[NUM_POINTS] = {0, 25, 50, 75, 100};   // ��Ӧ�İٷֱ�


//percentage=41 578
// percentage=32 562
// 4.2V �� 100%
// 3.6V �� 50%
// 3.0V �� 10%
// 2.7V �� 0%



// ���ұ�Ĵ�С
#define ADC_MAX 1023

// ������ʹ�����Բ�ֵ��ȡ��ذٷֱ�
uint8_t get_battery_percentage(uint16_t adc_value)
{
    if (adc_value >= adc_points[NUM_POINTS - 1])
    {
        return percent_points[NUM_POINTS - 1];
    }

    // ���ҵ�֮��Ĳ�ֵ
    for (u8 i = 0; i < NUM_POINTS - 1; i++)
    {
        if (adc_value >= adc_points[i] && adc_value < adc_points[i + 1])
        {
            // ���Բ�ֵ��ʽ
            uint16_t range = adc_points[i + 1] - adc_points[i];
            uint16_t delta = adc_value - adc_points[i];
            uint8_t percentage_range = percent_points[i + 1] - percent_points[i];
            uint8_t percentage = percent_points[i] + (delta * percentage_range) / range;
            if (percentage > 100)
                percentage = 100;
            return percentage;
        }
    }

    // ��������κ������ڣ�������Сֵ
    return percent_points[0];
}

/*********************************************************************
 * @fn      Battery_Init
 *
 * @brief   Initializes ADC collection.
 *
 * @return  none
 */
void Battery_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // ����adc1

    RCC_ADCCLKConfig(RCC_PCLK2_Div8); // ����ADCʱ��ΪPCLK2��1/8

    ADC_DeInit(ADC1); // ��λADC1�Ĵ�����Ĭ��״̬

    // ��ʼ��ADC���ýṹ��
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // ����ADCΪ����ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       // ����ɨ��ת��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // ��������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // ���ⲿ����
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // �����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             // ����ת��ͨ������Ϊ1
    ADC_Init(ADC1, &ADC_InitStructure);                                 // ��ʼ��ADC1��Ӧ�����ýṹ��

    ADC_Calibration_Vol(ADC1, ADC_CALVOL_50PERCENT); // У׼ADC��ʹ��50%�ο���ѹ
    ADC_DMACmd(ADC1, ENABLE);                        // ����DMA��֧��ADC���ݴ���
    ADC_Cmd(ADC1, ENABLE);                           // ����ADC
    ADC_ResetCalibration(ADC1);                      // ����ADCУ׼
    while (ADC_GetResetCalibrationStatus(ADC1))
        ;                       // �ȴ�У׼������
    ADC_StartCalibration(ADC1); // ����ADCУ׼
    while (ADC_GetCalibrationStatus(ADC1))
        ; // �ȴ�У׼���

    DMA_Tx_Init(DMA1_Channel1, (u32)&ADC1->RDATAR, (u32)BattaryBuf, ADC_CONUT); // ������һֱ�������buf����
    DMA_Cmd(DMA1_Channel1, ENABLE);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_241Cycles);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

  
}
void Battery_DeInit(void)
{
    // ֹͣADCת��
    ADC_SoftwareStartConvCmd(ADC1, DISABLE); // �����������ת��

    // ����DMA
    DMA_Cmd(DMA1_Channel1, DISABLE); // ���� DMA ͨ��
    DMA_DeInit(DMA1_Channel1);       // ��ѡ�����ϣ������DMA����

    // ����ADC
    ADC_Cmd(ADC1, DISABLE); // ���� ADC1

    // ����ADCʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE); // ���� ADC1 ʱ��
}

/*********************************************************************
 * @fn      Get_ADC_Val
 *
 * @brief   Returns ADCx conversion result data.
 *
 * @param   ch - ADC channel.
 *            ADC_Channel_0 - ADC Channel0 selected.
 *            ADC_Channel_1 - ADC Channel1 selected.
 *            ADC_Channel_2 - ADC Channel2 selected.
 *            ADC_Channel_3 - ADC Channel3 selected.
 *            ADC_Channel_4 - ADC Channel4 selected.
 *            ADC_Channel_5 - ADC Channel5 selected.
 *            ADC_Channel_6 - ADC Channel6 selected.
 *            ADC_Channel_7 - ADC Channel7 selected.
 *            ADC_Channel_8 - ADC Channel8 selected.
 *            ADC_Channel_9 - ADC Channel9 selected.
 *
 * @return  none
 */
u16 Get_ADC_Val(u8 ch)
{
    u16 val;

    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_241Cycles);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
        ;
    val = ADC_GetConversionValue(ADC1);

    return val;
}

/*********************************************************************
 * @fn      DMA_Tx_Init
 *
 * @brief   Initializes the DMAy Channelx configuration.
 *
 * @param   DMA_CHx - x can be 1 to 7.
 *          ppadr - Peripheral base address.
 *          memadr - Memory base address.
 *          bufsize - DMA channel buffer size.
 *
 * @return  none
 */
void DMA_Tx_Init(DMA_Channel_TypeDef *DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize)
{
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);                                                        // ȡ����ʼ��ָ����DMAͨ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;                           // �����������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr;                              // �����ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          // ���ݴ��䷽�򣺴����赽�ڴ�
    DMA_InitStructure.DMA_BufferSize = bufsize;                                 // ���ô������ݵĻ�������С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // �����ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // �ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // �������ݴ�С������
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // �ڴ����ݴ�С������
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // DMA����ģʽ��ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                     // �������ȼ����ǳ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // �����ڴ浽�ڴ�ģʽ
    DMA_Init(DMA_CHx, &DMA_InitStructure);                                      // ��ʼ��ָ����DMAͨ��
    DMA_ITConfig(DMA_CHx, DMA_IT_TC, ENABLE);                                   // ʹ��DMA��������ж�
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); // ����NVIC
}

void show_battery(UWORD Xpoint, UWORD Ypoint, UWORD Color_Background, UWORD Color_Foreground)
{

    static u8 percentage = 0;
    static u8 Prepercentage = 101;
    char strBuf[4]; // Ҫ�洢���3λ���ֺ�һ����ֹ�������������СΪ4

    percentage = get_battery_percentage(Battery_ADC_Average);
//   DEBUG_PRINT("percentage=%d %d\r\n",percentage,Battery_ADC_Average);
    // percentage = 100;

    // ���ưٷֱȷ�Χ
    if (percentage > 100)
        percentage = 100;

    if(abs(Prepercentage - percentage) > 5 || charge.needShowBattery )
    {
        charge.needShowBattery = 0;
        Prepercentage = percentage;

        u8 cnt = percentage / 25;
        u16 strX = Xpoint - 2 * Font16_Num.Width; // Ĭ�� 2 λ������ʾλ��
        u16 color = BLUE;                         // Ĭ����ɫ

        if (percentage >= 100)
        {
            strX = Xpoint - 3 * Font16_Num.Width; // 3 λ������ʾλ��
            color = GREEN;                        // ��ɫ

            intToStr(percentage, strBuf, 3);
        }
        else if (percentage < 10)
        {

            Paint_DrawString(Xpoint - Font16_Num.Width * 3, Ypoint, "<<", &Font16_Num, Color_Background, Color_Background, '0',999);
            strX = Xpoint - Font16_Num.Width; // 1 λ������ʾλ��
            color = RED;                      // ��ɫ

            intToStr(percentage, strBuf, 1);
        }
        else
        {
            Paint_DrawString(Xpoint - Font16_Num.Width * 3, Ypoint, "<", &Font16_Num, Color_Background, Color_Background, '0',999);

            intToStr(percentage, strBuf, 2);
        }

        // ���ְٷֱ�
        Paint_DrawString(strX, Ypoint, strBuf, &Font16_Num, Color_Background, Color_Foreground, '0',999);
        Paint_DrawChar(Xpoint, Ypoint, 10, &Font16_Num, Color_Background, Color_Foreground, 0); //%

        if (charge.state == CHARGING)
        {
            Paint_DrawChar(Xpoint + Font16_Num.Width, Ypoint, 1, &Font16_Bat, Color_Background, color, 0); // ���log
      
        }
        else if (charge.state == UNCHARGING)
        {
            Paint_DrawChar(Xpoint + Font16_Num.Width, Ypoint, 0, &Font16_Bat, Color_Background, color, 0); // �յ�log
            for (u8 i = 0; i < cnt; i++)
            {
                Paint_DrawLine(108 + 4 + i * 3, 3 + Ypoint, 108 + 4 + i * 3, 7 + Ypoint, color, 1, LINE_STYLE_SOLID);
            }
   
        }
    }
}
