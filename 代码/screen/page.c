/*
 * page.c
 *
 *  Created on: 2024��9��21��
 *      Author: 12630
 */
#include "page.h"
#include <string.h>
#include "seting.h"
#define MAX_LINE_TO_SHOW 5                                                             // �����ʾ����
#define EDGE 1                                                                         // ��Ե
#define OPERATE_DOWN 20                                                                // ���������±�Ե
#define CHAT_DOWN 122                                                                  // ������±�Ե
#define COLUMN ((LCD_WIDTH - EDGE - EDGE) / Font_WIDTH)                                // �и���18
#define CHAT_HISTORY_DOWN (MAX_LINE_TO_SHOW * Font_HEIGH) + OPERATE_DOWN + EDGE + EDGE // �����¼�±�Ե5��90/18=5*12=60
#define CHAT_UP CHAT_HISTORY_DOWN + 2                                                  // ���������ϲ���
#define CHAR_HEIGHT 18                                                                 // �����ַ��߶�
#define Y_OFFSET 22                                                                    // ����ҳ���ʼ��y��ƫ��
int8_t Englishcount = 0;                                                               // �ַ���λ��
int8_t Englishposx = 0;                                                                // x�ĸ���
int8_t Englishposy = 0;                                                                // y�ĸ���
// u8 lora_send_buf[100];                                                                 // ֻ��3�п�������һ��18
u8 current_setting = 0;    // ��ǰ���õ���
u8 refreshState = 1;       // ����ˢ�±�־λ
u8 isFirstSettingShow = 1; // ����ˢ�±�־
u8 isFirstBattaryShow = 1; // ���ˢ�±�־
u8 current_line = 0;       // ��ǰ��ʾ����ʼ��
u8 total_lines = 0;        // �ܵ������¼����

Setting settings[SETTING_COUNT] = {
    {"light", (u8 *)&TIM1->CH3CVR, NULL},
    {"shake", &shake_mode, NULL},
#if LORA_ENABLED
    {"freq", &Lora_Freq, SX1278_Init},
    {"power", &Lora_Power, SX1278_Init},
    {"lorabw", &Lora_BandWide, SX1278_Init},
    {"lorasf", &Lora_SpreadFactor, SX1278_Init}
#endif
};

Page page = PAGE_SEND;
u8 send_wait_time = 0; // �����״�250���Լ�1����һ�λᷢ��

#if LORA_ENABLED
void handle_lora_msg(sFONT *Font)
{
  // DEBUG_PRINT("lora_receive_flag=%d\r\n",lora_receive_flag);
  if (lora_receive_flag == 2)
  {
    send_wait_time++;
  }
  if (lora_receive_flag == 2 && send_wait_time > 200) // ���޵ȴ����ջ�Ӧ
  {
    DEBUG_PRINT("lora send start\r\n");
    send_wait_time = 0;
    if (!SX1278_LoRaTxPacket(lora_receive_buf, strlen(lora_receive_buf)))//����߼�ûë�����û�a���ܿ���b������λ�ã�̫ɵ��������
    {
      DEBUG_PRINT("lora send ok\r\n");
      lora_receive_flag = 1; // �����Ƿ���Ҫ��Ӧ
    }
    else
    {
      DEBUG_PRINT("lora send fail\r\n");
    }
    SX1278_LoRaEntryRx(); // �������ģʽ
  }
  if (lora_receive_flag == 1) // ���յ���Ӧ��
  {
    lora_receive_flag = 0;
    DEBUG_PRINT("lora get rsp ok fig\r\n");
    // Englishposy = 0;  // ������λ��
    // Englishposx = 0;  // ������λ��
    // Englishcount = 0; // ���һ��
    // memset(lora_send_buf, '\0', sizeof(lora_send_buf));
    // Screen_Clear(EDGE, CHAT_UP + EDGE, 127 - EDGE, 127 - EDGE, MY_SCREEN_COLOR); // �����������
  }
}
#endif

