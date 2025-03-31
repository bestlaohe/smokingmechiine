#include "SX1278.h"
#include <string.h> // ������׼C���е��ַ������ڴ��������
// Ƶ�����ݱ�
const u8 sx1276_7_8FreqTbl[][3] =
    {
        {0x6B, 0x00, 0x00}, // 428MHz
        {0x6B, 0x40, 0x00}, // 429MHz
        {0x6B, 0x80, 0x00}, // 430MHz
        {0x6B, 0xC0, 0x00}, // 431MHz
        {0x6C, 0x00, 0x00}, // 432MHz
        {0x6C, 0x40, 0x00}, // 433MHz
        {0x6C, 0x80, 0x00}, // 434MHz
        {0x6C, 0xC0, 0x00}, // 435MHz
        {0x6D, 0x00, 0x00}, // 436MHz
        {0x6D, 0x40, 0x00}, // 437MHz
        {0x6D, 0x80, 0x00}, // 438MHz
        {0x6D, 0xC0, 0x00}  // 439MHz
};
//

#define SX1278_MAX_BUFSIZE 64 // �������������ݳ���

// �������ò��֣�����ֻ���ز�Ƶ���ǿ���ͨ��������ģ���������ú�Ϊ�̶�ֵ
u8 Lora_Freq = LORAFREQ_434MHZ;         //  Ĭ��Ƶ������428-439MHz
u8 Lora_Power = LORAPOWER_20DBM;        //  �����������11��14��17��20
u8 Lora_SpreadFactor = 7;               //  ������Ƶ������7~12
u8 Lora_BandWide = LORABW_125KHz;       //  ��������
u8 Lora_ErrorCoding = ERROR_CODING_4_5; //  ǰ�����4/5 4/6 4/7 4/8
/********************************************************************/

/****************��������ֲ��Ҫʵ�ֵĶ�Ӧ�ӿڲ���***********************************/
#define SX1278_DelayMs(t) Delay_Ms(t) // ������ʱ������ʵ��
char lora_receive_buf[145] = {0};
volatile u8 lora_receive_len = 1;
volatile u8 lora_receive_flag = 0; // 0�ǳ�ʼ״̬��1�ǽ��յ������Ӧ�ˣ�2�ǵȴ����ջ�Ӧ��3�ǽ��յ�������

extern volatile u8 loraComplete; // lora�жϲ�����ɵı�־
// ���ƿ����ó�ʼ�����жϿ�����������жϳ�ʼ����
void SX1278_Receive()
{

  //  DEBUG_PRINT("lora ID  0x%X\r\n", SX1278_Read_Reg(REG_LR_VERSION)); // 0x12
  //  DEBUG_PRINT("lora mode  0x%X\r\n", SX1278_Read_Reg(LR_RegOpMode)); // 0x89  0100 0101
  //
  //  DEBUG_PRINT("SX1278_LoRaReadRSSI= %d \r\n", SX1278_LoRaReadRSSI());
  //  DEBUG_PRINT("SX1278_ReadRSSI= %d \r\n", SX1278_ReadRSSI());

  u8 res; // �����ķ���

  res = SX1278_LoRaRxPacket(lora_receive_buf, 5000);

  if (res == 0)
  {

    DEBUG_PRINT("lora_receive_len=  %d\r\n", lora_receive_len);

    u8 var;
    for (var = 0; var < lora_receive_len; ++var)
    {
      DEBUG_PRINT("lora_receive_buf= %d\r\n", lora_receive_buf[var]);
    }
    // ʵ�ʵ�����Ҫȥ��β����*=42
    if (lora_receive_len < 1)
    {
      DEBUG_PRINT("SX1278_Receive erro\r\n");
      return;
    }
    lora_receive_buf[lora_receive_len - 1] = 0; // Ҫ��ղ�Ȼ��Ϊ�ж��������
    lora_receive_len = lora_receive_len - 1;
  }
  else if (res == 2)
  {
    DEBUG_PRINT("lora CRC eeror!\r\n");
  }
  else if (res == 3)
  {
    DEBUG_PRINT("Received response: **\r\n");
  }
  else if (res == 4)
  {
    DEBUG_PRINT("illegal msg\r\n");
  }
}

// void SX1278_GPIO_Init()
// {
//   SX1278_NSS_DISABLE;
//   SET_SX1278_RST();
// }
// void SX1278_SPI_Init()
// {

//   SET_SX1278_MOSI();
//   SET_SX1278_SCK();
// }

