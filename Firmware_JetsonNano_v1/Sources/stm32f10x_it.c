/**
  ******************************************************************************
  * @file    IWDG/IWDG_Reset/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "modbus.h"
#include "main.h"
#include "mb_regs.h"

uint16_t temp_cnt[400];
void USART1_IRQHandler(void)
{ 
  uint8_t temp = 0;  
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    temp = USART_ReceiveData(USART1);
    if (mb.u16InCnt < BUFFER_RX_SIZE)
    {      
      temp_cnt[mb.u16InCnt] = TIM_GetCounter(TIM2);
      mb.u8BufferIn[mb.u16InCnt++] = temp;
      TIM_SetCounter(TIM2, 0);
      TIM_ClearFlag(TIM2, TIM_IT_Update);
      TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    }

    /*if(RxCounter == NbrOfDataToRead)
    {
      USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    }*/
  }  
  USART_ClearFlag(USART1, USART_IT_ORE);
  USART_ClearFlag(USART1, USART_IT_TXE);
  //led_toggle();
}

uint32_t temp_tim2 = 0;
void TIM2_IRQHandler(void)
{
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); 
  mb.flag |= 1;  
  //led_toggle();
}
    
void TIM4_IRQHandler(void)
{
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  timer50hz++;
  if (toggle50hz) 
    toggle50hz = 0;
  else 
    toggle50hz = 1;
  
  if ((timer50hz % 50) == 0) 
  {
    BoardBackTimer();
    timer1hz++;
    if (toggle50hz) 
      toggle50hz = 0;
    else 
      toggle50hz = 1;
    led_toggle();
    mb.registers.one[mbREG_work_tim_s]++;
  }
  
  uint8_t ppmerror = 0;
  for (uint8_t ch = 0; ch < 3; ch++)
  {
    if (inppm.lenght[ch][0] > inppm.lenght[ch][1])
      inppm.lenght[ch][1] += 2000;
    uint16_t tmp_ppm = inppm.lenght[ch][1] - inppm.lenght[ch][0];
    if ((tmp_ppm > 205) || (tmp_ppm < 95) || (tmp_ppm == 0))
      ppmerror = 1;
    else
      inppm.out[ch] = tmp_ppm;
    inppm.lenght[ch][0] = 0;
    inppm.lenght[ch][1] = 0;
  }
  if ((inppm.out[0] > 149) || (ppmerror > 0))
  {
    if (inppm.valid > 0)
      inppm.valid--;
  }
  else
    inppm.valid = 25;
    
}

void DMA1_Channel4_IRQHandler(void)
{
  DMA_ClearFlag(DMA1_IT_TC4);  
  DMA_Cmd(DMA1_Channel4, DISABLE);
  USART_ClearFlag(USART1, USART_IT_RXNE);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  mb.u16InCnt = 0;
  //led_toggle();
}

void EXTI15_10_IRQHandler(void)
{
  if (EXTI_GetFlagStatus(EXTI_Line10))
  {
    EXTI_ClearFlag(EXTI_Line10);
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 1)
      inppm.lenght[0][0] = TIM4->CNT;
    else
      inppm.lenght[0][1] = TIM4->CNT;
  }
  if (EXTI_GetFlagStatus(EXTI_Line11))
  {
    EXTI_ClearFlag(EXTI_Line11);
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 1)
      inppm.lenght[1][0] = TIM4->CNT;
    else
      inppm.lenght[1][1] = TIM4->CNT;
  }
  
  if (EXTI_GetFlagStatus(EXTI_Line12))
  {
    EXTI_ClearFlag(EXTI_Line12);
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1)
      inppm.lenght[2][0] = TIM4->CNT;
    else
      inppm.lenght[2][1] = TIM4->CNT;
  }
}


void TIM1_CC_IRQHandler(void)
{
  TIM_ClearFlag(TIM1, TIM_DIER_CC1IE);
  led_toggle();
}

void ADC1_2_IRQHandler(void)
{  
   ADC_ClearFlag(ADC1, ADC_IT_EOC);
   led_toggle();
}


void DMA1_Channel1_IRQHandler(void)
{
  if (DMA_GetITStatus(DMA1_IT_TC1) == SET)
  {
    adc_flag &=~ 1;
    adc_flag |= 2;
    DMA_ClearFlag(DMA1_IT_TC1);  
    //led_toggle();
  }
  if (DMA_GetITStatus(DMA1_IT_HT1) == SET)
  {
    adc_flag |= 1;
    adc_flag |= 2;
    DMA_ClearFlag(DMA1_IT_HT1);  
    //led_toggle();
  }
}

void DMA1_Channel2_IRQHandler(void)
{
  DMA_ClearFlag(DMA1_IT_TC1);     
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}