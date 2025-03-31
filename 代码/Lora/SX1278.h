/*****************************************************************************************************
 		  ������ �˴�ʹ�õ���ģ��SPI��ʽ�����ģ�ģ��ľͺã�ʹ��Ӳ��SPI��������
      ������ڣ� 2017-06-29
  ����޸����ڣ�	 
 		  ���ߣ�  Ф�� 
          ���ܣ�	 ���ڻ��΢ͨSX1278ģ���������ɱ䳤�����ݷ���(�����ϰ������Ծ���1.5KM����������)

 	 �޸�˵����	 
                 
*****************************************************************************************************/
/************************************************
//  RF module:           sx1276_7_8
//  LoRa:
//  Carry Frequency:     434MHz
//  Spreading Factor:    7/8/9/10/11/12
//  Tx Power Output:     20dbm/17dbm/14dbm/11dbm
//  Receive Bandwidth:   7.8KHz/10.4KHz/15.6KHz/20.8KHz/31.2KHz/41.7KHz/62.5KHz/125KHz/250KHz/500KHz
//  Coding:              NRZ
//  Tx Current:          about 120mA  (RFOP=+20dBm,typ.)
//  Rx Current:          about 11.5mA  (typ.)  
//  Packet_Length:       variable���ɱ䳤�ȵģ����256�ֽڣ�
**********************************************************/
#ifndef SX1278_H
#define SX1278_H

#include "debug.h"
#include "gpio.h"

//���òο����� 
/********************Laro***************************/
//Error Coding ��
#define ERROR_CODING_4_5    0x01
#define ERROR_CODING_4_6    0x02
#define ERROR_CODING_4_7    0x03
#define ERROR_CODING_4_8    0x04


//�ز�Ƶ�ʱ�
#define LORAFREQ_428MHZ LORAFREQ_433MHZ-4
#define LORAFREQ_429MHZ LORAFREQ_433MHZ-3
#define LORAFREQ_430MHZ LORAFREQ_433MHZ-3
#define LORAFREQ_431MHZ LORAFREQ_433MHZ-2
#define LORAFREQ_432MHZ LORAFREQ_433MHZ-1
#define LORAFREQ_433MHZ 5
#define LORAFREQ_434MHZ LORAFREQ_433MHZ+1
#define LORAFREQ_435MHZ LORAFREQ_433MHZ+2
#define LORAFREQ_436MHZ LORAFREQ_433MHZ+3
#define LORAFREQ_437MHZ LORAFREQ_433MHZ+4
#define LORAFREQ_438MHZ LORAFREQ_433MHZ+5
#define LORAFREQ_439MHZ LORAFREQ_433MHZ+6


//���书�ʶ���
#define LORAPOWER_20DBM 0xFF
#define LORAPOWER_17DBM 0xFC
#define LORAPOWER_14DBM 0xF9
#define LORAPOWER_11DBM 0xF6