void handle_chat_event(sFONT *Font)
{

#if LORA_ENABLED
  handle_lora_msg(Font);
#endif

  // ��������
  if (key.state == KEY_STATE_HOLD)
  {
    if (encode_struct.state == ENCODE_EVENT_DOWN) // �ĳ�ɾ������ɾ��Ҫ��Ҫ�Է�Ҳɾ���أ�
    {
       refreshState = 1;
      //  DEBUG_PRINT("start lora send\r\n");

      Englishposy = 0;
      Englishposx = 0;

      memset(lora_receive_buf, '\0', sizeof(lora_receive_buf));

      encode_struct.enable = 0; // ���ü�
      key.enable = 0;           // ���ü�
      send_wait_time = 250;
      lora_receive_flag = 2;
      return;
    }
    else if (encode_struct.state == ENCODE_EVENT_UP)
    {
      refreshState = 1;

      // if (lora_receive_buf[Englishposx + Englishposy * (LCD_WIDTH / Font->Width)] != 0)
      // lora_receive_buf[Englishposx + Englishposy * (LCD_WIDTH / Font->Width)] = '\0';
      // DEBUG_PRINT("����λ=%d \r\n", Englishposx + Englishposy * (LCD_WIDTH / Font->Width));
      Englishposx--;
      // �������
      if (Englishposx < 0)
      {
        if (Englishposy > 0)
        {
          Englishposy--;
          Englishposx = (LCD_WIDTH) / Font->Width - Font->Width; // �ܵ���ͷ
        }
        else
        {
          Englishposx = 0;
        }
      }

      send_wait_time = 250;
      encode_struct.enable = 0; // ���ü�
      key.enable = 0;           // ���ü�
      lora_receive_flag = 2;
      DEBUG_PRINT("��111������=%d \r\n", Englishposx);
      return;
    }
  }

  // ȷ������
  if (key.event == KEY_EVENT_CLICK)
  {

    refreshState = 1;
    if (lora_receive_buf[0] == 0)
    {
      return;
    }

    Englishposx++;
    // DEBUG_PRINT("��111������=%d \r\n", EDGE + EDGE + Englishposx * Font->Width);

    if (EDGE + EDGE + (Englishposx + 1) * Font->Width > LCD_WIDTH)
    {
      Englishposx = 0;
      Englishposy++;
      DEBUG_PRINT("��ǰ���򳤶�=%d \r\n", OPERATE_DOWN + EDGE + ((Englishposy)*Font->Height));
      if (OPERATE_DOWN + EDGE + ((Englishposy + 1) * Font->Height) >= LCD_HEIGHT) // ��һ�д���128����˵�����ˣ�ȡ������
      {
        Englishposy = 0; // ������λ��
      }
    }
    Englishcount = 0; // ���ü���

    if (lora_receive_buf[Englishposx + Englishposy * (LCD_WIDTH / Font->Width)] != 0)
    {
      DEBUG_PRINT("����\r\n");
      return;
    }
    lora_receive_buf[Englishposx + Englishposy * (LCD_WIDTH / Font->Width)] = 'a' + Englishcount;

    DEBUG_PRINT("����λ=%d \r\n", Englishposx + Englishposy * (LCD_WIDTH / Font->Width));
     lora_receive_flag = 2;
      send_wait_time = 250;
  }

  // �������״̬
  if (encode_struct.state == ENCODE_EVENT_UP)
  {
    Englishcount = (Englishcount + 1) % 27; // ѭ������
    lora_receive_flag = 2;
    send_wait_time = 250;
    lora_receive_buf[Englishposx + Englishposy * (LCD_WIDTH / Font->Width)] = 'a' + Englishcount;
    DEBUG_PRINT("����λ=%d \r\n", Englishposx + Englishposy * (LCD_WIDTH / Font->Width));
  }
  else if (encode_struct.state == ENCODE_EVENT_DOWN)
  {
    Englishcount = (Englishcount - 1 + 27) % 27; // ѭ������
    lora_receive_flag = 2;
    send_wait_time = 250;
    lora_receive_buf[Englishposx + Englishposy * (LCD_WIDTH / Font->Width)] = 'a' + Englishcount;
    DEBUG_PRINT("����λ=%d \r\n", Englishposx + Englishposy * (LCD_WIDTH / Font->Width));
  }
}

