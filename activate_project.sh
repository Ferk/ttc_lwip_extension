#!/bin/bash

#            ToolChain_STM32 v
#
# This script configures which extensions should be activated for this project. 
#
# Each extension provides one additional makefile being linked into extensions.active/
# The extension makefile can define
# - additional constants
# - additional include directories to search for .h-files
# - additional source directories to search for .c-files
# - additional object files to create (object files get compiled from .c/.s-files)
# - other magic makefile features
#
# Minimum setup
# - 000 basic setup
# - 050 exactly one compiler
# - 100 exactly one board (be sure to set the correct!)
# - 300 exactly one scheduler (+ some scheduler settings)
# 

function age() {
   local filename=$1
   local changed=`stat -c %Y "$filename"`
   local now=`date +%s`
   local elapsed

   let elapsed=now-changed
   echo $elapsed
}

MakeFile="extensions.active/makefile"
if [ -e $MakeFile ]; then
  AgeMakefile=$(age $MakeFile)
  AgeActivateProject=$(age $0)

  if [ $AgeActivateProject -lt $AgeMakefile ]; then
    echo "$0 - project settings changed: cleaning extensions"
    ./clean.sh
  fi
fi


ExtensionsActive=`ls 2>/dev/null ${MakeFile}.*`
if [ "$ExtensionsActive" == ""  ]; then
  echo "$0 - no extensions active: activating configured extensions"

  # golden rule: start to activate lower ranks first:
  # activate.000_XXX.sh
  # activate.100_XXX.sh
  # ...

if [ 1 == 1 ]; then #{ 000 - activate basic setup
  Rank='000'

  # Basic toolchain settings required by all architectures and boards.
  #activate.000_basic_extensions_basic_setup.sh QUIET "$0"

  # Disables argument checking in ttc_XXX() functions. Use with caution!
  #activate.000_basic_extensions_enable_no_argument_checks.sh QUIET "$0"

  # compile option: do not optimize at all (best visibility during debugging)
  #activate.000_basic_extensions_optimize_1_debug.sh QUIET "$0"

  # compiler option: create fast code (can increase code- and memory-usage)
  #activate.000_basic_extensions_optimize_1_fastest.sh QUIET "$0"

  # compiler option: create small code (can degrade execution speed)
  #activate.000_basic_extensions_optimize_1_smallest.sh QUIET "$0"

  # compiler option: try to inline as many functions as possible (can increase code- and memory-usage)
  #activate.000_basic_extensions_optimize_2_inline.sh QUIET "$0"
  activate.000_basic_extensions_basic_setup.sh QUIET "$0"
  activate.000_basic_extensions_optimize_1_debug.sh QUIET "$0"

fi #}000
if [ 1 == 1 ]; then #{ 050 - compiler settings
  Rank='050'

  # Compiler SourceyG++ (Closed Source, Binary only)
  #activate.050_compiler_sourcery_gpp.sh QUIET "$0"
  activate.050_compiler_sourcery_gpp.sh QUIET "$0"

fi #}050
if [ 1 == 1 ]; then #{ 100 - activate your board
  Rank='100'
  #  
  # Each board 
  # - activates a CPU
  # - sets memory sizes and locations
  # - activates required standard peripheral library (STM-types only)
  #
  # Note: Exactly one board must be activated for your project!

  # activate following lines to use your own board file in extensions.local/
  # ln -s makefile.100_board_* ../extensions.local/


  # Protoboard Mini-STM32 with 2,8" Color Touch LCD and STM32F103RBT6
  #activate.100_board_mini_stm32_v30.sh QUIET "$0"

  # Protoboard H103 from Olimex -> http://olimex.com/dev/stm32-h103.html
  #activate.100_board_olimex_h103.sh QUIET "$0"

  # Protoboard H107 from Olimex -> http://olimex.com/dev/stm32-h107.html
  #activate.100_board_olimex_h107.sh QUIET "$0"

  # Protoboard LCD from Olimex -> http://olimex.com/dev/stm32-lcd.html
  #activate.100_board_olimex_lcd.sh QUIET "$0"

  # Protoboard P103 from Olimex -> http://olimex.com/dev/stm32-p103.html
  #activate.100_board_olimex_p103.sh QUIET "$0"

  # Protoboard P107 from Olimex -> http://olimex.com/dev/stm32-p107.html
  activate.100_board_olimex_p107.sh QUIET "$0"

  # Protoboard LCD from Olimex -> http://olimex.com/dev/stm32-lcd.html
  #activate.100_board_wsn3_stm32w108.sh QUIET "$0"


fi #}100
if [ 1 == 1 ]; then #{ 200 - CPU
  Rank='200'
  #
  # The CPU should already been activated by the board makefile.
  # You may want to activate a CPU in case you don't yet have a board makefile 

  # CPU STM32F10x
  #activate.200_cpu_stm32f10x.sh QUIET "$0"

  # DEFAULT_INFO: ../extensions/activate.200_cpu_stm32f1xx_std_peripherals.sh created by ./install_15_STM-StdPeripheralsLibrary.sh
  #activate.200_cpu_stm32f1xx_std_peripherals.sh QUIET "$0"

  # CPU STM32F2xx
  #activate.200_cpu_stm32f2xx.sh QUIET "$0"

  # Standard Peripheral Library for CPU STM32W1xx
  #activate.200_cpu_stm32l1xx.sh QUIET "$0"

