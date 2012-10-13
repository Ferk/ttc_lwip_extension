#!/bin/bash

#
#  Install script for LwIP
#  URL: http://savannah.nongnu.org/projects/lwip/
#
#  Script written by Fernando Carmona Varo 2012.
# 
#  Feel free do distribute and change to your own needs!
#
# Tested successfully on:
#

source scripts/installFuncs.sh

if [ "$USER" == "root" ]; then
  echo "$0: ERROR this script should not been run as user root!"
  exit 10
fi

INSTALLPATH="400_network_lwip"
dir "$INSTALLPATH"
cd "$INSTALLPATH"

#{ IMPLEMENT YOUR INSTALL SCRIPT BELOW -----------------------------------------------

if [ ! -e OK.Docs ]; then #{
  Get_Fails=""
  dir ../../Documentation/Network
  getFile 'http://www.es.sdu.edu.cn/project/doc/' 'Design%20and%20Implementation%20of%20the%20lwIP%20tcpIP%20stack.pdf' "TCP-IP_Stack_LwIP_Design_and_Implementation.pdf" "../../Documentation/Network/"
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
  
  DirName="lwip-1.4.0"
  ZipFile=${DirName}\.zip
  getFile http://download.savannah.gnu.org/releases/lwip/ $ZipFile
  unZIP  $DirName $ZipFile
  #add2CleanUp ../cleanup.sh "rm -Rf $INSTALLPATH/$File"
  
  rm -f "$DirName/port"
  ln -s "$(pwd)/ttc_port" "$DirName/port"
  
  if [ ! -d ${DirName} ]; then
    echo "ERROR: Cannot download file (folder '${DirName}/' not found in `pwd`)!"
    exit 5
  else

      cd "${DirName}"
      # This file needs renaming to avoid conflicting with FreeRTOS timers.c
      mv src/core/timers.c src/core/lwip_timers.c
      
  #   File="Ethernet/app/uip-conf.h"
  #   if [ ! -e "${File}_orig" ]; then #{
  #       makeOrig ${File}
  #       echo "`pwd`> modifing ${File}.. (disabling double definitions)"
    
  #       replaceInFile ${File} 'typedef uint8_t u8_t;'     '#include "stm32_io.h" //ToolChain_STM32 already define in STM32_io.h  typedef uint8_t u8_t;'
  #       replaceInFile ${File} 'typedef uint16_t u16_t;'   '//ToolChain_STM32 already define in STM32_io.h  typedef uint16_t u16_t;'
  #   fi #}

      cd "$InstallDir"
    
    echo "" >OK.Install
  fi
fi #} [ ! -e OK.Install ]; then
if [   -e OK.Install ]; then #{
  Name="${INSTALLPATH}"
  createExtensionMakefileHead ${Name} #{
  cat <<END_OF_MAKEFILE >>../extensions/makefile.$Name

ifdef EXTENSION_$Name
  
  # activate certain parts of source-code
  COMPILE_OPTS += -DEXTENSION_$Name
  # load lwip
  COMPILE_OPTS +=  -lxilkernel -llwip4
  # activate semaphores for lwip
  COMPILE_OPTS += -DconfigUSE_COUNTING_SEMAPHORES
  COMPILE_OPTS += -DconfigUSE_MUTEXES

  INCLUDE_DIRS += -I additionals/${Name}/src/include/ \\
	      -I additionals/${Name}/src/include/ipv4/ \\
	      -I additionals/${Name}/src/include/ipv6/ \\
	      -I additionals/${Name}/src/include/ \\
	      -I additionals/${Name}/src/netif/ppp/ \\
              -I additionals/${Name}/port/include/ \\

  VPATH += additionals/${Name}/src/api/ \\
	 additionals/${Name}/src/core/ \\
	 additionals/${Name}/src/core/snmp/ \\
	 additionals/${Name}/src/core/ipv4/ \\
	 additionals/${Name}/src/core/ipv6/ \\
	 additionals/${Name}/src/netif/ \\
	 additionals/${Name}/src/unit/ \\
         additionals/${Name}/port/

  MAIN_OBJS += api_lib.o api_msg.o err.o netbuf.o netifapi.o sockets.o \\
tcpip.o dhcp.o init.o mem.o memp.o netif.o pbuf.o raw.o stats.o udp.o \\
autoip.o icmp.o igmp.o inet.o inet_chksum.o ip.o ip_addr.o ip_frag.o \\
asn1_dec.o asn1_enc.o mib2.o mib_structs.o msg_in.o msg_out.o etharp.o \\
tcp.o tcp_out.o tcp_in.o timers.o lwip_timers.o def.o sys_arch.o \\
syscalls.o ttc_eth_netif.o

endif
END_OF_MAKEFILE
  createExtensionMakefileTail ${Name} #}
  File="../extensions/activate.${Name}.sh"
  createActivateScriptHead $File $0 "TCP/IP-stack LwIP (multiple sockets; automatic resend; manages requests from an additional thread)" #{
  cat <<END_OF_ACTIVATE >>$File
# available variables (-> scripts/installFuncs.sh:createActivateScriptHead())
#
# Paths to certain directories in current project folder
#   \$Dir_Extensions        -> extensions/
#   \$Dir_ExtensionsLocal   -> extensions.local/
#   \$Dir_ExtensionsActive  -> extensions.active/
#   \$Dir_Additionals       -> additionals/

# activate dependencies
activate.500_ttc_ethernet.sh QUIET

# remove activated variants of same type
rm -f \$Dir_ExtensionsActive/makefile.400_network_*

# create links into extensions.active/
createLink \$Dir_Extensions/makefile.$Name \$Dir_ExtensionsActive/makefile.$Name '' QUIET

END_OF_ACTIVATE
  createActivateScriptTail $File $Name

  echo "createLink \$Dir_Extensions/makefile.$Name \$Dir_ExtensionsActive/makefile.$Name '' QUIET" >>$File
  
  createActivateScriptTail $File $Name #}

  addLine ../scripts/createLinks.sh "rm 2>/dev/null $INSTALLPATH;  ln -sv \$Source/ToolChain_STM32/ToolChain/$INSTALLPATH/lwip-1.4.0  $INSTALLPATH"
  echo "Installed successfully: $INSTALLPATH"
#}
else
  echo "failed to install $INSTALLPATH"
  exit 10
fi

cd ..

#} IMPLEMENT YOUR INSTALL SCRIPT ABOVE -----------------------------------------------

exit 0