// void handle_chat_history_event()
// {
//   if (encode_struct.state == ENCODE_EVENT_UP) // ���Ϲ���
//   {
//     if (current_line > 0)
//     {
//       current_line--;
//       refreshState = 1;
//     }
//   }

//   if (encode_struct.state == ENCODE_EVENT_DOWN) // ���¹���
//   {
//     if (current_line < total_lines - MAX_LINE_TO_SHOW - 1)
//     {
//       current_line++;
//       refreshState = 1;
//     }
//   }

//   if (key.state == KEY_STATE_HOLD) // ɾ�������¼
//   {
//     if (encode_struct.state == ENCODE_EVENT_DOWN)
//     {
//       refreshState = 1;
//       memset(lora_receive_buf, 0, sizeof(lora_receive_len));                                          // ��������¼
//       Screen_Clear(EDGE, OPERATE_DOWN + EDGE, 127 - EDGE, CHAT_HISTORY_DOWN - EDGE, MY_SCREEN_COLOR); // �����ʾ����

//       total_lines = 0;
//       current_line = 0;
//       key.enable = 0; // ���ü�
//     }
//   }
// }

// 1��18��
void show_history_data(sFONT *Font)
{

  if (lora_receive_flag == 3)
  {
     refreshState = 1;
    lora_receive_flag = 0;
    Englishposy = lora_receive_len / ((LCD_WIDTH - (EDGE + EDGE)) / Font->Width);
    Englishposx = lora_receive_len - Englishposy * ((LCD_WIDTH - (EDGE + EDGE)) / Font->Width) - 1;

     DEBUG_PRINT("�յ���Ϣ=%d, %d,%d,\r\n", Englishposx,Englishposy,lora_receive_len);
  }
#if 0
  int start_line = current_line;
  int end_line = current_line + MAX_LINE_TO_SHOW;

  // strcpy(lora_receive_buf, \
// "aelloworldthisisat" \
// "bstanotherlinewwwr" \
// "celloworldthisisat" \
// "dstanotherlinewwwr" \
// "eelloworldthisisat" \
// "fstanotherlinewwwr" \
// "gelloworldthisisat" \
// "hstanotherlinewwwr");
  //   lora_receive_len = 145;

  total_lines = (lora_receive_len + COLUMN - 1) / COLUMN; // ����������

  if (end_line > total_lines)
  {
    end_line = total_lines;
  }

  // �����ʾ����
  //   Screen_Clear(EDGE, OPERATE_DOWN + EDGE, 127 - EDGE, CHAT_HISTORY_DOWN - EDGE, MY_SCREEN_COLOR); // �����������

  // ��̬���� lora_receive_buf ����ʾ��ǰ��Χ�ڵ������¼
  const char *ptr = lora_receive_buf;
  int line_count = 0;
  int displayed_lines = 0;
  int line_length = 0; // ÿһ�еĳ���

  while (*ptr && line_count < total_lines)
  {
    if (line_count >= start_line && line_count < end_line)
    {
      char line[COLUMN + 1] = {0};
      int i = 0;
      while (*ptr && line_length < COLUMN)
      {
        line[i++] = *ptr++;
        // DEBUG_PRINT("line=%s \r\n", line[i++]);
        line_length++;
      }

      Paint_DrawString(1, 22 + (displayed_lines++) * Font->Height, line, Font, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, 'a',999);
    }
    else
    {
      while (*ptr && line_length < COLUMN) // ����������ͷ��������
      {
        ptr++;
        line_length++;
      }
    }
    line_count++;
    line_length = 0;
  }
#endif
}