fi #}200
if [ 1 == 1 ]; then #{ 250 - Std Peripherals Library
  Rank='250'
  # board has activated CPU + Std Peripherals Library
  # activate certain .c-files of Std Peripherals Library

  # Architecture dependant standard peripheral library for STM32F10x_StdPeriph_Driver microcontrollers
  #activate.250_CPAL_STM32F10x_StdPeriph_Driver.sh QUIET "$0"

  # Architecture dependant standard peripheral library for STM32F2xx_StdPeriph_Driver microcontrollers
  #activate.250_CPAL_STM32F2xx_StdPeriph_Driver.sh QUIET "$0"

  # Architecture dependant standard peripheral library for STM32L1xx_StdPeriph_Driver microcontrollers
  #activate.250_CPAL_STM32L1xx_StdPeriph_Driver.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_adc.c
  #activate.250_stm_std_peripherals__adc.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_bkp.c
  #activate.250_stm_std_peripherals__bkp.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_can.c
  #activate.250_stm_std_peripherals__can.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_cec.c
  #activate.250_stm_std_peripherals__cec.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_crc.c
  #activate.250_stm_std_peripherals__crc.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_dac.c
  #activate.250_stm_std_peripherals__dac.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_dbgmcu.c
  #activate.250_stm_std_peripherals__dbgmcu.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_dma.c
  activate.250_stm_std_peripherals__dma.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_exti.c
  #activate.250_stm_std_peripherals__exti.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_flash.c
  #activate.250_stm_std_peripherals__flash.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_fsmc.c
  #activate.250_stm_std_peripherals__fsmc.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_gpio.c
  activate.250_stm_std_peripherals__gpio.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_i2c.c
  #activate.250_stm_std_peripherals__i2c.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_iwdg.c
  #activate.250_stm_std_peripherals__iwdg.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_pwr.c
  #activate.250_stm_std_peripherals__pwr.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_rcc.c
  activate.250_stm_std_peripherals__rcc.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_rtc.c
  #activate.250_stm_std_peripherals__rtc.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_sdio.c
  #activate.250_stm_std_peripherals__sdio.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_spi.c
  #activate.250_stm_std_peripherals__spi.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_tim.c
  #activate.250_stm_std_peripherals__tim.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_usart.c
  #activate.250_stm_std_peripherals__usart.sh QUIET "$0"

  # adds c-source to list of compiled objects: stm32f10x_wwdg.c
  #activate.250_stm_std_peripherals__wwdg.sh QUIET "$0"

fi #}250
if [ 1 == 1 ]; then #{ 251 - Support Libraries for STM32Wxxx architecture
  Rank='251'
  # STM32Wxxx architecture (STM32W108) has been developed by Ember Corp. as EM351/EM357. 
  # These chips are not compatible with Standard Peripheral Library.
  # The HAL library can be used instead. 
  # The, binary only, SimpleMAC library uses its own HAL.
  #

fi #}251
if [ 1 == 1 ]; then #{ 270 - Communication Application Peripheral Library
  Rank='270'
  # CPAL has been designed on top of std peripherals library
  # activate certain .c-files of CPAL
  #

  # Driver required to use arechitecture independant features of ST's Common Peripheral Access Library (CPAL)
  #activate.270_CPAL_STM32_CPAL_Driver.sh QUIET "$0"

  # adds c-source to list of compiled objects: cpal_hal.c
  #activate.270_CPAL_STM32_CPAL_Driver__cpal_hal.sh QUIET "$0"

  # adds c-source to list of compiled objects: cpal_i2c.c
  #activate.270_CPAL_STM32_CPAL_Driver__cpal_i2c.sh QUIET "$0"

  # adds c-source to list of compiled objects: cpal_usercallback_template.c
  #activate.270_CPAL_STM32_CPAL_Driver__cpal_usercallback_template.sh QUIET "$0"

fi #}270
if [ 1 == 1 ]; then #{ 300 - multitasking scheduler
  Rank='300'
  #
  # A multitasking scheduler provides basic multithreading features.
  # The activation of one scheduler and a heap type is mandatory. 
  #

  # Multitasking scheduler FreeRTOS (http://freertos.org)
  #activate.300_scheduler_free_rtos.sh QUIET "$0"

  # memory allocation scheme 1: can only allocate; smallest memory + processing overhead
  #activate.300_scheduler_free_rtos_heap1.sh QUIET "$0"

  # memory allocation scheme 2: allocate/ deallocate (deterministic, high memory usage)
  #activate.300_scheduler_free_rtos_heap2.sh QUIET "$0"

  # memory allocation scheme 3: allocate/ deallocate (full implementation uses malloc()/ free())
  #activate.300_scheduler_free_rtos_heap3.sh QUIET "$0"

  # DEFAULT_INFO: ../extensions/activate.300_scheduler_free_rtos_network_lwip.sh created by ./install_06_FreeRTOS.sh
  #activate.300_scheduler_free_rtos_network_lwip.sh QUIET "$0"
  activate.300_scheduler_free_rtos.sh QUIET "$0"
  activate.300_scheduler_free_rtos_heap2.sh QUIET "$0"

fi #}300
if [ 1 == 1 ]; then #{ 400 - External Support Libraries
  Rank='400'

  # Fixpoint Library (faster calculations than software floating point)
  #activate.400_fix_point_lib.sh QUIET "$0"

  # low-level driver for LCD-panel provided on protoboard stm32_lcd from Olimex
  #activate.400_lcd_320x240_olimex.sh QUIET "$0"

  # TCP/IP-stack uIP (only one socket; manual resend; no multitasking required)
  #activate.400_network_uip_ste101p.sh QUIET "$0"

  # Architecture independent support for Inter-integrated circuit interface (I2C)
  #activate.400_sensor_lis3lv02dl.sh QUIET "$0"

fi #}400
if [ 1 == 1 ]; then #{ 500 - Support Libraries provided by ToolChain_STM32
  Rank='500'
  #
  # These libraries provide example code for how to initialize
  # and use certain funtional units like watchdogs, adcs, ...
  #

  # architecture independent support of Analog Digital Converters (ToDo: INCOMPLETE)
  #activate.500_ttc_adc.sh QUIET "$0"

  # Provides some global struct-pointers to CPU-registers. Makes register-debugging easier.
  activate.500_ttc_debug_registers.sh QUIET "$0"

  # Architecture independent support for Ethernet
  #activate.500_ttc_eth.sh QUIET "$0"

  # Architecture independent support for Universal Synchronous Asynchronous Serial Receiver Transmitter (GPIO)
  activate.500_ttc_gpio.sh QUIET "$0"

  # Architecture independent support for Inter-integrated circuit interface (I2C)
  #activate.500_ttc_i2c.sh QUIET "$0"

  # Architecture independent support for Serial Peripheral Interface (SPI)
  #activate.500_ttc_random.sh QUIET "$0"

  # Architecture independent support for Serial Peripheral Interface (SPI)
  #activate.500_ttc_spi.sh QUIET "$0"

  # Architecture independent support for Universal Synchronous Asynchronous Serial Receiver Transmitter (USART)
  #activate.500_ttc_usart.sh QUIET "$0"

  # architecture independent support of hardware watchdogs
  #activate.500_ttc_watchdogs.sh QUIET "$0"

fi #}500
if [ 1 == 1 ]; then #{ 600 - Examples
  Rank='600'
  #
  # Only one example can be active at a time.
  # In order to run, each example requires
  # - board (must define PB_LED1, PB_LED2 and others)
  # - cpu
  # - multitasking scheduler
  # - standard peripheral library
  #
  # 600_example_leds is the most simple example and should work on any board that defines PB_LED1, PB_LED2
  #

  # Example of how to use architecture independent Serial Peripheral Interface (I2C) support
  #activate.600_example_i2c_master.sh QUIET "$0"

  # Example of how to use architecture independent Serial Peripheral Interface (I2C) support
  #activate.600_example_i2c_slave.sh QUIET "$0"

  # Example of how to draw moving boxes with multiple tasks
  #activate.600_example_lcd_boxes.sh QUIET "$0"

  # Example of how to print on LCD-screen
  #activate.600_example_lcd_printf.sh QUIET "$0"

  # Example code for a multitasking Heartbeat LED
  #activate.600_example_leds.sh QUIET "$0"

  # DEFAULT_INFO: ../extensions/activate.600_example_lwip.sh created by ./install_20_ExampleLwIP.sh
  #activate.600_example_lwip.sh QUIET "$0"

  # Example of how to use architecture independent Serial Peripheral Interface (SPI) support
  #activate.600_example_spi_master.sh QUIET "$0"

  # Example of how to use architecture independent Serial Peripheral Interface (SPI) support
  #activate.600_example_spi_slave.sh QUIET "$0"

  # Examples of using Multitasking: Thread-synchronization/ -communication via Queues for boards with LCD-screen
  #activate.600_example_threading_queues.sh QUIET "$0"

  # Examples of using Multitasking: Thread-synchronization/ -communication via Semaphores for boards with LCD-screen
  #activate.600_example_threading_semaphores.sh QUIET "$0"

  # Example of how to use architecture independent USART support
  #activate.600_example_usart.sh QUIET "$0"
  #activate.600_example_leds.sh QUIET "$0"

fi #}600

  # this makefile belongs to current project and is added automatically
  cd extensions.active/
  
  # activate local makefile
  ln -s ../extensions.local/makefile.700_extra_settings .
  
  cd ..
else
  echo "$0 - extension setup found: not changing extensions.active/"
fi