// ������Ч����λ��ǰ��SCKƽʱΪ�͵�ƽ
u8 SX1278_SPI_RW(u8 byte)
{
  u8 retry = 0;

  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
  {
    retry++;
    if (retry > 200)
    {
      DEBUG_PRINT("send spi outtime!\r\n");
      return 0;
    }

  } // ��û����
  SPI_I2S_SendData(SPI1, (uint16_t)byte);

  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
  {
    retry++;
    if (retry > 200)
    {
      DEBUG_PRINT("rec spi outtime!\r\n");
      return 0;
    }
  } // ��û����

  return (uint8_t)SPI_I2S_ReceiveData(SPI1);
}

/*****************��������ֲ��Ҫʵ�ֵĲ���**************************************************************/

/*****************������MCU�޹أ���ֲ�������*******************************************************/
/**********************************************************
**Name:     SX1278_Read_Reg
**Function: SPI Read CMD
**Input:    adr -> address for read
**Output:   None
**********************************************************/
u8 SX1278_Read_Reg(u8 adr)
{

  while (!READ_LCD_CS)
  {
    DEBUG_PRINT("wait lcd cs 1!\r\n");
  }
  u8 tmp;
  SX1278_NSS_ENABLE;
  SX1278_SPI_RW(adr);
  tmp = SX1278_SPI_RW(adr);
  SX1278_NSS_DISABLE;
  return (tmp);
}

/**********************************************************
**Name:     SX1278_Write_Reg
**Function: SPI Write CMD
**Input:    u8 address & u8 data
**Output:   None
**********************************************************/
void SX1278_Write_Reg(u8 adr, u8 WrPara)
{
  while (!READ_LCD_CS)
  {

    DEBUG_PRINT("wait lcd cs 1!\r\n");
  }
  SX1278_NSS_ENABLE;
  SX1278_SPI_RW(adr | 0x80);
  SX1278_SPI_RW(WrPara); // д������

  SX1278_NSS_DISABLE;
}
/**********************************************************
**Name:     SX1278_Burst_Read
**Function: SPI burst read mode
**Input:    adr-----address for read
**          ptr-----data buffer point for read
**          length--how many bytes for read
**Output:   None
**********************************************************/
void SX1278_Burst_Read(u8 adr, u8 *ptr, u8 length)
{
  u8 i;
  if (length <= 1) // length must more than one
    return;
  else
  {
    while (!READ_LCD_CS)
    {

      DEBUG_PRINT("wait lcd cs 1!\r\n");
    }
    SX1278_NSS_ENABLE;
    SX1278_SPI_RW(adr);
    for (i = 0; i < length; i++)
      ptr[i] = SX1278_SPI_RW(0);
    SX1278_NSS_DISABLE;
  }
}

/**********************************************************
**Name:     SPISX1278_Burst_Write
**Function: SPI burst write mode
**Input:    adr-----address for write
**          ptr-----data buffer point for write
**          length--how many bytes for write
**Output:   none
**********************************************************/
void SX1278_Burst_Write(u8 adr, u8 *ptr, u8 length)
{
  u8 i;
  if (length > 0) // length must more than one
  {
    while (!READ_LCD_CS)
    {

      DEBUG_PRINT("wait lcd cs 1!\r\n");
    }
    SX1278_NSS_ENABLE;
    SX1278_SPI_RW(adr | 0x80);
    for (i = 0; i < length; i++)
    {

      //  DEBUG_PRINT(" ptr[i]: %d\r\n", ptr[i]);
      SX1278_SPI_RW(ptr[i]);
    }
    SX1278_NSS_DISABLE;
  }
}

// ����: SX1278 �����Ĵ���������
unsigned char SX1276ReadReg(unsigned char addr)
{
  unsigned char dat;
  while (!READ_LCD_CS)
  {

    DEBUG_PRINT("wait lcd cs 1!\r\n");
  }
  SX1278_NSS_ENABLE;
  SX1278_SPI_RW(addr & 0x7F);
  dat = SX1278_SPI_RW(0);
  SX1278_NSS_DISABLE;
  return dat;
}

/*************************************************************************
 ���ܣ�����׼��״̬
 ��������
 ���أ���
***************************************************************************/
void SX1278_Standby(void)
{
  SX1278_Write_Reg(LR_RegOpMode, 0x09); // Standby//Low Frequency Mode
  // SX1278_Write_Reg(LR_RegOpMode,0x01);                              	 //Standby//High Frequency Mode
}

