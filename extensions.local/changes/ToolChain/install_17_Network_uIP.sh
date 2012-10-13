#!/bin/bash

#
#  Install script for Ethernet module STE101P on Olimex prototype board STM32-P107.
#  URL: http://olimex.com/dev/stm32-p107.html
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

INSTALLPATH="400_network_uip_ste101p"
dir "$INSTALLPATH"
cd "$INSTALLPATH"

#{ IMPLEMENT YOUR INSTALL SCRIPT BELOW -----------------------------------------------

if [ ! -e OK.Docs ]; then #{
  Get_Fails=""
  dir ../../Documentation/Network
  getFile 'http://www.sics.se/~adam/download/?f=' uip-1.0-refman.pdf "TCP-IP_Stack_uIP_Manual.pdf" "../../Documentation/Network/"
  if [ "$Get_Fails" == "" ]; then
    touch OK.Docs
  else
    echo "$0 - ERROR: missing files: $Get_Fails"
  fi
fi #}
if [ ! -e OK.Install ]; then #{
  echo "installing in $INSTALLPATH ..."
  InstallDir=`pwd`
  
  InstallInHomeDir="1"
  if [ "$InstallInHomeDir" == "1" ]; then
    Prefix=`pwd`  # install into subfolder of ToolChain/ for current user only
  else
    findPrefix    # install for all users
  fi
  
  DirName="STM32-P107_ethernet_demo_5.50"
  ZipFile=${DirName}\.zip
  getFile http://olimex.com/dev/soft/arm/ST/STM-P107/ $ZipFile
  unZIP  $DirName $ZipFile
  #add2CleanUp ../cleanup.sh "rm -Rf $INSTALLPATH/$File"
  
  if [ ! -d ${DirName} ]; then
    echo "ERROR: Cannot download file (folder '${DirName}/' not found in `pwd`)!"
    exit 5
  fi
  #cd "${DirName}/"
  if [ -d ${DirName} ]; then
    cd "${DirName}"
    File="Ethernet/app/uip-conf.h"
    if [ ! -e "${File}_orig" ]; then #{
        makeOrig ${File}
        echo "`pwd`> modifing ${File}.. (disabling double definitions)"
    
        replaceInFile ${File} 'typedef uint8_t u8_t;'     '#include "stm32_io.h" //ToolChain_STM32 already define in STM32_io.h  typedef uint8_t u8_t;'
        replaceInFile ${File} 'typedef uint16_t u16_t;'   '//ToolChain_STM32 already define in STM32_io.h  typedef uint16_t u16_t;'
        #replaceInFile ${File} ''                         '//ToolChain_STM32 already define in STM32_io.h  '
        
    fi #}
    
    File="Ethernet/app/ethernet.h"
    if [ ! -e "${File}_orig" ]; then #{
      makeOrig ${File}
      echo "`pwd`> modifing ${File}.. (disabling double definitions)"
  
      replaceInFile ${File} '#include "httpd.h"'     '//ToolChain_STM32 httpd not required for minimal TCP/IP - #include "httpd.h"'
      
    fi #}
    cd "$InstallDir"
    
    echo "" >OK.Install
  fi
fi #} [ ! -e OK.Install ]; then
if [   -e OK.Install ]; then #{
  Name="${INSTALLPATH}" #{
  createExtensionMakefileHead ${Name} #{
  cat <<END_OF_MAKEFILE >>../extensions/makefile.$Name

#EXTENSION_HTTP_SERVER=1   # activate to include this feature!

ifdef EXTENSION_$Name
#   ifndef STM32F10X_CL
#     ERROR: STM32F10X_CL_must_be_set!
#   endif
 
# activate certain parts of source-code
COMPILE_OPTS += -DEXTENSION_${Name}=1

INCLUDE_DIRS += -I additionals/$Name/Libraries/STM32_ETH_Driver/inc/ \\
                -I additionals/$Name/Ethernet/uip/ \\
                -I additionals/$Name/Ethernet/app/ \\
                -I additionals/$Name/Ethernet/uip/httpd/

VPATH += additionals/$Name/Libraries/STM32_ETH_Driver/src/ \\
         additionals/$Name/Ethernet/app \\
         additionals/$Name/Ethernet/uip


MAIN_OBJS += clock-arch.o ethernet.o timer.o uip.o uip_arp.o uiplib.o misc.o sbrk.o

ifdef EXTENSION_HTTP_SERVER

# activate certain parts of source-code
COMPILE_OPTS += -DEXTENSION_HTTP_SERVER

VPATH += $Name/Ethernet/uip/httpd/
MAIN_OBJS += httpd-fs.o http-strings.o httpd.o httpd-cgi.o psock.o

endif

endif
END_OF_MAKEFILE
  createExtensionMakefileTail ${Name} #}
  File="../extensions/activate.${Name}.sh"
  createActivateScriptHead $File $0 "TCP/IP-stack uIP (only one socket; manual resend; no multitasking required)" #{

  cat <<END_OF_ACTIVATE >>$FILE

# activate dependencies
activate.250_stm32_eth.sh QUIET

# remove activated variants of same type
rm -f \$Dir_ExtensionsActive/makefile.400_network_*

# create links into extensions.active/
createLink \$Dir_Extensions/makefile.$Name \$Dir_ExtensionsActive/makefile.$Name '' QUIET

END_OF_ACTIVATE
  
  createActivateScriptTail $File $Name #}

  addLine ../scripts/createLinks.sh "rm 2>/dev/null $INSTALLPATH;  ln -sv \$Source/ToolChain_STM32/ToolChain/$INSTALLPATH/STM32-P107_ethernet_demo_5.50/  $INSTALLPATH"
  #}

  Name="250_stm32_eth" #{
  createExtensionMakefileHead ${Name} #{
  cat <<END_OF_MAKEFILE >>../extensions/makefile.$Name

COMPILE_OPTS += -DEXTENSION_${Name}=1

INCLUDE_DIRS += -I additionals/$Name/inc/

vpath %.c additionals/$Name/src/

MAIN_OBJS += stm32_eth.o

END_OF_MAKEFILE
  createExtensionMakefileTail ${Name} #}
  File="../extensions/activate.${Name}.sh"
  createActivateScriptHead $File $0 "STM32 Ethernet Firmware" #{
  cat <<END_OF_ACTIVATE >>$File
# available variables (-> scripts/installFuncs.sh:createActivateScriptHead())
#
# Paths to certain directories in current project folder
#   \$Dir_Extensions        -> extensions/
#   \$Dir_ExtensionsLocal   -> extensions.local/
#   \$Dir_ExtensionsActive  -> extensions.active/
#   \$Dir_Additionals       -> additionals/

# create links into extensions.active/
createLink \$Dir_Extensions/makefile.$Name \$Dir_ExtensionsActive/makefile.$Name '' QUIET

END_OF_ACTIVATE
  createActivateScriptTail $File $Name #}

  addLine ../scripts/createLinks.sh "rm 2>/dev/null $Name;  ln -sv \$Source/ToolChain_STM32/ToolChain/$INSTALLPATH/STM32-P107_ethernet_demo_5.50/Libraries/STM32_ETH_Driver/  $Name"
  #}
  
  echo "Installed successfully: $INSTALLPATH"
#}
else
  echo "failed to install $INSTALLPATH"
  exit 10
fi

cd ..




#} IMPLEMENT YOUR INSTALL SCRIPT ABOVE -----------------------------------------------

exit 0