void chat_page(sFONT *Font)
{

#if BATTERY_ENABLED
  show_battery(BATTERY_X, BATTERY_Y, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, &isFirstBattaryShow); // ��ʾ�����Ϣ
#endif

  if (refreshState)
  {

    Paint_DrawChar(1, 1, 0, &Font16_Operate, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, 0); // ���õ�ͼ��
    // Screen_Clear(0, OPERATE_DOWN, 127, CHAT_HISTORY_DOWN, MY_SCREEN_COLOR);                // �����¼����
    Screen_Clear(0, OPERATE_DOWN, 127, CHAT_DOWN, MY_SCREEN_COLOR);                              // �����
    Paint_DrawRectangle(0, OPERATE_DOWN, 127, CHAT_DOWN, GREEN, DOT_PIXEL_1X1, DRAW_FILL_EMPTY); // ��������

    refreshState = 0;
  }

  Paint_DrawString(EDGE, OPERATE_DOWN + EDGE, lora_receive_buf, Font, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, 'a', Englishposx + Englishposy * (LCD_WIDTH / Font->Width));

  show_history_data(Font);
  handle_chat_event(Font);
}
// void chat_history_page(sFONT *Font)
// {
// #if BATTERY_ENABLED
//   show_battery(BATTERY_X, BATTERY_Y, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, &isFirstBattaryShow);
// #endif
//   if (refreshState)
//   {
//     Paint_DrawRectangle(0, OPERATE_DOWN, 127, CHAT_HISTORY_DOWN, GREEN, DOT_PIXEL_1X1, DRAW_FILL_EMPTY); // �����¼�������
//                                                                                                          //  Screen_Clear(0, CHAT_UP, 128, CHAT_DOWN, MY_SCREEN_COLOR);                                        // �����
//     Paint_DrawRectangle(0, CHAT_UP, 127, CHAT_DOWN, MY_SCREEN_COLOR, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);    // ��������
//     show_history_data(Font);
//     refreshState = 0;
//   }

//   handle_chat_history_event();
// }

void perpare_setting_page(sFONT *Font)
{
#if BATTERY_ENABLED
  show_battery(BATTERY_X, BATTERY_Y, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, &isFirstBattaryShow); // ������
#endif
  if (refreshState)
  {
    Screen_Clear(0, OPERATE_DOWN, 128, CHAT_DOWN, MY_SCREEN_COLOR); // �����¼����
    Paint_DrawChar(0, 0, 0, &Font16_Operate, GREEN, BLUE, 0);       // ����ҳ�����

    refreshState = 0;
  }
  Paint_DrawString(EDGE, OPERATE_DOWN + EDGE, lora_receive_buf, Font, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, 'a', 999);
  show_history_data(Font);
}

/**************************************����ҳ��**************************************************** */
void clamp_value(int *value, int min, int max)
{
  if (*value < min)
    *value = min;
  if (*value > max)
    *value = max;
}

void update_setting_value(int value, int min, int max, int current_setting)
{
  clamp_value(&value, min, max);
  *settings[current_setting].value = (u8)value;
}

void update_current_setting(int value)
{
  switch (current_setting)
  {
  case SETTING_SCREEN_LIGHT:
    value = (value < 0) ? 0 : (value > 100) ? 100
                                            : value;
    TIM1->CH3CVR = value;
    break;
  case SETTING_SHAKE_MODE:
    *settings[current_setting].value = !(*settings[current_setting].value);
    break;
  case SETTING_LORA_FREQ:
    update_setting_value(value, LORAFREQ_MIN, LORAFREQ_MAX, current_setting);
    break;
  case SETTING_LORA_POWER:
    update_setting_value(value, LORAPOWER_MIN, LORAPOWER_MAX, current_setting);
    break;
  case SETTING_LORA_BANDWIDTH:
    update_setting_value(value, LORABANDWIDTH_MIN, LORABANDWIDTH_MAX, current_setting);
    break;
  case SETTING_LORA_SPREAD_FACTOR:
    update_setting_value(value, LORASpreadFactor_MIN, LORASpreadFactor_MAX, current_setting);
    break;
  default:
    break;
  }

  if (settings[current_setting].update_func)
  {
    settings[current_setting].update_func();
  }
}