//�������
#define LORABW_7_8KHz  0
#define LORABW_10_4KHz 1
#define LORABW_15_6KHz 2
#define LORABW_20_8KHz 3
#define LORABW_31_2KHz 4
#define LORABW_41_7KHz 5
#define LORABW_62_5KHz 6
#define LORABW_125KHz  7
#define LORABW_250KHz  8
#define LORABW_500KHz  9
/********************************************************/

 
#define LR_RegFifo                                  0x00 //FIFO��д���ʣ�����������˯��ģʽʱ��FIFO�����㣬�޷�����
// Common settings
#define LR_RegOpMode                                0x01 //����ģʽ Lora��FSKѡ��
#define LR_RegFrMsb                                 0x06 //RF�ز�Ƶ�������Чλ
#define LR_RegFrMid                                 0x07 //RF�ز�Ƶ���м���Чλ
#define LR_RegFrLsb                                 0x08 //RF�ز�Ƶ�������Чλ  ��F(XOSC)=32MHz����ֱ���Ϊ61.035Hz��
// Tx settings ��������
#define LR_RegPaConfig                              0x09 //PAѡ���������ʿ���
#define LR_RegPaRamp                                0x0A //PAб��/б��ʱ��͵�����PLL�Ŀ���
#define LR_RegOcp                                   0x0B //������������
// Rx settings ��������
#define LR_RegLna                                   0x0C //LNA����
// LoRa registers
#define LR_RegFifoAddrPtr                           0x0D //FIFO���ݻ�������SPI�ӿڵ�ַָ��
#define LR_RegFifoTxBaseAddr                        0x0E //FIFO���ݻ��������͵�������д�����ַ
#define LR_RegFifoRxBaseAddr                        0x0F //FIFO���ݻ��������յ������Ķ�ȡ����ַ
#define LR_RegFifoRxCurrentaddr                     0x10 //���յ����һ�����ݰ�����ʼ��ַ
#define LR_RegIrqFlagsMask                          0x11 //��ѡ�жϱ�־����
#define LR_RegIrqFlags                              0x12 //�жϱ�־
#define LR_RegRxNbBytes                             0x13 //���һ�ν��յ������ݰ��ĸ����ֽ���
#define LR_RegRxHeaderCntValueMsb                   0x14 //���һ��ת����Rxģʽ����յ���Ч��ͷ������λ��Ч��
#define LR_RegRxHeaderCntValueLsb                   0x15 //���һ��ת����Rxģʽ����յ���Ч��ͷ������λ��Ч��
#define LR_RegRxPacketCntValueMsb                   0x16 //���һ��ת����Rxģʽ����յ���Ч���ݰ�������λ��Ч��
#define LR_RegRxPacketCntValueLsb                   0x17 //���һ��ת����Rxģʽ����յ���Ч���ݰ�������λ��Ч��
#define LR_RegModemStat                             0x18 //LoRa���ƽ�����ֳ�״̬
#define LR_RegPktSnrValue                           0x19 //�����յ������ݰ���SNR(�����)Ԥ��ֵ
#define LR_RegPktRssiValue                          0x1A //�����յ������ݰ���RSSI��dBm�� RSSI[dBm]=-137+PacketRssi
#define LR_RegRssiValue                             0x1B //����RSSIֵ��dBm��RSSI[dBm]=-137+Rssi 
#define LR_RegHopChannel                            0x1C //FHSS��ʼ�ŵ�
#define LR_RegModemConfig1                          0x1D //���ƽ�������������1:7-4λ�źŴ���(0x07Ĭ��125kHz) 3-1λ���������(Ĭ��001->4/5) 0λ��ͷģʽ(0 0->��ʽ��ͷģʽ )
#define LR_RegModemConfig2                          0x1E //���ƽ�������������2:7-4λSFֵ(�� 2 ��������ʾ Ĭ��0x07->128 ��Ƭ/���� ) 3λ(0����ģʽ���͵������ݰ� 1����ģʽͨ��FIFO���Ͷ�����ݰ�(����Ƶ�׷���)) 2λ���Ͷ�CRC��Ϣ(0�ر�CRC 1����CRC) 1-0λRX��ʱ�����Чλ
#define LR_RegSymbTimeoutLsb                        0x1F //���ջ���ʱֵ,RX��ʱ�����ЧλRX������ʱֵ�Է�������ʾ��TimeOut=SymbTimeout-Ts 
#define LR_RegPreambleMsb                           0x20 //ǰ���볤�ȣ����λ��=PreambleLength+4.25 ����
#define LR_RegPreambleLsb                           0x21 //ǰ���볤�ȣ����λ��
#define LR_RegPayloadLength                         0x22 //LoRa���س���:��ʽ��ͷģʽ�� ��Ҫ���üĴ������ԴﵽԤ�ڵ� ���ݰ����ȡ��������Ĵ���ֵ ����Ϊ 0�� 
#define LR_RegMaxPayloadLength                      0x23 //LoRa���س������ֵ:�����ͷ���� ���ȳ��������ֵ���������� ͷ CRC ��������Գ��Ȳ��� ȷ�����ݰ����й��ˡ�
#define LR_RegHopPeriod                             0x24 //FHSS��Ƶ����:��0= �رգ�����һ�����Ƿ����ڵ�һ�� ��ͷ���ź�
#define LR_RegFifoRxByteAddr                        0x25 //FIFO�����д���ֽڵĵ�ַ,�������ݻ��浱ǰָ�루�� Lora ���ջ�д������һ���ֽڵĵ�ַ�� 
#define LR_RegModemConfig3                          0x26 //���ƽ�������������3
 
#define LR_RegPacketConfig2                         0x31 //RESERVED
#define LR_RegSeqConfig2                            0x37 //RESERVED
 
#define REG_LR_DIOMAPPING1                          0x40 //DIO0��DIO3����ӳ��
#define REG_LR_DIOMAPPING2                          0x41 //DIO4��DIO5����ӳ�䡢ClkOutƵ��
// Version
#define REG_LR_VERSION                              0x42 //оƬ�汾
// Additional settings
#define REG_LR_PLLHOP                               0x44 //unused
#define REG_LR_TCXO                                 0x4B //TCXO��XTAL��������
#define REG_LR_PADAC                                0x4D //PA���������� ���20dBm
#define REG_LR_FORMERTEMP                           0x5B //IQУ׼�ڼ�洢�¶�
#define REG_LR_BITRATEFRAC                          0x5D
#define REG_LR_AGCREF                               0x61 //AGC��ֵ����
#define REG_LR_AGCTHRESH1                           0x62 //AGC��ֵ����
#define REG_LR_AGCTHRESH2                           0x63 //AGC��ֵ����
#define REG_LR_AGCTHRESH3                           0x64 //AGC��ֵ����
#define REG_LR_PLL                                  0x70 //PLL�������





/*!
 * RegIrqFlags
 */
#define RFLR_IRQFLAGS_RXTIMEOUT                     0x80 
#define RFLR_IRQFLAGS_RXDONE                        0x40 
#define RFLR_IRQFLAGS_PAYLOADCRCERROR               0x20 
#define RFLR_IRQFLAGS_VALIDHEADER                   0x10 
#define RFLR_IRQFLAGS_TXDONE                        0x08 
#define RFLR_IRQFLAGS_CADDONE                       0x04 
#define RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL            0x02 
#define RFLR_IRQFLAGS_CADDETECTED                   0x01 

/********************FSK/ook mode***************************/
#define  RegFIFO  	 			0x00				//FIFO
#define  RegOpMode 	 			0x01				//����ģʽ
#define  RegBitRateMsb 		0x02				//BR ��λ
#define  RegBitRateLsb 		0x03				//BR ��λ
#define  RegFdevMsb	 			0x04				//FD ��λ
#define  RegFdevLsb	 			0x05 				//FD ��λ
#define  RegFreqMsb	 			0x06				//Freq ��λ
#define  RegFreqMid	 			0x07 				//Freq ��λ
#define  RegFreqLsb   		0x08				//Freq ��λ
#define	 RegPaConfig			0x09
#define  RegPaRamp				0x0a
#define  RegOcp						0x0b
#define  RegLna						0x0c
#define  RegRxConfig			0x0d
#define  RegRssiConfig		0x0e
#define  RegRssiCollision 0x0f
#define  RegRssiThresh		0x10
#define  RegRssiValue			0x11
#define  RegRxBw					0x12
#define  RegAfcBw					0x13
#define  RegOokPeak				0x14
#define  RegOokFix				0x15
#define  RegOokAvg				0x16

#define  RegAfcFei				0x1a	
#define  RegAfcMsb				0x1b
#define  RegAfcLsb				0x1c
#define  RegFeiMsb				0x1d
#define  RegFeiLsb				0x1e
#define  RegPreambleDetect	0x1f
#define  RegRxTimeout1		0x20
#define  RegRxTimeout2		0x21
#define  RegRxTimeout3		0x22
#define  RegRxDelay		    0x23
#define  RegOsc  	 		0x24				//OSC����
#define  RegPreambleMsb		0x25
#define  RegPreambleLsb		0x26
#define  RegSyncConfig		0x27
#define  RegSyncValue1		0x28
#define  RegSyncValue2		0x29
#define  RegSyncValue3		0x2a
#define  RegSyncValue4		0x2b
#define  RegSyncValue5		0x2c
#define  RegSyncValue6		0x2d
#define  RegSyncValue7		0x2e
#define  RegSyncValue8		0x2f
#define  RegPacketConfig1		0x30
#define  RegPacketConfig2		0x31
#define  RegPayloadLength		0x32
#define  RegNodeAdrs			0x33
#define  RegBroadcastAdrs		0x34
#define  RegFifoThresh		0x35
#define  RegSeqConfig1		0x36
#define  RegSeqConfig2		0x37
#define  RegTimerResol		0x38
#define  RegTimer1Coef		0x39
#define  RegTimer2Coef		0x3a
#define  RegImageCal			0x3b
#define  RegTemp			    0x3c
#define  RegLowBat				0x3d
#define  RegIrqFlags1			0x3e
#define  RegIrqFlags2			0x3f
#define  RegDioMapping1		0x40
#define  RegDioMapping2		0x41
#define  RegVersion				0x42

#define	 RegPllHop				0x44
#define  RegPaDac					0x4d
#define	 RegBitRateFrac		0x5d
/*********************************************************/
//command
/*********************************************************/
void SX1278_LoRaClearIrq(void);
void SX1278_Sleep(void);
void SX1278_Burst_Read(u8 adr, u8 *ptr, u8 length);
u8 SX1278_LoRaEntryRx(void);

u8 SX1278_Read_Reg(u8 adr);
void SX1278_Standby(void);
u8 SX1278_SPI_RW(u8 byte);
extern void SX1278_Init();
//extern void SX1278_Config(void);
//extern u8   SX1278_LoRaEntryRx(void); 
void SX1278_Write_Reg(u8 adr, u8 WrPara);
extern u8   SX1278_LoRaReadRSSI(void);
u8 SX1278_ReadRSSI(void);
extern u8 SX1278_LoRaRxPacket(u8 *valid_data, u16 timeout);
//extern u8   SX1278_LoRaEntryTx(u8 packet_length);
extern u8   SX1278_LoRaTxPacket(u8 *valid_data, u8 packet_length);
void SX1278_Receive() ;


#endif /* SX1278_H */
