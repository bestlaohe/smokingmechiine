/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v00x_it.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Main Interrupt Service Routines.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#include <ch32v00x_it.h>
#include "adc.h"
#include "seting.h"
// #include <core_riscv.h>
void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI7_0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast"))); // ��Ϊ����ʱ10msһ�δ����ж�
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void AWU_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

volatile u8 dmaTransferComplete = 0;
volatile u8 needSleep = 0;
volatile u8 needDeinit = 0;

volatile int circle = 0;
int SleepCounter = 0;

// ɾ���������ر�־λ
Encode encode_struct = {ENCODE_EVENT_NONE, 0};
Key key = {KEY_STATE_IDLE, KEY_EVENT_NONE, 0, 0, 1};
Charge charge = {UNCHARGING};

void TIM2_IRQHandler()
{

  volatile uint16_t tempcnt = TIM2->CNT, temparr = TIM2->ATRLR;

  if (TIM_GetITStatus(TIM2, TIM_IT_Update))
  {

    //     DEBUG_PRINT("��ǰ����=%d\r\n", tempcnt);
    //     DEBUG_PRINT("��װ��ֵ=%d\r\n", temparr);

    if (tempcnt < temparr / 2)
    {
      circle += 1;
    }
    else
    {
      circle -= 1;
    }
  }
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
void refresh_SleepCounter(int newtime)
{
  SleepCounter = newtime;
  // DEBUG_PRINT("refresh_SleepCounter\r\n");
}

void DMA1_Channel3_IRQHandler(void)
{
  if (DMA_GetITStatus(DMA1_IT_TC3))
  {
    //   DEBUG_PRINT("dmaTransferComplete\r\n");
    dmaTransferComplete = 1;
    DMA_ClearITPendingBit(DMA1_IT_TC3); // ����жϱ�־
  }
}
u16 Battery_ADC_Average = 0;
u16 pre_Battery_ADC_Average = 0;
void DMA1_Channel1_IRQHandler(void)
{

  if (DMA_GetITStatus(DMA1_IT_TC1))
  {

    Battery_ADC_Average = 0;
    // ������ƽ��ֵ
    for (u8 i = 0; i < ADC_CONUT; i++)
    {
      Battery_ADC_Average += BattaryBuf[i];
      //        DEBUG_PRINT("BattaryBuf[i]=%d\r\n",BattaryBuf[i]);
    }
    Battery_ADC_Average /= ADC_CONUT; // ��ƽ��ֵ

    if (abs(pre_Battery_ADC_Average - Battery_ADC_Average) < 10)
    {
      Battery_ADC_Average = pre_Battery_ADC_Average;
    }
    else
    {
      pre_Battery_ADC_Average = Battery_ADC_Average;
    }
    //  DEBUG_PRINT("Battery_ADC_Average=%d\r\n",Battery_ADC_Average);

    DMA_ClearITPendingBit(DMA1_IT_TC1); // ����жϱ�־
  }
}
/*********************************************************************
 * @fn      EXTI2_IRQHandler
 *
 * @brief   This function handles EXTI2 Handler.
 *
 * @return  none
 */
void EXTI7_0_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line2) != RESET)
  {
    // ɾ������ﴥ������

    EXTI_ClearITPendingBit(EXTI_Line2); /* Clear Flag */

    if (!KEY0)
    {
      key.state = KEY_STATE_PRESS;
      DEBUG_PRINT("start press\r\n");
    }
    else
    {
      DEBUG_PRINT("end press\r\n");
      if (key.LongKeyCounter <= HOLD_TIME * (PWM_FRE / 10000))
      {
        key.event = KEY_EVENT_CLICK;
        DEBUG_PRINT("KEY_EVENT_CLICK ontime\r\n");
      }
      else
      {
        key.event = KEY_EVENT_LONG_CLICK;
        DEBUG_PRINT("KEY_EVENT_LONG_CLICK ontime\r\n");
        key.LongKeyCounter = 0;
        if (key.enable == 0)
        {
          key.event = KEY_EVENT_NONE;
          key.state = KEY_STATE_IDLE;
          key.LongKeyCounter = 0;
          key.enable = 1;
          DEBUG_PRINT("disable key operate\r\n");
        }
      }
      if (key.enable == 0)
      {
        key.event = KEY_EVENT_NONE;
        key.state = KEY_STATE_IDLE;
        key.LongKeyCounter = 0;
        key.enable = 1;
        DEBUG_PRINT("disable key operate\r\n");
      }
      key.state = KEY_STATE_RELEASE;
    }
    key.LongKeyCounter = 0;

    system_wokeup(); // ϵͳ����
  }

  if (EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    // ɾ������ﴥ������

    if (!CHARGE)
    {
      charge.state = CHARGING;
      DEBUG_PRINT("start chage\r\n");
      charge.needShowBattery = 1;
    }
    else
    {
      charge.state = UNCHARGING;
      DEBUG_PRINT("end chage\r\n");
       charge.needShowBattery = 1;
    }

    system_wokeup();                    // ϵͳ����
    EXTI_ClearITPendingBit(EXTI_Line1); /* Clear Flag */
  }
}
/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{ // ���� NMI �쳣�Ĵ���
  // ���磬��¼������Ϣ�򴥷�ϵͳ��ȫ����
  DEBUG_PRINT("start NMI_Handler\r\n");
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
// �������ǲ����������ж�,����hardfault,�������˵�mcause��Ӧ����Load address misaligned
// Interrupt	Exception Code	Description
// 1	0	User software interrupt
// 1	1	Supervisor software interrupt
// 1	2	Hypervisor software interrupt
// 1	3	Machine software interrupt
// 1	4	User timer interrupt
// 1	5	Supervisor timer interrupt
// 1	6	Hypervisor timer interrupt
// 1	7	Machine timer interrupt
// 1	8	User external interrupt
// 1	9	Supervisor external interrupt
// 1	10	Hypervisor external interrupt
// 1	11	Machine external interrupt
// 1	��12	Reserved
// 0	0	Instruction address misaligned
// 0	1	Instruction access fault
// 0	2	Illegal instruction
// 0	3	Breakpoint
// 0	4	Load address misaligned
// 0	5	Load access fault
// 0	6	Store/AMO address misaligned
// 0	7	Store/AMO access fault
// 0	8	Environment call from U-mode
// 0	9	Environment call from S-mode
// 0	10	Environment call from H-mode
// 0	11	Environment call from M-mode
// 0	��12	Reserved

// ������ַmtval
void HardFault_Handler(void)
{
  // ���� HardFault �쳣�Ĵ���
  // ���磬��¼������Ϣ�����Իָ�ϵͳ������ϵͳ
  DEBUG_PRINT("start HardFault_Handler\r\n");

  //  uint32_t v_mepc, v_mcause;
  //
  //  v_mepc = __get_MEPC();
  //  v_mcause = __get_MCAUSE();
  //
  //
  //  printf("mepc:%08x\n", v_mepc);
  //  printf("mcause:%08x\n", v_mcause);

  while (1)
    ;
}

/*********************************************************************
 * @fn      EXTI0_INT_INIT
 *
 * @brief   Initializes EXTI0 collection.
 *
 * @return  none
 */
void EXTI_INT_INIT(void)
{
  EXTI_InitTypeDef EXTI_InitStructure = {0};
  NVIC_InitTypeDef NVIC_InitStructure = {0};

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  // GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource6|GPIO_PinSource2|GPIO_PinSource7);

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource6);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);

  EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line1; // key��charge
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  EXTI_InitStructure.EXTI_Line = EXTI_Line9; // AWU
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = AWU_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI7_0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}
void system_wokeup()
{

  refresh_SleepCounter(0); // ˢ������ʱ��

  if (needSleep) // �Ѿ�������
  {
    //    My_GPIO_Init();                                                    // IO�ڳ�ʼ��****4484-4232=252�ֽ�
    TIM1_Init(100, (SystemCoreClock / (100 * PWM_FRE)) - 1, PWM_Duty); // ��Ļ�ı������  Ĭ�ϰٷְ�����******5076-4484=592�ֽ�pwmҪ200��+��ʱ��300

#if ENCODER_ENABLED
    TIM2_Init(11, 1); // ������������,����ֵΪ65535������Ƶ��1Ȧ12��****6020-6900=880�ֽ����벶��Ҫ500��+��ʱ��300
#endif

#if SCREEN_ENABLED
    LCD_Drive_Init(); // ��ĻӲ����ʼ��****200�ֽ�
#endif

#if BATTERY_ENABLED
    Battery_Init(); // ��ص�adc��ʼ��****9456-8636=820
#endif

    //  EXTI_INT_INIT();                                                   // ��������磬lora�жϳ�ʼ��
    USART_Printf_Init(115200);

    DEBUG_PRINT("system_wokeup\r\n");

    // �������¼�������¼�
    key.enable = 0;
    key.event = KEY_EVENT_NONE;
    encode_struct.state = ENCODE_EVENT_NONE;
    needSleep = 0;
  }
}

void system_enter_sleep()
{

  if (needDeinit)
  {
    DEBUG_PRINT("system_Deinit\r\n");
    // My_GPIO_DeInit();//���Ѳ��˴򿪵Ļ�

#if SCREEN_ENABLED
    LCD_Drive_DeInit();
#endif

#if BATTERY_ENABLED
    Battery_DeInit();
#endif

    TIM1_DeInit();

#if ENCODER_ENABLED
    TIM2_DeInit(); // ��������
#endif

    USART_DeInit(USART1);
    needDeinit = 0;
  }
}

void Sleep_Scan()
{
  if (needSleep)
  {
    system_enter_sleep();
    PWR_EnterSTANDBYMode(PWR_STANDBYEntry_WFI);
  }
}

// ��ʱ���жϷ�����10ms  100us
void TIM1_UP_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
  {
    // ����жϱ�־
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

#if SLEEP == 1
    SleepCounter++;
    if (SleepCounter >= SLEEP_TIME * (PWM_FRE / 10000))
    {
      SleepCounter = 0;
      needSleep = 1;
      needDeinit = 1;
      DEBUG_PRINT("EnterSTANDBYMode\r\n");
    }
#endif

    if (!KEY0)
    {
      key.LongKeyCounter++;
      if (key.LongKeyCounter >= DEBOUNCE_TIME * (PWM_FRE / 10000)) // ����
        key.state = KEY_STATE_HOLD;
    }

    // ɾ������ﶨʱ�������
  }
}

/*********************************************************************
 * @fn      EXTI0_IRQHandler
 *
 * @brief   This function handles EXTI0 Handler.
 *
 * @return  none
 */
void AWU_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line9) != RESET)
  {

#if SLEEP == 1
    DEBUG_PRINT("AWU Wake_up=%d\r\n", SleepCounter);
#endif
    EXTI_ClearITPendingBit(EXTI_Line9); /* Clear Flag */
  }
}

// ɾ�� process_motor_flags ����
