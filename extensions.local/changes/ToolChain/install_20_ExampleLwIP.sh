#!/bin/bash

#
#  Script written by Fernando Carmona Varo 2012-2013.
# 
#  Feel free do distribute and change to your own needs!
#

source scripts/installFuncs.sh

if [ "$USER" == "root" ]; then
  echo "$0: ERROR this script should not been run as user root!"
  exit 10
fi

INSTALLPATH="600_example_lwip"
dir $INSTALLPATH
cd $INSTALLPATH

#{ IMPLEMENT YOUR INSTALL SCRIPT BELOW -----------------------------------------------

  createExtensionMakefileHead ${INSTALLPATH} #{
  cat <<END_OF_MAKEFILE >>../extensions/makefile.$INSTALLPATH #{
# Adds Libraries/stm32_Registers.* to the project
# Provides global variables with detailed structures for various CortexM3 registers.
# See Libraries/stm32_Registers.* for details.

# activate certain parts of source-code
COMPILE_OPTS += -DEXTENSION_$INSTALLPATH

# debug lwip
COMPILE_OPTS +=  -DLWIP_DEBUG

ifndef uCONTROLLER
  STM32F10X_HD   = 1
  uCONTROLLER    = STM32F10X_HD 
endif

INCLUDE_DIRS += -I additionals/examples/
vpath %.c additionals/examples/

MAIN_OBJS += example_lwip.o

END_OF_MAKEFILE
#}
  createExtensionMakefileTail ${INSTALLPATH} #}
  File="../extensions/activate.${INSTALLPATH}.sh" #{
  createActivateScriptHead $File $0
  cat <<END_OF_ACTIVATE >>$File #{

activate.250_stm_std_peripherals__rcc.sh QUIET
activate.250_stm_std_peripherals__dma.sh QUIET
activate.250_stm_std_peripherals__gpio.sh QUIET
activate.300_scheduler_free_rtos_heap2.sh QUIET
activate.500_ttc_gpio.sh QUIET
activate.500_ttc_ethernet.sh QUIET
activate.400_network_lwip.sh QUIET

rm 2>/dev/null \$Dir_ExtensionsActive/makefile.600_example_*
createLink \$ScriptPath/makefile.$INSTALLPATH \$Dir_ExtensionsActive/makefile.$INSTALLPATH '' QUIET

END_OF_ACTIVATE
#}
  createActivateScriptTail $File $INSTALLPATH
  #}

  echo "Installed successfully: $INSTALLPATH"

cd ..
#} IMPLEMENT YOUR INSTALL SCRIPT ABOVE -----------------------------------------------

exit 0