/*************************************************************************
 ���ܣ�����˯��״̬
 ��������
 ���أ���
***************************************************************************/
void SX1278_Sleep(void)
{
  SX1278_Write_Reg(LR_RegOpMode, 0x08); // Sleep//Low Frequency Mode
  // SX1278_Write_Reg(LR_RegOpMode,0x00);                            		 //Sleep//High Frequency Mode
}

/*********************************************************/
// LoRa mode
/*********************************************************/
/*************************************************************************
 ���ܣ�����LORAģʽ
 ��������
 ���أ���
***************************************************************************/
void SX1278_EntryLoRa(void)
{
  SX1278_Write_Reg(LR_RegOpMode, 0x88); // Low Frequency Mode
  // SX1278_Write_Reg(LR_RegOpMode,0x80);//High Frequency Mode
}

/*************************************************************************
 ���ܣ����жϱ�־λ
 ��������
 ���أ���
***************************************************************************/
void SX1278_LoRaClearIrq(void)
{
  SX1278_Write_Reg(LR_RegIrqFlags, 0xFF);
}

/*************************************************************************
 ���ܣ������Ĵ�������
 ��������
 ���أ���
***************************************************************************/
void SX1278_Config(void)
{

  // ˯��ģʽ�ɳ�ʼ���ͽ��շ��ͺ������
  SX1278_Sleep(); // �ı�ģʽ��Ҫ��˯��ģʽ
  SX1278_DelayMs(10);

  SX1278_EntryLoRa();
  // SX1278_Write_Reg(0x5904);   //Change digital regulator form 1.6V to 1.47V: see errata note

  SX1278_Burst_Write(LR_RegFrMsb, (u8 *)sx1276_7_8FreqTbl[Lora_Freq], 3); // setting frequency parameter

  // setting base parameter �������
  SX1278_Write_Reg(LR_RegPaConfig, Lora_Power); // Setting output power parameter

  SX1278_Write_Reg(LR_RegOcp, 0x0B); // RegOcp,Close Ocp �������� 100mA
  SX1278_Write_Reg(LR_RegLna, 0x23); // RegLNA,High & LNA Enable����������

  //	if(Lora_SpreadFactor==6)           //SFactor=6//��ʹ����Ƶ6����
  //	{
  //		u8 tmp;
  //		SX1278_Write_Reg(LR_RegModemConfig1,((Lora_BandWide<<4)+(Lora_ErrorCoding<<1)+0x01));//Implicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
  //		SX1278_Write_Reg(LR_RegModemConfig2,((Lora_SpreadFactor<<4)+0x04+0x03));
  //
  //		tmp = SX1278_Read_Reg(0x31);
  //		tmp &= 0xF8;
  //		tmp |= 0x05;
  //		SX1278_Write_Reg(0x31,tmp);
  //		SX1278_Write_Reg(0x37,0x0C);
  //        DEBUG_PRINT("SFactor=6\r\n");
  //	}
  //	else
  //	{
  SX1278_Write_Reg(LR_RegModemConfig1, ((Lora_BandWide << 4) + (Lora_ErrorCoding << 1) + 0x00)); // �������� & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)����ʾ��ͷģʽ
  SX1278_Write_Reg(LR_RegModemConfig2, ((Lora_SpreadFactor << 4) + 0x04 + 0x03));                // SFactor & CRC LNA gain set by the internal AGC loop
  //	}
  //	SX1278_Write_Reg(LR_RegSymbTimeoutLsb,0xFF);                   //RegSymbTimeoutLsb Timeout = 0x3FF(Max)

  SX1278_Write_Reg(LR_RegPreambleMsb, 0x00); // RegPreambleMsb
  SX1278_Write_Reg(LR_RegPreambleLsb, 12);   // RegPreambleLsb 8+4=12byte Preamble

  SX1278_Write_Reg(REG_LR_DIOMAPPING2, 0x01); // RegDioMapping2 DIO5=00, DIO4=01

  SX1278_Standby(); // Entry standby mode
}

/*************************************************************************
 ���ܣ��������ģʽ
 ��������
 ���أ���
***************************************************************************/

u8 SX1278_LoRaEntryRx(void)
{
  u8 addr = 0;
  u8 t = 5;
  SX1278_Standby();                     // �������ģʽ�Ӵ���ģʽ��������ģʽ
  SX1278_Write_Reg(REG_LR_PADAC, 0x84); // zhen and Rx

  SX1278_Write_Reg(LR_RegHopPeriod, 0xFF);    // ����ģʽ��Ƶ����д0XFF����Ƶ
  SX1278_Write_Reg(REG_LR_DIOMAPPING1, 0x01); // DIO0=00, DIO1=00, DIO2=00, DIO3=01

  SX1278_Write_Reg(LR_RegIrqFlagsMask, 0x9F); // Open RxDone interrupt & CRC  8001 1111

  //  SX1278_Write_Reg(REG_LR_DIOMAPPING1, 0x41); // DIO0=01, DIO1=00, DIO2=00, DIO3=01
  //  SX1278_Write_Reg(LR_RegIrqFlagsMask, 0x0F); // Open RxDone interrupt & CRC  8001

  SX1278_LoRaClearIrq();

  //  SX1278_Write_Reg(LR_RegPayloadLength,21);         //RegPayloadLength  21byte(this register must difine when the data long of one byte in SF is 6)
  // ʹ�ö������ݰ�ʱ��Ҫ���ã���ʹ�ö�����������

  addr = SX1278_Read_Reg(LR_RegFifoRxBaseAddr); // Read RxBaseAddr
  SX1278_Write_Reg(LR_RegFifoAddrPtr, addr);    // RxBaseAddr -> FiFoAddrPtr��
  SX1278_Write_Reg(LR_RegOpMode, 0x8d);         // Continuous Rx Mode//Low Frequency Mode 1000 1101
  // SX1278_Write_Reg(LR_RegOpMode,0x05);                          //Continuous Rx Mode//High Frequency Mode

  //  while (SX1278_Read_Reg(LR_RegOpMode)!=0x88)//1000 1000 tx���
  while (t--)
  {

    // DEBUG_PRINT("Rx Mode=0x%X \r\n", SX1278_Read_Reg(LR_RegOpMode));//0x8d 1000 1010
    if ((SX1278_Read_Reg(LR_RegModemStat) & 0x04) == 0x04) // Rx-on going RegModemStat
      break;
  }
  return 1;
}
/*************************************************************************
 ���ܣ���ȡLora�ź�ǿ��
 ��������
 ���أ��ź�ǿ�� 127���
***************************************************************************/
u8 SX1278_LoRaReadRSSI(void)
{
  u16 temp = 10;
  temp = SX1278_Read_Reg(LR_RegRssiValue); // Read RegRssiValue��Rssi value
  temp = temp + 127 - 137;                 // 127:Max RSSI, 137:RSSI offset
  return (u8)temp;
}
/*************************************************************************
 ���ܣ���ȡ���հ����������� ����ͨ���ź�����������������ʱ��ѯ����
 ������valid_data[out]     �ⲿ���������׵�ַ
      packet_length[out]  �������ݵĳ���
            timeout[in]   ���ճ�ʱ��Ϊ0ʱ���õȴ� ��λʱ��ms
���أ�	 0:���ճɹ� 1������ʧ�ܳ�ʱ 2:CRCУ�����
***************************************************************************/
u8 SX1278_LoRaRxPacket(u8 *valid_data, u16 timeout)
{
  u8 addr, irq_flag, mode;
  u8 packet_size;
  u8 temp_data[200];

  mode = SX1278_Read_Reg(LR_RegOpMode);

  if ((mode & 0x05) != 0x05)
  {
    // DEBUG_PRINT("enter rxmode  \r\n");
    SX1278_LoRaEntryRx(); // �������ģʽ
  }

  if (loraComplete) // ���жϽ���
  {
    loraComplete = 0;
    irq_flag = SX1278_Read_Reg(LR_RegIrqFlags);
    //      DEBUG_PRINT("0-irq_flag=0x%X\r\n", irq_flag);
    //    DEBUG_PRINT("1-irq_flag=0x%X\r\n", SX1278_Read_Reg(LR_RegIrqFlags));

    if ((irq_flag & RFLR_IRQFLAGS_RXDONE) == RFLR_IRQFLAGS_RXDONE)
    {
      if ((irq_flag & RFLR_IRQFLAGS_PAYLOADCRCERROR) == RFLR_IRQFLAGS_PAYLOADCRCERROR) // �����CRCУ������ж�
      {
        SX1278_LoRaClearIrq();
        // SX1278_Sleep();
        return (2);
      }

      addr = SX1278_Read_Reg(LR_RegFifoRxCurrentaddr); // last packet addr
      DEBUG_PRINT("addr  %d\r\n", addr);
      SX1278_Write_Reg(LR_RegFifoAddrPtr, addr);      // RxBaseAddr -> FiFoAddrPtr
      packet_size = SX1278_Read_Reg(LR_RegRxNbBytes); // Number for received bytes



      // ��¼��ǰ��Ч���ݵĳ���
      u8 current_length = lora_receive_len;

      if ((current_length + packet_size) >= sizeof(lora_receive_buf)) // ����200��ͷ����
      {
        lora_receive_len = 1;
        current_length = 0;
      }

      // ��FIFO�ж�ȡ���ݵ�valid_data�����з�β��һֱ�ۼ�
      // SX1278_Burst_Read(0x00, valid_data + current_length, packet_size);

      SX1278_Burst_Read(0x00, temp_data, packet_size); // �����ݶ�ȡ��������

      SX1278_LoRaClearIrq();
      // SX1278_Sleep(); // ����˯��ģʽ

      // ����յ��������Ƿ�Ϊ "**"
      //      DEBUG_PRINT(" packet_size: %d\r\n", packet_size);
      //      DEBUG_PRINT("  valid_data[current_length]: %d\r\n", valid_data[current_length]);
      if (packet_size == 2 && temp_data[packet_size - 1] == '*' && temp_data[packet_size - 2] == '*')
      {

        lora_receive_flag = 1;
        return 3; // ��ʾ�յ��Է���Ӧ�����ظ�
      }
      else // ������� "**"������ "**" ��ʾ�յ�
      {

        if (temp_data[packet_size - 1] != '*')
        {
          DEBUG_PRINT(" packet_size: %d\r\n", packet_size);
          DEBUG_PRINT("illegal msg=%c\r\n", temp_data[packet_size - 1]);
          DEBUG_PRINT("illegal msg=%c\r\n", temp_data[packet_size - 2]);
          DEBUG_PRINT("illegal msg=%c\r\n", temp_data[0]);
          lora_receive_len = 1;
          lora_receive_flag = 3;
          memset(valid_data, '\0', sizeof(lora_receive_buf));
          return 4;
        }
        else // �Ϸ���Ϣ��Ӧһ��
        {

          if (!SX1278_LoRaTxPacket("*", 1))
          {

            DEBUG_PRINT("mode=0x%X \r\n", SX1278_Read_Reg(LR_RegOpMode)); // 0x88 1000 1000
            DEBUG_PRINT("Lora send ok\r\n");
            SX1278_LoRaEntryRx(); // �������ģʽ
          }
          else
          {

            DEBUG_PRINT("mode=0x%X \r\n", SX1278_Read_Reg(LR_RegOpMode)); // 0x8b 1000 1010

            DEBUG_PRINT("Lora send fail\r\n");
            SX1278_LoRaEntryRx(); // �������ģʽ
          }
          // ��������������
          memcpy(valid_data, temp_data, packet_size);
          lora_receive_len = packet_size;
          DEBUG_PRINT("Received my msg\r\n");
          lora_receive_flag = 3;
        }
      }

      return 0;
    }
    else
    {
      if ((irq_flag & RFLR_IRQFLAGS_TXDONE) == RFLR_IRQFLAGS_TXDONE)
      {
        DEBUG_PRINT("send ok irq_flag\r\n");
      }
      else
      {
        DEBUG_PRINT("2-irq_flag=0x%X\r\n", SX1278_Read_Reg(LR_RegIrqFlags));

      }
    }
  }

  // SX1278_Sleep(); // ����˯��ģʽ
  return (1);
}

