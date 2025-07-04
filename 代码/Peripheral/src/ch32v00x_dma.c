/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_dma.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : This file provides all the DMA firmware functions.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#include <ch32v00x_dma.h>
#include <ch32v00x_rcc.h>

/* DMA1 Channelx interrupt pending bit masks */
#define DMA1_Channel1_IT_Mask    ((uint32_t)(DMA_GIF1 | DMA_TCIF1 | DMA_HTIF1 | DMA_TEIF1))
#define DMA1_Channel2_IT_Mask    ((uint32_t)(DMA_GIF2 | DMA_TCIF2 | DMA_HTIF2 | DMA_TEIF2))
#define DMA1_Channel3_IT_Mask    ((uint32_t)(DMA_GIF3 | DMA_TCIF3 | DMA_HTIF3 | DMA_TEIF3))
#define DMA1_Channel4_IT_Mask    ((uint32_t)(DMA_GIF4 | DMA_TCIF4 | DMA_HTIF4 | DMA_TEIF4))
#define DMA1_Channel5_IT_Mask    ((uint32_t)(DMA_GIF5 | DMA_TCIF5 | DMA_HTIF5 | DMA_TEIF5))
#define DMA1_Channel6_IT_Mask    ((uint32_t)(DMA_GIF6 | DMA_TCIF6 | DMA_HTIF6 | DMA_TEIF6))
#define DMA1_Channel7_IT_Mask    ((uint32_t)(DMA_GIF7 | DMA_TCIF7 | DMA_HTIF7 | DMA_TEIF7))

/* DMA2 FLAG mask */
#define FLAG_Mask                ((uint32_t)0x10000000)

/* DMA registers Masks */
#define CFGR_CLEAR_Mask          ((uint32_t)0xFFFF800F)

/*********************************************************************
 * @fn      DMA_DeInit
 *
 * @brief   Deinitializes the DMAy Channelx registers to their default
 *        reset values.
 *
 * @param   DMAy_Channelx - here y can be 1 to select the DMA and x can be
 *        1 to 7 for DMA1 to select the DMA Channel.
 *
 * @return  none
 */
void DMA_DeInit(DMA_Channel_TypeDef *DMAy_Channelx)
{
    DMAy_Channelx->CFGR &= (uint16_t)(~DMA_CFGR1_EN);
    DMAy_Channelx->CFGR = 0;
    DMAy_Channelx->CNTR = 0;
    DMAy_Channelx->PADDR = 0;
    DMAy_Channelx->MADDR = 0;
    if(DMAy_Channelx == DMA1_Channel1)
    {
        DMA1->INTFCR |= DMA1_Channel1_IT_Mask;
    }
    else if(DMAy_Channelx == DMA1_Channel2)
    {
        DMA1->INTFCR |= DMA1_Channel2_IT_Mask;
    }
    else if(DMAy_Channelx == DMA1_Channel3)
    {
        DMA1->INTFCR |= DMA1_Channel3_IT_Mask;
    }
    else if(DMAy_Channelx == DMA1_Channel4)
    {
        DMA1->INTFCR |= DMA1_Channel4_IT_Mask;
    }
    else if(DMAy_Channelx == DMA1_Channel5)
    {
        DMA1->INTFCR |= DMA1_Channel5_IT_Mask;
    }
    else if(DMAy_Channelx == DMA1_Channel6)
    {
        DMA1->INTFCR |= DMA1_Channel6_IT_Mask;
    }
    else if(DMAy_Channelx == DMA1_Channel7)
    {
        DMA1->INTFCR |= DMA1_Channel7_IT_Mask;
    }
}

/*********************************************************************
 * @fn      DMA_Init
 *
 * @brief   Initializes the DMAy Channelx according to the specified
 *        parameters in the DMA_InitStruct.
 *
 * @param   DMAy_Channelx - here y can be 1 to select the DMA and x can be
 *        1 to 7 for DMA1 to select the DMA Channel.
 *          DMA_InitStruct - pointer to a DMA_InitTypeDef structure that contains
 *        contains the configuration information for the specified DMA Channel.
 *
 * @return  none
 */