void draw_setting(int index, int highlight, sFONT *Font)
{

  char strBuf[4]; // ���ڴ洢���3λ���ֺ�һ����ֹ��
  UWORD bg_color = highlight ? GREEN : MY_THEME_COMPONT_COLOR;

  Paint_DrawString(0, index * CHAR_HEIGHT + Y_OFFSET, settings[index].name, Font, MY_THEME_BACK_COLOR, bg_color, 'a', 999);
  Paint_DrawChar(Font->Width * strlen(settings[index].name), index * CHAR_HEIGHT + Y_OFFSET, 11, &Font16_Num, MY_THEME_BACK_COLOR, bg_color, 0);

  // ���ݵ�ǰ�������ͻ���ֵ
  if (index == SETTING_SHAKE_MODE)
  {

    if (highlight || isFirstSettingShow)
    {
      isFirstSettingShow = 0;
      if (*settings[index].value == ON)
      {
        Paint_DrawString(Font->Width * (strlen(settings[index].name) + 1), index * CHAR_HEIGHT + Y_OFFSET, "0", &Font16_button, MY_THEME_BACK_COLOR, GREEN, '0', 999);
        Paint_DrawString(Font->Width * (strlen(settings[index].name) + 1) + 15, index * CHAR_HEIGHT + Y_OFFSET + 2, "0", &Font16_cycle, GREEN, WHITE, '0', 999);
      }
      else
      {
        Paint_DrawString(Font->Width * (strlen(settings[index].name) + 1), index * CHAR_HEIGHT + Y_OFFSET, "0", &Font16_button, MY_THEME_BACK_COLOR, RED, '0', 999);
        Paint_DrawString((Font->Width * (strlen(settings[index].name) + 1)) + 2, index * CHAR_HEIGHT + Y_OFFSET + 2, "0", &Font16_cycle, RED, WHITE, '0', 999);
      }
    }

    return;
  }
  else
  {
    intToStr(*settings[index].value, strBuf, 3);
  }

  Paint_DrawString(Font->Width * (strlen(settings[index].name) + 1), index * CHAR_HEIGHT + Y_OFFSET, strBuf, &Font16_Num, MY_THEME_BACK_COLOR, bg_color, '0', 999);
}

void display_settings(sFONT *Font)
{

  if (refreshState)
  {
    for (int i = 0; i < SETTING_COUNT; i++)
    {
      draw_setting(i, i == current_setting, Font);
    }
    refreshState = 0;
  }
}

void handle_setting_event()
{
  if (encode_struct.state == ENCODE_EVENT_UP)
  { // ���������Ϲ���
    update_current_setting(*settings[current_setting].value + 1);
    refreshState = 1;
  }
  else if (encode_struct.state == ENCODE_EVENT_DOWN)
  { // ���������¹���
    update_current_setting(*settings[current_setting].value - 1);
    refreshState = 1;
  }
  if (key.event == KEY_EVENT_CLICK)
  { // ȷ�ϼ����
    current_setting = (current_setting + 1) % SETTING_COUNT;
    refreshState = 1;
  }
}

void setting_page(sFONT *Font)
{
  // ���ö���
  Paint_DrawString(48, 0, "01", &Font16_setting, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, '0', 999);

  display_settings(Font);
  handle_setting_event();
}

/**************************************����ҳ��**************************************************** */