/*************************************************************************
 ���ܣ����뷢��ģʽ
 ��������
 ���أ���
***************************************************************************/
u8 SX1278_LoRaEntryTx(u8 packet_length)
{
  u8 addr = 0, temp;
  u8 t = 3;
  SX1278_Standby();                           // �������ģʽ�Ӵ���ģʽ��������ģʽ
  SX1278_Write_Reg(REG_LR_PADAC, 0x87);       // Tx for 20dBm����ʱ��Ҫ���õ�ֵ
  SX1278_Write_Reg(LR_RegHopPeriod, 0x00);    // ����ģʽ��Ƶ����д0X00����Ƶ
  SX1278_Write_Reg(REG_LR_DIOMAPPING1, 0x41); // DIO0=01, DIO1=00, DIO2=00, DIO3=01

  SX1278_Write_Reg(LR_RegIrqFlagsMask, 0xF7); // Open TxDone interrupt
  SX1278_LoRaClearIrq();

  SX1278_Write_Reg(LR_RegPayloadLength, packet_length);

  addr = SX1278_Read_Reg(LR_RegFifoTxBaseAddr); // RegFiFoTxBaseAddr
  SX1278_Write_Reg(LR_RegFifoAddrPtr, addr);    // RegFifoAddrPtr
  while (t--)
  {
    temp = SX1278_Read_Reg(LR_RegPayloadLength);
    if (temp == packet_length)
    {
      // DEBUG_PRINT("SX1278_LoRaEntryTx=OK\r\n");
      break;
    }
  }
  return 0;
}
/*************************************************************************
 ���ܣ��������ݰ�,�������� ����ͨ���ź�����������������ʱ��ѯ������
      �������ʱ��500ms
 ������tx_buf[in]         �ⲿ�����������׵�ַ
       packet_length[in] �������ݳ���
 ���أ�0:���ͳɹ� 1������ʧ��
***************************************************************************/
u8 SX1278_LoRaTxPacket(u8 *valid_data, u8 packet_length)
{
  //  u8 timeout = 255;
  u8 temp_data[54];
  u8 temp_packet_length = packet_length;
  // ��ԭʼ���ݸ��Ƶ���ʱ������
  memcpy(temp_data, valid_data, packet_length);

  // ����ʱ������ĩβ��� '*' �ַ�
  temp_data[packet_length] = '*';
  temp_packet_length++; // ������ʱ���ݰ������԰�����ӵ� '*' �ַ�
  int t;
  for (t = 0; t < temp_packet_length; t++)
  {
    DEBUG_PRINT("sss=%c ", temp_data[t]);
    // DEBUG_PRINT("%d=%c ", t, temp_data[t]);
  }
  DEBUG_PRINT("\r\n");
  SX1278_LoRaEntryTx(temp_packet_length); // �������ģʽ
  SX1278_Burst_Write(0x00, temp_data, temp_packet_length);
  SX1278_Write_Reg(LR_RegOpMode, 0x8b); // Tx Mode 1000 1011  loraģʽ��txģʽ

  while (SX1278_Read_Reg(LR_RegOpMode) != 0x88) // 1000 1000 tx���
  {

    if (READ_SX1278_NIRQ()) // Packet send over
    {
      // DEBUG_PRINT("1SX1278_TX_NIRQ%d\r\n",SX1278_Read_Reg(LR_RegIrqFlags));
      SX1278_Read_Reg(LR_RegIrqFlags);
      SX1278_LoRaClearIrq(); // Clear irq
                             // DEBUG_PRINT("2SX1278_TX_NIRQ%d\r\n",SX1278_Read_Reg(LR_RegIrqFlags));
      //			SX1278_Standby();                                     //Entry Standby mode
      SX1278_Sleep(); // ����˯��ģʽ
      return 0;
    }
  }

  DEBUG_PRINT("sb\r\n");
  SX1278_Sleep(); // ����˯��ģʽ
  return 1;
}