void DMA_Init(DMA_Channel_TypeDef *DMAy_Channelx, DMA_InitTypeDef *DMA_InitStruct)
{
    uint32_t tmpreg = 0;

    tmpreg = DMAy_Channelx->CFGR;
    tmpreg &= CFGR_CLEAR_Mask;
    tmpreg |= DMA_InitStruct->DMA_DIR | DMA_InitStruct->DMA_Mode |
              DMA_InitStruct->DMA_PeripheralInc | DMA_InitStruct->DMA_MemoryInc |
              DMA_InitStruct->DMA_PeripheralDataSize | DMA_InitStruct->DMA_MemoryDataSize |
              DMA_InitStruct->DMA_Priority | DMA_InitStruct->DMA_M2M;


    DMAy_Channelx->CFGR = tmpreg;
    DMAy_Channelx->CNTR = DMA_InitStruct->DMA_BufferSize;
    DMAy_Channelx->PADDR = DMA_InitStruct->DMA_PeripheralBaseAddr;
    DMAy_Channelx->MADDR = DMA_InitStruct->DMA_MemoryBaseAddr;
}

/*********************************************************************
 * @fn      DMA_StructInit
 *
 * @brief   Fills each DMA_InitStruct member with its default value.
 *
 * @param   DMAy_Channelx - here y can be 1 to select the DMA and x can be
 *        1 to 7 for DMA1 to select the DMA Channel.
 *          DMA_InitStruct - pointer to a DMA_InitTypeDef structure that contains
 *        contains the configuration information for the specified DMA Channel.
 *
 * @return  none
 */
void DMA_StructInit(DMA_InitTypeDef *DMA_InitStruct)
{
    DMA_InitStruct->DMA_PeripheralBaseAddr = 0;
    DMA_InitStruct->DMA_MemoryBaseAddr = 0;
    DMA_InitStruct->DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct->DMA_BufferSize = 0;
    DMA_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct->DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct->DMA_Priority = DMA_Priority_Low;
    DMA_InitStruct->DMA_M2M = DMA_M2M_Disable;
}

/*********************************************************************
 * @fn      DMA_Cmd
 *
 * @brief   Enables or disables the specified DMAy Channelx.
 *
 * @param   DMAy_Channelx - here y can be 1 to select the DMA and x can be
 *        1 to 7 for DMA1 to select the DMA Channel.
 *          NewState - new state of the DMAy Channelx(ENABLE or DISABLE).
 *
 * @return  none
 */
void DMA_Cmd(DMA_Channel_TypeDef *DMAy_Channelx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        DMAy_Channelx->CFGR |= DMA_CFGR1_EN;
    }
    else
    {
        DMAy_Channelx->CFGR &= (uint16_t)(~DMA_CFGR1_EN);
    }
}

/*********************************************************************
 * @fn      DMA_ITConfig
 *
 * @brief   Enables or disables the specified DMAy Channelx interrupts.
 *
 * @param   DMAy_Channelx - here y can be 1 to select the DMA and x can be
 *        1 to 7 for DMA1 to select the DMA Channel.
 *          DMA_IT - specifies the DMA interrupts sources to be enabled
 *        or disabled.
 *           DMA_IT_TC - Transfer complete interrupt mask
 *           DMA_IT_HT - Half transfer interrupt mask
 *           DMA_IT_TE -  Transfer error interrupt mask
 *          NewState - new state of the DMAy Channelx(ENABLE or DISABLE).
 *
 * @return  none
 */
void DMA_ITConfig(DMA_Channel_TypeDef *DMAy_Channelx, uint32_t DMA_IT, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        DMAy_Channelx->CFGR |= DMA_IT;
    }
    else
    {
        DMAy_Channelx->CFGR &= ~DMA_IT;
    }
}

/*********************************************************************
 * @fn      DMA_SetCurrDataCounter
 *
 * @brief   Sets the number of data units in the current DMAy Channelx transfer.
 *
 * @param   DMAy_Channelx - here y can be 1 to select the DMA and x can be
 *        1 to 7 for DMA1 to select the DMA Channel.
 *          DataNumber - The number of data units in the current DMAy Channelx
 *        transfer.
 *
 * @return  none
 */
