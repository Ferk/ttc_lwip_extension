/* memory.ld will define memory spaces */
INCLUDE configs/memory.ld

/*
OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
ENTRY(_start)
SEARCH_DIR(.)

GROUP(libgcc.a libc.a libcs3.a libcs3unhosted.a)

EXTERN(_start)
*/
/*
EXTERN (NMI_Handler)
EXTERN (HardFault_Handler)
EXTERN (MemManage_Handler)
EXTERN (BusFault_Handler)
EXTERN (UsageFault_Handler)
EXTERN (SVC_Handler)
EXTERN (DebugMon_Handler)
EXTERN (PendSV_Hander)
EXTERN (SysTick_Handler)
EXTERN (WWDG_IRQHandler)
EXTERN (PVD_IRQHandler)
EXTERN (TAMPER_IRQHandler)
EXTERN (RTC_IRQHandler)
EXTERN (FLASH_IRQHandler)
EXTERN (RCC_IRQHandler)
EXTERN (EXTI0_IRQHandler)
EXTERN (EXTI1_IRQHandler)
EXTERN (EXTI2_IRQHandler)
EXTERN (EXTI3_IRQHandler)
EXTERN (EXTI4_IRQHandler)
EXTERN (DMA1_Channel1_IRQHandler)
EXTERN (DMA1_Channel2_IRQHandler)
EXTERN (DMA1_Channel3_IRQHandler)
EXTERN (DMA1_Channel4_IRQHandler)
EXTERN (DMA1_Channel5_IRQHandler)
EXTERN (DMA1_Channel6_IRQHandler)
EXTERN (DMA1_Channel7_IRQHandler)
EXTERN (ADC1_2_IRQHandler)
EXTERN (USB_HP_CAN1_TX_IRQHandler)
EXTERN (USB_LP_CAN1_RX0_IRQHandler)
EXTERN (CAN1_RX1_IRQHandler)
EXTERN (CAN1_SCE_IRQHandler)
EXTERN (EXTI9_5_IRQHandler)
EXTERN (TIM1_BRK_IRQHandler)
EXTERN (TIM1_UP_IRQHandler)
EXTERN (TIM1_TRG_COM_IRQHandler)
EXTERN (TIM1_CC_IRQHandler)
EXTERN (TIM2_IRQHandler)
EXTERN (TIM3_IRQHandler)
EXTERN (TIM4_IRQHandler)
EXTERN (I2C1_EV_IRQHandler)
EXTERN (I2C1_ER_IRQHandler)
EXTERN (I2C2_EV_IRQHandler)
EXTERN (I2C2_ER_IRQHandler)
EXTERN (SPI1_IRQHandler)
EXTERN (SPI2_IRQHandler)
EXTERN (USART1_IRQHandler)
EXTERN (USART2_IRQHandler)
EXTERN (USART3_IRQHandler)
EXTERN (EXTI15_10_IRQHandler)
EXTERN (RTCAlarm_IRQHandler)
EXTERN (USBWakeUp_IRQHandler)
*/

_estack = 0x20005000;

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >rom
    
    .text : 
    {
        . = ALIGN(4);
        *(.text .text.*)        /* Program code */
        *(.rodata)      /* Read only data */
        *(.glue_7)
        *(.glue_7t)
        . = ALIGN(4);
        
        _etext = .;
        _sidata = _etext;
    } >rom

    .data : AT(_sidata) 
    {
        . = ALIGN(8);
        _sdata = .;
        *(.data)        /* Data memory */
        *(.data.*)
        . = ALIGN(8);
        _edata = .;
    } >ram

    .bss : 
    {
        . = ALIGN(8);
        _sbss = .;
        *(.bss)         /* Zero-filled run time allocate data memory */
        *(.bss.*)
        . = ALIGN(8);
        _ebss = .;
    } >ram
}