/*************************************************************************
 ���ܣ���ȡ�ź�ǿ��
 ��������
 ���أ��ź�ǿ�� 127���
***************************************************************************/
u8 SX1278_ReadRSSI(void)
{
  u8 temp = 0xff;

  temp = SX1278_Read_Reg(0x11);
  temp >>= 1;
  temp = 127 - temp; // 127:Max RSSI
  return temp;
}
/*************************************************************************
 ���ܣ�ģ���ʼ��
 ������freq[in]: �ز�Ƶ������ 428 ~ 439 ��λMHz
 ���أ���
***************************************************************************/
void SX1278_Init()
{
  //  SX1278_GPIO_Init();
  //  SX1278_SPI_Init(); // SPI��ʼ��
  //       SX1278_NSS_DISABLE;
  //       SET_SX1278_RST();
  //       SET_SX1278_MOSI();
  //       SET_SX1278_SCK();
  // if (freq >= 428 && freq <= 439) // �����÷�Χ�ڣ���Ȼ�Ͱ���Ĭ��Ƶ������
  // {
  //   Lora_Freq = freq % 428; // �����ز�Ƶ��
  // }
  CLR_SX1278_RST();
  SX1278_DelayMs(100);
  SET_SX1278_RST();
  SX1278_DelayMs(10); // ��λ����

  SX1278_Config();

  while (SX1278_Read_Reg(REG_LR_VERSION) != 0x12)
  {
    DEBUG_PRINT("wait sx1278 init \r\n");
    Delay_Ms(10);
  }
  DEBUG_PRINT("lora init ok=0x%X \r\n", SX1278_Read_Reg(REG_LR_VERSION)); // 0x89  0100 0101
}
