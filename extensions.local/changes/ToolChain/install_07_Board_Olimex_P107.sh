#!/bin/bash

#
#  Install script for Fixpoint Mathematical Library 
#  URL: http://code.google.com/p/fixpointlib/
#
#  Script written by Gregor Rebel 2010-2011.
# 
#  Feel free do distribute and change to your own needs!
#
# Tested successfully on:
# Ubuntu 10.04 Netbook Edition x86
# KUbuntu 11.04 x86
# openSuSE 11.4 x86
# XUbuntu 11.10 x64

source scripts/installFuncs.sh
                          
if [ "$USER" == "root" ]; then
  echo "$0: ERROR this script should not been run as user root!"
  exit 10
fi

INSTALLPATH="100_board_olimex_p107"
dir "$INSTALLPATH"
cd "$INSTALLPATH"

#{ IMPLEMENT YOUR INSTALL SCRIPT BELOW -----------------------------------------------
  Dir=`pwd`
  if [ ! -e OK.Docs ]; then #{
  
    Get_Fails=""
    dir ../../Documentation/Boards
    cd ../../Documentation/Boards
    getFile http://olimex.com/dev/pdf/ARM/ST/ STM32-P107.pdf Board_STM32-P107.pdf
    getFile http://pdf1.alldatasheet.com/datasheet-pdf/view/162452/STMICROELECTRONICS/STE101P/+0W3W7WVPSLpETSpPKIbaZZTOfqgjUppKk+/ datasheet.pdf Board_STM32-P107_EthernetPhy_STE101P.pdf
  
    cd "$Dir"
    if [ "$Get_Fails" == "" ]; then
      touch OK.Docs
    else
      echo "$0 - ERROR: missing files: $Get_Fails"
    fi
  fi #}

  Name="${INSTALLPATH}" #{
  createExtensionMakefileHead ${Name} #{
  File="../extensions/makefile.${Name}" 
  cat <<END_OF_MAKEFILE >>$File 

# activate certain parts of source-code
COMPILE_OPTS += -DEXTENSION_${Name}=1
COMPILE_OPTS += -DBOARD=$Name
BOARD=$Name

# define microcontroller used on board
STM32F10X_CL  =1
COMPILE_OPTS += -DUC_DEVICE=STM32F107VCT6
COMPILE_OPTS += -DUC_PACKAGE_PINS=100

COMPILE_OPTS += -DRAM_SIZE_K=64
COMPILE_OPTS += -DRAM_START=0x20000000
COMPILE_OPTS += -DFLASH_SIZE_K=256
COMPILE_OPTS += -DFLASH_START=0x08000000

# Define port pins
# reference RevA: http://olimex.com/dev/pdf/ARM/ST/STM32-P107-Rev.A-schematic.pdf
# reference RevB: http://olimex.com/dev/ARM/ST/STM32-P107/STM32-P107-REV-B-SCH.pdf

COMPILE_OPTS += -DTTC_LED1=PIN_PC6
COMPILE_OPTS += -DTTC_LED2=PIN_PC7

COMPILE_OPTS += -DTTC_SWITCH1=PIN_PC13                # press button switch 1 (named Tamper)
COMPILE_OPTS += -DTTC_SWITCH1_TYPE=tgm_input_floating # -> ttc_gpio_types.h
COMPILE_OPTS += -DTTC_SWITCH1_LOWACTIVE=1
COMPILE_OPTS += -DTTC_SWITCH2=PIN_PA0                 # press button switch 2 (named WKUP)
COMPILE_OPTS += -DTTC_SWITCH2_TYPE=tgm_input_floating # -> ttc_gpio_types.h
COMPILE_OPTS += -DTTC_SWITCH2_LOWACTIVE=0

BoardRevA=1
#BoardRevB=1

ifdef BoardRevA  #{ USART pin configuration for Board rev.A
  COMPILE_OPTS += -DTTC_USART1=USART3       # USART connected to RS232 connector 9-pin female
  COMPILE_OPTS += -DTTC_USART1_TX=PIN_PD8   # RS232 connector 9-pin female TX-pin
  COMPILE_OPTS += -DTTC_USART1_RX=PIN_PD9   # RS232 connector 9-pin female RX-pin
  COMPILE_OPTS += -DTTC_USART1_RTS=PIN_PD12 # RS232 connector 9-pin female RTS-pin
  COMPILE_OPTS += -DTTC_USART1_CTS=PIN_PD11 # RS232 connector 9-pin female CTS-pin
  
  COMPILE_OPTS += -DTTC_USART2=USART2       # USART connected to 10-pin male header connector
  COMPILE_OPTS += -DTTC_USART2_TX=PIN_PD5   # RS232 on 10-pin male header connector TX-Pin
  COMPILE_OPTS += -DTTC_USART2_RX=PIN_PD6   # RS232 on 10-pin male header connector RX-Pin
endif #}
ifdef BoardRevB  #{ USART pin configuration for Board rev.B
  COMPILE_OPTS += -DTTC_USART1=USART2       # USART connected to RS232 connector 9-pin female
  COMPILE_OPTS += -DTTC_USART1_TX=PIN_PD8   # RS232 connector 9-pin female TX-pin
  COMPILE_OPTS += -DTTC_USART1_RX=PIN_PD9   # RS232 connector 9-pin female RX-pin
  COMPILE_OPTS += -DTTC_USART1_RTS=PIN_PD12 # RS232 connector 9-pin female RTS-pin
  COMPILE_OPTS += -DTTC_USART1_CTS=PIN_PD11 # RS232 connector 9-pin female CTS-pin
  
  COMPILE_OPTS += -DTTC_USART2=USART3       # USART connected to 10-pin male header connector
  COMPILE_OPTS += -DTTC_USART2_TX=PIN_PD4   # RS232 on 10-pin male header connector TX-Pin
  COMPILE_OPTS += -DTTC_USART2_RX=PIN_PD5   # RS232 on 10-pin male header connector RX-Pin
endif #}

#{ SPI1
COMPILE_OPTS += -DTTC_SPI1=SPI3          # SPI connected to RS232 10-pin male header UEXT and SDCARD slot
COMPILE_OPTS += -DTTC_SPI1_MOSI=PIN_PC12 # UEXT pin 8
COMPILE_OPTS += -DTTC_SPI1_MISO=PIN_PC11 # UEXT pin 7
COMPILE_OPTS += -DTTC_SPI1_SCK=PIN_PC10  # UEXT pin 9
COMPILE_OPTS += -DTTC_SPI1_NSS=PIN_PB1   # UEXT pin 10 (SoftwareNSS)
#}SPI1
#{ I2C1 - define inter-integrated circuit interface #1
  COMPILE_OPTS += -DTTC_I2C1=I2C1           # internal device being used for this port
  COMPILE_OPTS += -DTTC_I2C1_SDA=PIN_PB9    # UEXT-6
  COMPILE_OPTS += -DTTC_I2C1_SCL=PIN_PB8    # UEXT-5
  COMPILE_OPTS += -DTTC_I2C1_SMBAL=PIN_PB5  # 
#}I2C1

#{ ETH1 - define ethernet interface
  COMPILE_OPTS += -DTTC_ETH1=ETH1 
  # AF Output Push Pull
  COMPILE_OPTS += -DTTC_ETH1_RMII_MDIO=PIN_PA2
  COMPILE_OPTS += -DTTC_ETH1_RMII_MDC=PIN_PC1
  COMPILE_OPTS += -DTTC_ETH1_RMII_TX_EN=PIN_PB11
  COMPILE_OPTS += -DTTC_ETH1_RMII_TXD0=PIN_PB12
  COMPILE_OPTS += -DTTC_ETH1_RMII_TXD1=PIN_PB13
  COMPILE_OPTS += -DTTC_ETH1_RMII_PPS_OUT=PIN_PB5
  COMPILE_OPTS += -DTTC_ETH1_MII_MDIO=PIN_PA2
  COMPILE_OPTS += -DTTC_ETH1_MII_MDC=PIN_PC1
  COMPILE_OPTS += -DTTC_ETH1_MII_TXD2=PIN_PC2
  COMPILE_OPTS += -DTTC_ETH1_MII_TX_EN=PIN_PB11
  COMPILE_OPTS += -DTTC_ETH1_MII_TXD0=PIN_PB12
  COMPILE_OPTS += -DTTC_ETH1_MII_TXD1=PIN_PB13
  COMPILE_OPTS += -DTTC_ETH1_MII_PPS_OUT=PIN_PB5
  COMPILE_OPTS += -DTTC_ETH1_MII_TXD3=PIN_PB8
  # Input (Reset Value):
  COMPILE_OPTS += -DTTC_ETH1_RMII_REF_CLK=PIN_PA1
  COMPILE_OPTS += -DTTC_ETH1_RMII_CRS_DV=PIN_PD8
  COMPILE_OPTS += -DTTC_ETH1_RMII_RXD0=PIN_PD9
  COMPILE_OPTS += -DTTC_ETH1_RMII_RXD1=PIN_PD10
  COMPILE_OPTS += -DTTC_ETH1_MII_CRS_WKUP=PIN_PA0
  COMPILE_OPTS += -DTTC_ETH1_MII_RX_CLK=PIN_PA1
  COMPILE_OPTS += -DTTC_ETH1_MII_COL=PIN_PA3
  COMPILE_OPTS += -DTTC_ETH1_MII_RX_DV=PIN_PD8
  COMPILE_OPTS += -DTTC_ETH1_MII_TX_CLK=PIN_PC3
  COMPILE_OPTS += -DTTC_ETH1_MII_RXD0=PIN_PD9
  COMPILE_OPTS += -DTTC_ETH1_MII_RXD1=PIN_PD10
  COMPILE_OPTS += -DTTC_ETH1_MII_RXD2=PIN_PD11
  COMPILE_OPTS += -DTTC_ETH1_MII_RXD3=PIN_PD12
  COMPILE_OPTS += -DTTC_ETH1_MII_RX_ER=PIN_PB10 
#}ETH1

END_OF_MAKEFILE
  createExtensionMakefileTail ${Name} #}
  File="../extensions/activate.${Name}.sh" #{
  createActivateScriptHead $File $0 "Protoboard P107 from Olimex -> http://olimex.com/dev/stm32-p107.html"
  cat <<END_OF_ACTIVATE >>$File #{

  rm 2>/dev/null \$Dir_ExtensionsActive/makefile.100_board_*
  
  # create links into extensions.active/
  createLink \$Dir_Extensions/makefile.$Name \$Dir_ExtensionsActive/makefile.$Name '' QUIET
  
  activate.200_cpu_stm32f10x.sh QUIET "\$0"

END_OF_ACTIVATE
#}
  createActivateScriptTail $File $Name
  #}

#}
  echo "Installed successfully: $INSTALLPATH"

cd ..
rmdir 2>/dev/null $INSTALLPATH
#} IMPLEMENT YOUR INSTALL SCRIPT ABOVE -----------------------------------------------

exit 0
