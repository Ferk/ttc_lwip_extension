#!/bin/bash
#
#
#  Install script for generic Ethernet support 
#
#  Initial Script written by Fernando Carmona 2012.
# 
#  Published under GNU General Public License
#  Check LEGAL.txt for details.
#

source scripts/installFuncs.sh

if [ "$USER" == "root" ]; then
  echo "$0: ERROR this script should not been run as user root!"
  exit 10
fi

INSTALLPATH="500_ttc_ethernet"
dir "$INSTALLPATH"
cd "$INSTALLPATH"

#{ IMPLEMENT YOUR INSTALL SCRIPT BELOW -----------------------------------------------

  Name="$INSTALLPATH"
  createExtensionMakefileHead ${Name}      #{ create makefile
  cat <<END_OF_MAKEFILE >>../extensions/makefile.$Name 

# activate certain parts of source-code
COMPILE_OPTS += -DEXTENSION_${Name}=1

MAIN_OBJS += ttc_ethernet.o

ifdef TARGET_ARCHITECTURE_STM32
  MAIN_OBJS += stm32_ethernet.o
endif

END_OF_MAKEFILE
  createExtensionMakefileTail ${Name} #}
  File="../extensions/activate.${Name}.sh" #{ create activate script
  createActivateScriptHead $File "$0" "Architecture independent support for Ethernet"
  cat <<END_OF_ACTIVATE >>$File
# available variables (-> scripts/installFuncs.sh:createActivateScriptHead())
#
# Paths to certain directories in current project folder
#   \$Dir_Extensions        -> extensions/
#   \$Dir_ExtensionsLocal   -> extensions.local/
#   \$Dir_ExtensionsActive  -> extensions.active/
#   \$Dir_Additionals       -> additionals/

# activate dependencies
activate.250_stm32_eth.sh QUIET

# remove activated variants of same type
# rm 2>/dev/null \$Dir_Additionals/${Name}_*

ArchitectureSupportAvailable=""

# check if stm32fxxx-support is available
if [ -e extensions/activate.250_stm_std_peripherals__gpio.sh ]; then
  activate.250_stm_std_peripherals__eth.sh QUIET \"\$0\"
  ArchitectureSupportAvailable="1"
fi

if [ "\$ArchitectureSupportAvailable" == "1" ]; then
  activate.500_ttc_gpio.sh QUIET \"\$0\"
  # create links into extensions.active/
  createLink "\$Dir_Extensions/makefile.$Name" "\$Dir_ExtensionsActive/makefile.$Name" '' QUIET
else
  echo "\$0 - ERROR: No support for current architecture available!"
fi

END_OF_ACTIVATE
  createActivateScriptTail $File $Name
  #}

  echo "Installed successfully: $Name"

cd ..
#} IMPLEMENT YOUR INSTALL SCRIPT ABOVE -----------------------------------------------

exit 0
