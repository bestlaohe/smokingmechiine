#include "debug.h"
#include "encode.h" // timeapp->encode->main
#include "adc.h"    // timeapp->pwm.screen->Screen_api->adc->main
#include "page.h"
#include "seting.h"

// ����������ͣ�˯�߹������
// 16k=16,384
// data�Ǳ�ʾ����Σ�text segment���Ĵ�С�����ֽ�Ϊ��λ������ΰ����˳���Ŀ�ִ��ָ�
// bss�Ǵ洢��Щ��Դ������û����ʽ��ʼ����ȫ�ֱ����;�̬������
// data����ʽ��ʼ����ȫ�ֱ����;�̬����

//  ROM��
//  text �Σ��洢������롣
//  data �Σ��洢�ѳ�ʼ����ȫ�ֱ����;�̬�����ĳ�ʼֵ��

//  RAM��
//  text �Σ�������ʱ���ڴ洢����ִ�У���ֱ��ռ�� RAM �Ĵ洢�ռ䣬����Ҫ RAM ��ִ�У���
//  data �Σ�������ʱ�洢�ѳ�ʼ����ȫ�ֱ����;�̬������
//  bss �Σ�������ʱ�洢δ��ʼ����ȫ�ֱ����;�̬������

// 2+16k

int main(void)
{

  /*********************�������ݳ�ʼ��******************************/
  SystemCoreClockUpdate(); // 48000000ϵͳʱ��ˢ��3324-3212=100k
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  USART_Printf_Init(115200); // ���ڳ�ʼ����Ҫ�ڴ�ӡǰ����Ȼ�Ῠ��3956-3324=600k
  DEBUG_PRINT("\r\nSystemClk:%d\r\n", SystemCoreClock);
  //  DEBUG_PRINT("ChipID:%08x\r\n", DBGMCU_GetCHIPID());
  Delay_Init(); // ��ʱ��ʼ����Ҫ����ʱǰ����Ȼ�Ῠ��4012-3956=100�ֽ�

  // �״β�Ҫ
  //  FLASH_Unlock();
  //  FLASH_UserOptionByteConfig(OB_IWDG_SW, OB_STDBY_NoRST, OB_RST_NoEN, OB_PowerON_Start_Mode_BOOT);
  //  FLASH_Lock(); //**4232-4012=200�ֽ�

  /*********************Ӧ�ú�����ʼ��******************************/
#if DEBUG_ENABLED != 0
  Check_Reset_Flag(); // ��ѯ��λԭ��
#endif
  My_GPIO_Init();                                                    // IO�ڳ�ʼ��****4484-4232=252�ֽ�

#if ENCODER_ENABLED
  TIM2_Init(11, 1); // ������������,����ֵΪ65535������Ƶ��1Ȧ12��****6020-6900=880�ֽ����벶��Ҫ500��+��ʱ��300
#endif

  // ��ʼ��TIM1������Ĭ��ֵ
  // ������Ԥװ��ֵarr(100), Ԥ��Ƶ��psc, ��ʼռ�ձ�duty(0-100)
  TIM1_Init(100, (SystemCoreClock / (100 * PWM_FRE)) - 1, PWM_Duty);
  
  // ���ó�ʼ�����ٶ�Ϊ50%
  FAN_SetSpeed(fan_speed);
  
  // ���ó�ʼ����LED����Ϊ80%
  LED_SetLight(led_light);

#if SCREEN_ENABLED
  LCD_Drive_Init();    // ��ĻӲ����ʼ��****200�ֽ�
  LCD_SHOW_API_INIT(); // ��Ļ����******8404-6224=2180
#endif

#if BATTERY_ENABLED
  Battery_Init(); // ��ص�adc��ʼ��****9456-8636=820
#endif

  EXTI_INT_INIT(); // ����������жϳ�ʼ��
  //   startup_animation();                                             // ��������

  IWDG_Feed_Init(IWDG_Prescaler_256, 4000); // �ò��������ǽ��� 0 �� 0x0FFF ֮���һ����ֵ    // 4�벻ι���͸�λ   ��Ƶʱ���ڲ�128khz����256=500,1����500����4000=8s****12467-12356=111�ֽ�

#if SLEEP == 1
  AWU_Init(); // ����ʱ��Ϊ25/12.5��Լ��2s���ҡ�
#endif
  while (1)
  {

    if (needSleep)
    {
#if SLEEP == 1
      IWDG_ReloadCounter(); // ι��
      Sleep_Scan();         // ����Ƿ�˯��1
#endif
    }
    else
    {
      show_page();

#if ENCODER_ENABLED
      Encoder_Scan();
#endif

      IWDG_ReloadCounter(); // ι��

#if SLEEP == 1
      Sleep_Scan(); // ����Ƿ�˯��
#endif
      //      Delay_Ms(1000);
    }
  }
}
