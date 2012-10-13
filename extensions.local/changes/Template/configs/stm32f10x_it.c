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
#include "ttc_basic.h"

#ifdef EXTENSION_600_example_lwip
#include "example_lwip.h"
#endif


void FaultStack_Check(u32_t* fault_stack);

/* void Reset_Handler() {
  // write your own reset-handler here...
  
  
  Assert(0, ec_UNKNOWN);
} */
void NMI_Handler() {
  // call your interrupt-handler right here, or else ...
  
  
  Assert(0, ec_UNKNOWN);
}
void HardFault_Handler(void) {
  // Default Fault Handler when others are not activated
  FaultStack_Check(0);
  /* Go to infinite loop when Hard Fault exception occurs */
  Assert(0, ec_UNKNOWN);
}
void MemManage_Handler() {
  // Attemped to access unprivileged memory!
  FaultStack_Check(0);
  /* Go to infinite loop when Memory Management Fault occurs */
  Assert(0, ec_UNKNOWN);
}
void BusFault_Handler() {
  // Attempted to access an invalid or offline memory region!
  FaultStack_Check(0);
  /* Go to infinite loop when Bus Fault occurs */
  Assert(0, ec_UNKNOWN);
}
void UsageFault_Handler() {
  // undefined instruction or some other unaligned access!
  FaultStack_Check(0);
  /* Go to infinite loop when Usage Fault occurs */
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
/**
  * @brief  This function handles ETH interrupt request.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void)
{
#ifdef EXTENSION_600_example_lwip
    example_lwip_package_handler();
    return;
#endif
    Assert(0,ec_UNKNOWN);
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


void FaultStack_Check(u32_t * fault_stack) {
#ifdef TARGET_ARCHITECTURE_STM32 

  // If no stack was passed, retrieve it
  if( fault_stack == 0 ) {
      __asm__ volatile
	  ( // Assembly code to retrieve register information
	   " TST LR, #4    \n" // Substract 4 from LR (will obtain the faulty instruction)
	   " ITE EQ        \n" // If EQ
	   " MRSEQ R0, MSP \n" // Then.. move Main Stack    to R0
	   " MRSNE R0, PSP \n" // Else.. move Process Stack to R0
	   " B FaultStack_Check " // jump back to function (with the stack as parameter)
	    );
  }

  u32_t R0  = fault_stack[0]; // R0-R12: General-purpose
  u32_t R1  = fault_stack[1];
  u32_t R2  = fault_stack[2];
  u32_t R3  = fault_stack[3];
  u32_t R12 = fault_stack[4];
  u32_t LR  = fault_stack[5]; // Link Register   (R14)
  u32_t PC  = fault_stack[6]; // Program Counter (R15)
  u32_t PSR = fault_stack[7]; // Program Status Register
	      
  // See: http://infocenter.arm.com/help/topic/com.arm.doc.dui0552a/CIHFDJCA.html

  struct { // Configurable Fault Status Register

      struct { // Memory Management Fault Status Register (8b)
	  unsigned IACCVIOL  : 1; // =1: Instruction access violation
	  unsigned DACCVIOL  : 1; // =1: Data access violation
	  unsigned reserved2 : 1;
	  unsigned UNMSTKERR : 1; // =1: MemManage fault on unstacking for a return from exception
	  unsigned MSTKERR   : 1; // =1: MemManage fault on stacking for exception entry
	  unsigned reserved1 : 2;
	  unsigned MMARVALID : 1; // =1: MMAR is known
      } MMFSR; // Memory Management Fault Status Register (8b)
      
      struct { // Bus Fault Status Register (8b)
	  unsigned IBUSERR     : 1; // =1: Instruction bus error
	  unsigned PRECISERR   : 1; // =1: Data bus error, the PC has the intruction causing the fault
	  unsigned IMPRECISERR : 1; // =1: Data bus error asynchronouns, PC advanced after the error
	  unsigned UNSTKERR    : 1; // =1: BusFault on unstacking for a return from exception
	  unsigned STKERR      : 1; // =1: BusFault on stacking for exception entry
	  unsigned reserved1   : 2;
	  unsigned BFARVALID   : 1; // =1: BFAR is known
      } BFSR;  // Bus Fault Status Register (8b)

      struct { // Usage Fault Status Register (16b)
	  unsigned reserved1  : 6;
	  unsigned DIVBYZERO  : 1; // =1: Division by zero
	  unsigned UNALIGNED  : 1; // =1: Unaligned memory access
	  unsigned reserved2  : 4;
	  unsigned NOCP       : 1; // =1: Attempted to use a coprocessor
	  unsigned INVPC      : 1; // =1: Invalid PC load (illegal EXC_RETURN)
	  unsigned INVSTATE   : 1; // =1: Illegal use of EPSR (Execution Program Status)
	  unsigned UNDEFINSTR : 1; // =1: Attempted to execute an undefined instruction
      } UFSR;  // Usage Fault Status Register (16b)

  } CFSR;  // Configurable Fault Status Register

  //CFSR = *((volatile typeof(CFSR)*) &(SCB->CFSR));
  CFSR.MMFSR = (*((volatile typeof(CFSR.MMFSR)*) (0xE000ED28))); 
  CFSR.BFSR  = (*((volatile typeof(CFSR.BFSR)*)  (0xE000ED29))); 
  CFSR.UFSR  = (*((volatile typeof(CFSR.UFSR)*)  (0xE000ED2A))); 

  // Take these from SCB since SCB_Type isn't in the debugger context
  u32_t BFAR  = SCB->BFAR;  // Bus Fault Address Register
  u32_t MMFAR = SCB->MMFAR; // MemManage Fault Address RegisteR
  u32_t AFSR  = SCB->AFSR;  // Auxiliary Fault Status Register

  struct { // HardFault Status Register
      unsigned reserved1 : 1;
      unsigned VECTTBL   : 1; // BusFault on a vector table read during exception processing
      unsigned reserved2 : 28;
      unsigned FORCED    : 1; // The HardFault was triggered because other fault handler was disabled or had lower priority
      unsigned DEBUGEVT  : 1; // Reserved for debug use (set to 0 if you wanna write on HFSR)
  } HFSR = *((volatile typeof(HFSR)*) &(SCB->HFSR)); // HardFault Status Register
  
  /*****
   * Some useful debugger commands:
   *
   *  l *PC      shows line executed when the interrupt was triggered
   *  l *LR      shows last function call
   *  p CFSR     shows struct with information about the fault 
   *  p (CFSR.BFSR.BFARVALID)?BFAR:0    location that generated a Bus Fault (0 if no BusFault)
   *  p (CFSR.MMSR.MMARVALID)?MMFAR:0  location that generated a MemManage Fault (0 if no MMFault)
   * 
   * The most common error is Imprecise Bus Fault (CFSR.BFSR.IMPRECISERR = 1) meaning that
   * there was an attempt to write in an invalid address, and since this access was asynchronous
   * the PC might be 1 or more lines ahead (so the actual faulty instruction might be a little
   * before). Also, since the PC is usually in some binary code without source, LR can be useful.
   ****/

  // unused vars
  ((void)CFSR);((void)HFSR);
  ((void)(R0+R1+R2+R3+R12+LR+PC+PSR+BFAR+MMFAR+AFSR));

#endif /* TARGET_ARCHITECTURE_STM32 */
  
  /* Stop the execution */
  Assert(0, ec_UNKNOWN);
}