void DMA_SetCurrDataCounter(DMA_Channel_TypeDef *DMAy_Channelx, uint16_t DataNumber)
{
    DMAy_Channelx->CNTR = DataNumber;
}

/*********************************************************************
 * @fn      DMA_GetCurrDataCounter
 *
 * @brief   Returns the number of remaining data units in the current
 *        DMAy Channelx transfer.
 *
 * @param   DMAy_Channelx - here y can be 1 to select the DMA and x can be
 *        1 to 7 for DMA1 to select the DMA Channel.
 *
 * @return  DataNumber - The number of remaining data units in the current
 *        DMAy Channelx transfer.
 */
uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef *DMAy_Channelx)
{
    return ((uint16_t)(DMAy_Channelx->CNTR));
}

/*********************************************************************
 * @fn      DMA_GetFlagStatus
 *
 * @brief   Checks whether the specified DMAy Channelx flag is set or not.
 *
 * @param   DMAy_FLAG - specifies the flag to check.
 *            DMA1_FLAG_GL1 - DMA1 Channel1 global flag.
 *            DMA1_FLAG_TC1 - DMA1 Channel1 transfer complete flag.
 *            DMA1_FLAG_HT1 - DMA1 Channel1 half transfer flag.
 *            DMA1_FLAG_TE1 - DMA1 Channel1 transfer error flag.
 *            DMA1_FLAG_GL2 - DMA1 Channel2 global flag.
 *            DMA1_FLAG_TC2 - DMA1 Channel2 transfer complete flag.
 *            DMA1_FLAG_HT2 - DMA1 Channel2 half transfer flag.
 *            DMA1_FLAG_TE2 - DMA1 Channel2 transfer error flag.
 *            DMA1_FLAG_GL3 - DMA1 Channel3 global flag.
 *            DMA1_FLAG_TC3 - DMA1 Channel3 transfer complete flag.
 *            DMA1_FLAG_HT3 - DMA1 Channel3 half transfer flag.
 *            DMA1_FLAG_TE3 - DMA1 Channel3 transfer error flag.
 *            DMA1_FLAG_GL4 - DMA1 Channel4 global flag.
 *            DMA1_FLAG_TC4 - DMA1 Channel4 transfer complete flag.
 *            DMA1_FLAG_HT4 - DMA1 Channel4 half transfer flag.
 *            DMA1_FLAG_TE4 - DMA1 Channel4 transfer error flag.
 *            DMA1_FLAG_GL5 - DMA1 Channel5 global flag.
 *            DMA1_FLAG_TC5 - DMA1 Channel5 transfer complete flag.
 *            DMA1_FLAG_HT5 - DMA1 Channel5 half transfer flag.
 *            DMA1_FLAG_TE5 - DMA1 Channel5 transfer error flag.
 *            DMA1_FLAG_GL6 - DMA1 Channel6 global flag.
 *            DMA1_FLAG_TC6 - DMA1 Channel6 transfer complete flag.
 *            DMA1_FLAG_HT6 - DMA1 Channel6 half transfer flag.
 *            DMA1_FLAG_TE6 - DMA1 Channel6 transfer error flag.
 *            DMA1_FLAG_GL7 - DMA1 Channel7 global flag.
 *            DMA1_FLAG_TC7 - DMA1 Channel7 transfer complete flag.
 *            DMA1_FLAG_HT7 - DMA1 Channel7 half transfer flag.
 *            DMA1_FLAG_TE7 - DMA1 Channel7 transfer error flag.
 *
 * @return  The new state of DMAy_FLAG (SET or RESET).
 */
FlagStatus DMA_GetFlagStatus(uint32_t DMAy_FLAG)
{
    FlagStatus bitstatus = RESET;
    uint32_t   tmpreg = 0;

    tmpreg = DMA1->INTFR;

    if((tmpreg & DMAy_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/*********************************************************************
 * @fn      DMA_ClearFlag
 *
 * @brief   Clears the DMAy Channelx's pending flags.
 *
 * @param   DMAy_FLAG - specifies the flag to check.
 *            DMA1_FLAG_GL1 - DMA1 Channel1 global flag.
 *            DMA1_FLAG_TC1 - DMA1 Channel1 transfer complete flag.
 *            DMA1_FLAG_HT1 - DMA1 Channel1 half transfer flag.
 *            DMA1_FLAG_TE1 - DMA1 Channel1 transfer error flag.
 *            DMA1_FLAG_GL2 - DMA1 Channel2 global flag.
 *            DMA1_FLAG_TC2 - DMA1 Channel2 transfer complete flag.
 *            DMA1_FLAG_HT2 - DMA1 Channel2 half transfer flag.
 *            DMA1_FLAG_TE2 - DMA1 Channel2 transfer error flag.
 *            DMA1_FLAG_GL3 - DMA1 Channel3 global flag.
 *            DMA1_FLAG_TC3 - DMA1 Channel3 transfer complete flag.
 *            DMA1_FLAG_HT3 - DMA1 Channel3 half transfer flag.
 *            DMA1_FLAG_TE3 - DMA1 Channel3 transfer error flag.
 *            DMA1_FLAG_GL4 - DMA1 Channel4 global flag.
 *            DMA1_FLAG_TC4 - DMA1 Channel4 transfer complete flag.
 *            DMA1_FLAG_HT4 - DMA1 Channel4 half transfer flag.
 *            DMA1_FLAG_TE4 - DMA1 Channel4 transfer error flag.
 *            DMA1_FLAG_GL5 - DMA1 Channel5 global flag.
 *            DMA1_FLAG_TC5 - DMA1 Channel5 transfer complete flag.
 *            DMA1_FLAG_HT5 - DMA1 Channel5 half transfer flag.
 *            DMA1_FLAG_TE5 - DMA1 Channel5 transfer error flag.
 *            DMA1_FLAG_GL6 - DMA1 Channel6 global flag.
 *            DMA1_FLAG_TC6 - DMA1 Channel6 transfer complete flag.
 *            DMA1_FLAG_HT6 - DMA1 Channel6 half transfer flag.
 *            DMA1_FLAG_TE6 - DMA1 Channel6 transfer error flag.
 *            DMA1_FLAG_GL7 - DMA1 Channel7 global flag.
 *            DMA1_FLAG_TC7 - DMA1 Channel7 transfer complete flag.
 *            DMA1_FLAG_HT7 - DMA1 Channel7 half transfer flag.
 *            DMA1_FLAG_TE7 - DMA1 Channel7 transfer error flag.
 *
 * @return  none
 */
void DMA_ClearFlag(uint32_t DMAy_FLAG)
{

    DMA1->INTFCR = DMAy_FLAG;

}

/*********************************************************************
 * @fn      DMA_GetITStatus
 *
 * @brief   Checks whether the specified DMAy Channelx interrupt has
 *        occurred or not.
 *
 * @param   DMAy_IT - specifies the DMAy interrupt source to check.
 *            DMA1_IT_GL1 - DMA1 Channel1 global flag.
 *            DMA1_IT_TC1 - DMA1 Channel1 transfer complete flag.
 *            DMA1_IT_HT1 - DMA1 Channel1 half transfer flag.
 *            DMA1_IT_TE1 - DMA1 Channel1 transfer error flag.
 *            DMA1_IT_GL2 - DMA1 Channel2 global flag.
 *            DMA1_IT_TC2 - DMA1 Channel2 transfer complete flag.
 *            DMA1_IT_HT2 - DMA1 Channel2 half transfer flag.
 *            DMA1_IT_TE2 - DMA1 Channel2 transfer error flag.
 *            DMA1_IT_GL3 - DMA1 Channel3 global flag.
 *            DMA1_IT_TC3 - DMA1 Channel3 transfer complete flag.
 *            DMA1_IT_HT3 - DMA1 Channel3 half transfer flag.
 *            DMA1_IT_TE3 - DMA1 Channel3 transfer error flag.
 *            DMA1_IT_GL4 - DMA1 Channel4 global flag.
 *            DMA1_IT_TC4 - DMA1 Channel4 transfer complete flag.
 *            DMA1_IT_HT4 - DMA1 Channel4 half transfer flag.
 *            DMA1_IT_TE4 - DMA1 Channel4 transfer error flag.
 *            DMA1_IT_GL5 - DMA1 Channel5 global flag.
 *            DMA1_IT_TC5 - DMA1 Channel5 transfer complete flag.
 *            DMA1_IT_HT5 - DMA1 Channel5 half transfer flag.
 *            DMA1_IT_TE5 - DMA1 Channel5 transfer error flag.
 *            DMA1_IT_GL6 - DMA1 Channel6 global flag.
 *            DMA1_IT_TC6 - DMA1 Channel6 transfer complete flag.
 *            DMA1_IT_HT6 - DMA1 Channel6 half transfer flag.
 *            DMA1_IT_TE6 - DMA1 Channel6 transfer error flag.
 *            DMA1_IT_GL7 - DMA1 Channel7 global flag.
 *            DMA1_IT_TC7 - DMA1 Channel7 transfer complete flag.
 *            DMA1_IT_HT7 - DMA1 Channel7 half transfer flag.
 *            DMA1_IT_TE7 - DMA1 Channel7 transfer error flag.
 *
 * @return  The new state of DMAy_IT (SET or RESET).
 */
ITStatus DMA_GetITStatus(uint32_t DMAy_IT)
{
    ITStatus bitstatus = RESET;
    uint32_t tmpreg = 0;

    tmpreg = DMA1->INTFR;

    if((tmpreg & DMAy_IT) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

/*********************************************************************
 * @fn      DMA_ClearITPendingBit
 *
 * @brief   Clears the DMAy Channelx's interrupt pending bits.
 *
 * @param   DMAy_IT - specifies the DMAy interrupt source to check.
 *            DMA1_IT_GL1 - DMA1 Channel1 global flag.
 *            DMA1_IT_TC1 - DMA1 Channel1 transfer complete flag.
 *            DMA1_IT_HT1 - DMA1 Channel1 half transfer flag.
 *            DMA1_IT_TE1 - DMA1 Channel1 transfer error flag.
 *            DMA1_IT_GL2 - DMA1 Channel2 global flag.
 *            DMA1_IT_TC2 - DMA1 Channel2 transfer complete flag.
 *            DMA1_IT_HT2 - DMA1 Channel2 half transfer flag.
 *            DMA1_IT_TE2 - DMA1 Channel2 transfer error flag.
 *            DMA1_IT_GL3 - DMA1 Channel3 global flag.
 *            DMA1_IT_TC3 - DMA1 Channel3 transfer complete flag.
 *            DMA1_IT_HT3 - DMA1 Channel3 half transfer flag.
 *            DMA1_IT_TE3 - DMA1 Channel3 transfer error flag.
 *            DMA1_IT_GL4 - DMA1 Channel4 global flag.
 *            DMA1_IT_TC4 - DMA1 Channel4 transfer complete flag.
 *            DMA1_IT_HT4 - DMA1 Channel4 half transfer flag.
 *            DMA1_IT_TE4 - DMA1 Channel4 transfer error flag.
 *            DMA1_IT_GL5 - DMA1 Channel5 global flag.
 *            DMA1_IT_TC5 - DMA1 Channel5 transfer complete flag.
 *            DMA1_IT_HT5 - DMA1 Channel5 half transfer flag.
 *            DMA1_IT_TE5 - DMA1 Channel5 transfer error flag.
 *            DMA1_IT_GL6 - DMA1 Channel6 global flag.
 *            DMA1_IT_TC6 - DMA1 Channel6 transfer complete flag.
 *            DMA1_IT_HT6 - DMA1 Channel6 half transfer flag.
 *            DMA1_IT_TE6 - DMA1 Channel6 transfer error flag.
 *            DMA1_IT_GL7 - DMA1 Channel7 global flag.
 *            DMA1_IT_TC7 - DMA1 Channel7 transfer complete flag.
 *            DMA1_IT_HT7 - DMA1 Channel7 half transfer flag.
 *            DMA1_IT_TE7 - DMA1 Channel7 transfer error flag.
 *
 * @return  none
 */
void DMA_ClearITPendingBit(uint32_t DMAy_IT)
{
    DMA1->INTFCR = DMAy_IT;
}
