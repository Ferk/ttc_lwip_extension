/*{ stm32f10x_it.h ***********************************************
 *
 * TheToolChain.com
 *
 * Written by Gregor Rebel 2010-2012
 *
 * Interrupt handler definitions
 * Place a call to your interrupt-handling right here. 
 * 
}*/
#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f10x.h"
#include "ttc_basic.h"

//void Reset_Handler();
void NMI_Handler();
void HardFault_Handler();
void MemManage_Handler();
void BusFault_Handler();
void UsageFault_Handler();
void SVC_Handler();
void DebugMon_Handler();
void PendSV_Handler();
void SysTick_Handler();
void WWDG_IRQHandler();
void PVD_IRQHandler();
void TAMPER_IRQHandler();
void RTC_IRQHandler();
void FLASH_IRQHandler();
void RCC_IRQHandler();
void EXTI0_IRQHandler();
void EXTI1_IRQHandler();
void EXTI2_IRQHandler();
void EXTI3_IRQHandler();
void EXTI4_IRQHandler();
void DMA1_Channel1_IRQHandler();
void DMA1_Channel2_IRQHandler();
void DMA1_Channel3_IRQHandler();
void DMA1_Channel4_IRQHandler();
void DMA1_Channel5_IRQHandler();
void DMA1_Channel6_IRQHandler();
void DMA1_Channel7_IRQHandler();
void ADC1_2_IRQHandler();
void CAN1_TX_IRQHandler();
void CAN1_RX0_IRQHandler();
void CAN1_RX1_IRQHandler();
void CAN1_SCE_IRQHandler();
void EXTI9_5_IRQHandler();
void TIM1_BRK_IRQHandler();
void TIM1_UP_IRQHandler();
void TIM1_TRG_COM_IRQHandler();
void TIM1_CC_IRQHandler();
void TIM2_IRQHandler();
void TIM3_IRQHandler();
void TIM4_IRQHandler();
void I2C1_EV_IRQHandler();
void I2C1_ER_IRQHandler();
void I2C2_EV_IRQHandler();
void I2C2_ER_IRQHandler();
void SPI1_IRQHandler();
void SPI2_IRQHandler();
void USART1_IRQHandler();
void USART2_IRQHandler();
void USART3_IRQHandler();
void EXTI15_10_IRQHandler();
void RTCAlarm_IRQHandler();
void OTG_FS_WKUP_IRQHandler();
void TIM5_IRQHandler();
void SPI3_IRQHandler();
void UART4_IRQHandler();
void UART5_IRQHandler();
void TIM6_IRQHandler();
void TIM7_IRQHandler();
void DMA2_Channel1_IRQHandler();
void DMA2_Channel2_IRQHandler();
void DMA2_Channel3_IRQHandler();
void DMA2_Channel4_IRQHandler();
void DMA2_Channel5_IRQHandler();
void ETH_IRQHandler();
void ETH_WKUP_IRQHandler();
void CAN2_TX_IRQHandler();
void CAN2_RX0_IRQHandler();
void CAN2_RX1_IRQHandler();
void CAN2_SCE_IRQHandler();
void OTG_FS_IRQHandler();
void BootRAM();

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_IT_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