void show_info(int posx, int posy, const char *label, int value, int offset)
{
  char strBuf[4];                                                                                         // �洢���3λ���ֺ�һ����ֹ��
  Paint_DrawString(posx, posy, label, &Font16_En, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, 'a', 999); // ���Ʊ�ǩ
  Paint_DrawChar(posx + Font16_En.Width * (offset - 1), posy, 11, &Font16_Num, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, 0);
  intToStr(value, strBuf, 3);
  Paint_DrawString(posx + Font16_Num.Width * offset, posy, strBuf, &Font16_Num, MY_THEME_BACK_COLOR, MY_THEME_COMPONT_COLOR, '0', 999); // ����ֵ
}

void info_page()
{
#if BATTERY_ENABLED
  show_info(0, 0, "batadc", Battery_ADC_Average, 7); // ������ʾ�����Ϣ
#endif

#if LORA_ENABLED
  show_info(0, 20, "rssi", SX1278_LoRaReadRSSI(), 5);             // ������ʾ Lora RSSI
  show_info(0, 40, "loraid", SX1278_Read_Reg(REG_LR_VERSION), 7); // ������ʾ Lora ID
#endif

#if SCREEN_ENABLED
  show_info(0, 60, "light", TIM1->CH3CVR, 6); // ������ʾ��Ļ����
#endif
}

void show_page()
{
#if DEBUG_ENABLED == 2
  page = PAGE_SEND;
#endif
  switch (page) // ����ҳ��
  {
  case PAGE_SEND: // ���ͽ���

    chat_page(&Font12_En);

    if (key.event == KEY_EVENT_LONG_CLICK) // ����
    {
      refreshState = 1;
      page = PAGE_PERPARE_SETTING;
    }
    break;

    // case PAGE_HISTROY_CHAT: // �����¼����  ��Ҫ�����鿴��¼
    //   chat_history_page(&Font12_En);
    //   if (key.event == KEY_EVENT_LONG_CLICK)
    //   {
    //     refreshState = 1;
    //     page = PAGE_PERPARE_SETTING;
    //   }

    //   break;

  case PAGE_PERPARE_SETTING: // ׼�����ý���
    perpare_setting_page(&Font12_En);
    if (key.event == KEY_EVENT_CLICK)
    {
      refreshState = 1;
      Screen_Clear(0, 0, 127, 127, MY_THEME_BACK_COLOR);

      // // ����һ���������õĵط�
      // Paint_DrawChar(40, 40, 0, &Font24_icon, MY_THEME_BACK_COLOR, BLUE, 0);
      // Delay_Ms(500);
      // // ����ɾ��

      // Screen_Clear(0, 0, 127, 127, MY_THEME_BACK_COLOR);
      isFirstSettingShow = 1;
      page = PAGE_SETTING;
    }

    if (key.event == KEY_EVENT_LONG_CLICK)
    {
      refreshState = 1;
      Screen_Clear(0, 0, 127, 127, MY_THEME_BACK_COLOR);

      isFirstBattaryShow = 1;
      page = PAGE_SEND;
    }
    break;

  case PAGE_SETTING: // ���ý���

    setting_page(&Font16_En);

    if (key.event == KEY_EVENT_LONG_CLICK)
    {
      refreshState = 1;
      Screen_Clear(0, 0, 127, 127, MY_THEME_BACK_COLOR);
      // page = PAGE_INFO;
      isFirstBattaryShow = 1;
      page = PAGE_SEND;
    }
    break;

    // case PAGE_INFO: // ��Ϣ����
    //   info_page();
    //   if (key.event == KEY_EVENT_LONG_CLICK)
    //   {
    //     Screen_Clear(0,0,128,128,MY_THEME_BACK_COLOR);
    //     page = PAGE_SEND;
    //   }
    //   break;

  default: // Ĭ�����
    page = PAGE_SEND;
    break;
  }

  // �������¼�������¼�
  key.event = KEY_EVENT_NONE;
  encode_struct.state = ENCODE_EVENT_NONE;
}

// ��д���ֽṹ-
// ÿ��page��ҳ��������ʾ
// ÿ��ҳ��Ĳ���
// show_page��ʾ�ܵ�page����

// ������ʾ
// �߼�����
