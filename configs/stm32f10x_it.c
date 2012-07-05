/*{ stm32f10x_it.c ***********************************************
 *
 * TheToolChain.com
 *
 * Written by Gregor Rebel 2010-2012
 *
 * Interrupt handler definitions
 * Place a call to your interrupt-handling right here. 
 * 
}*/
#include "stm32f10x_it.h"

#include "ttc_gpio.h"
#include "ttc_basic.h"

/* void Reset_Handler() {
  // write your own reset-handler here...
  
  
  Assert(0, ec_UNKNOWN);
} */
void NMI_Handler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void HardFault_Handler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void MemManage_Handler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void BusFault_Handler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void UsageFault_Handler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void SVC_Handler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DebugMon_Handler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void PendSV_Handler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void SysTick_Handler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void WWDG_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void PVD_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void TAMPER_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void RTC_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void FLASH_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void RCC_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void EXTI0_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void EXTI1_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void EXTI2_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void EXTI3_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void EXTI4_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DMA1_Channel1_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DMA1_Channel2_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DMA1_Channel3_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DMA1_Channel4_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DMA1_Channel5_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DMA1_Channel6_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DMA1_Channel7_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void ADC1_2_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void CAN1_TX_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void CAN1_RX0_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void CAN1_RX1_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void CAN1_SCE_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void EXTI9_5_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void TIM1_BRK_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void TIM1_UP_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void TIM1_TRG_COM_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void TIM1_CC_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void TIM2_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void TIM3_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void TIM4_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void I2C1_EV_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void I2C1_ER_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void I2C2_EV_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void I2C2_ER_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void SPI1_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void SPI2_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void USART1_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void USART2_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void USART3_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void EXTI15_10_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void RTCAlarm_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void OTG_FS_WKUP_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void TIM5_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void SPI3_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void UART4_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void UART5_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void TIM6_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void TIM7_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DMA2_Channel1_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DMA2_Channel2_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DMA2_Channel3_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DMA2_Channel4_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void DMA2_Channel5_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void ETH_IRQHandler() {
  // call your interrupt-handler right here, or else ...
#ifdef TTC_LED2
  extern ttc_Port_t Led1Port;
  ttc_portSet(&Led1Port);
#endif /* TTC_LED2 */
  mSleep(1000);
#ifdef TTC_LED2
  ttc_portClr(&Led1Port);
#endif /* TTC_LED2 */
  mSleep(500);
}
void ETH_WKUP_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void CAN2_TX_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void CAN2_RX0_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void CAN2_RX1_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void CAN2_SCE_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void OTG_FS_IRQHandler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void BootRAM() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
