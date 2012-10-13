/*{ example_lwip::main.c ********************************************
 *                        ToolChain_STM32
 * 
 * Project example_lwip
 * Author: Fernando Carmona Varo
 *
 * This code is part of a project for the ToolChain_STM32 framework for 
 * microcontrollers with ARM CortexM3 CPU core.
 * ToolChain_STM32 is a free of charge set of open source scripts that download 
 * and install all tools being required to compile, flash and debug software for 
 * the before mentioned uCs. No additional closed source software is required.
 *
 * ToolChain_STM32 has been written 2010-2012 by Gregor Rebel
 *
 * 
 * Support for external tools
 *   + QtCreator
 *     The subfolder QtCreator/ contains an automatically updated creator-project.
 *     This project can be loaded into QtCreator. Using this IDE seems to be more comfortable
 *     for many developers.
 * 
 *   + Eclipse
 *     Support for Eclipse should come soon.
 *
 *
 * Basic framework template for
 *   - Olimex STM32-P107/ STM32-H107/ STM32-LCD/ .. eval boards
 *   - StdPeripheralsLibrary with CMSIS
 *   - GNU toolchain from CodeSourcery G++
 *   - FreeRTOS
 *   - FixPointLib 
 *   - TCP/IP-Stacks
 *   - Color LCD Panel support
 *   - local/ remote debugging (JTAG-Adapter may be connected to different computer)
 *
 * Coding-style
 *  + gnu99
 *    It's year 2012, get a decent compiler if your's does not comply with this.
 *    arm-none-eabi-gcc-4.4.1 does support gnu99.
 *     
 *  + Text Folding
 *    This source makes use of text folding to improve hierarchical readability 
 *    It has been written with jEdit + plugin Configurable Fold Handler.
 *    Activate folding in your favourite text-editor for { and } 
 *    (fold-start and -end) for best visual experience.
 *
 * Prerequisites:
 *   ToolChain_STM32 (http://hlb-labor.de/cortexm3)
 *   Latest version should be downloaded and installed automatically by issuing ./compile.sh
 *
 * Compile source + flash binary onto uC:  
 *   modify _/interface.cfg to suit to your JTAG programmer 
 *   ./compile.sh
 *
 * Debug:
 * - start openocd as gdb-server (may even run on another computer connected over network)
 *   ./_/gdb_server.sh
 * - start gdb text user-interface in another shell
 *   ./_/gdb.sh
 * 
 * Configuration of source-code
 *  - FreeRTOSConfig.h
 *    configure behaviour and features of FeeRTOS
 *
 *  - stm32f10x_conf.h
 *    modify according to your hardware (if differing from above) 
 *  
 *  - stm32f10x_it.c/ stm32f10x_it.h
 *    implement your own interrupthandlers in existing functions
 *    
 *  - main.c (this file)
 *    Ideally, the main.c file contains only some function calls of names like
 *    init_XXX() or start_XXX(). The intention of this file is to show an overview
 *    of your project and how to use your code. 
 *    Place real algorithms and implementations into extra .c and .h files and include 
 *    them from here. It should be easy to copy your code from this main.c into foregin sources.
 *    This makes reuse of your code much easier.
 *    
 *  - makefile
 *    check activate_project.sh for local makefile extensions
 * 
 *  - template.c_/ template.h_
 *    can be used as source-code templates for your own code
 * 
}*/
//{ Defines/ TypDefs *****************************************************

//} Defines
//{ Includes *************************************************************

#include "extensions.h"  // will include activated extensions

#include "example_lwip.h"

//} Includes
//{ Global Variables *****************************************************

//} Global Variables
//{ Function prototypes **************************************************

int main(void);

// task which runs main eventloop
void taskMain(void *pvParameters);

//} Function prototypes
//{ Function definitions *************************************************

int main(void) {

    // Enable BusFault, UsageFault and MemManage interrupts
    SCB->SHCSR |= (SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);
    
    startExtensions(taskMain, 512, NULL);  // will call taskMain() as first task
}
void taskMain(void *pvParameters) {
    
    pvParameters = pvParameters; // avoids warning: unused parameter 'pvParameters'

    // initialize your task
    example_lwip_start();
    
    while (1)  mSleep(1000);
    
}

//} Function definitions
