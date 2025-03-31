/*
 * time.h
 *
 *  Created on: 2024��9��1��
 *      Author: 12630
 */

#ifndef TIME_TIME_APP_H_
#define TIME_TIME_APP_H_
#include "ch32v00x_rcc.h"

void TIM1_Init(u16 arr, u16 psc, u16 ccp);
void TIM2_Init(u16 arr, u16 psc);
void IWDG_Feed_Init(u16 prer, u16 rlr);//���Ź�
void system_wokeup();
void TIM1_DeInit(void);
void TIM2_DeInit(void);
void AWU_Init();
#endif /* TIME_TIME_APP_H_ */
