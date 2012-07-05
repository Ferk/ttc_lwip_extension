#!/bin/bash
#
#                  ToolChain_STM32
#
#           written 2010-2012 by Gregor Rebel
#
# This script activates an extension for ToolChain_STM32.
# This script has been automatically created by ./install_08_Board_Olimex_H107.sh.
# Any changes made to this script will be lost!
#
# Usage: Run this script from your project folder where your main.c is.
#

BoardName="XXX"  # <-- change according to name of makefile to activate 

if [ "$1" == "INFO" ]; then
  cat <<'END_OF_INFO'
Prototype board configured for local project in extensions.local/makefile.100_board_$BoardName
END_OF_INFO
  exit 0
fi


if [ "$1" == "QUIET" ]; then
  Quiet="1"
fi

if [ "$Quiet" == "" ]; then
  echo ""
fi

# ensure that $PATH contains no . (brings find into trouble)
#export PATH=`perl -e "my @A=split(':', '$PATH');  print join(':', grep { \\\$_ ne '.' } @A );"`
export PATH=`perl -e "my @A=split(':', '$PATH');  print join(':', grep { substr(\\\$_,0,1) eq '/' } @A );"`

function createLink() {      # Source,Link             creates symbolic link (removes old one before)
  Source="$1"
  Link="$2"
  User="$3"
  
  if [ -e $Link ]; then
    if [ "$User" != "" ]; then
      sudo -u$User rm $Link
    else
      rm $Link
    fi
  fi
  
  if [ "$User" != "" ]; then
    sudo -u$User ln -s $Source $Link
  else
    ln -s $Source $Link
  fi
}
function getScriptPath() {   #                         ScriptPath = absolute path of directory in which script is located
  MyPWD=`pwd`
  FirstChar=`perl -e "print substr('$0', 0, 1);"`
  if [ "$FirstChar" == "/" ]; then
    MySelf="$0"
  else
    MySelf="$MyPWD/$0"
  fi
  ScriptPath=`perl -e "print substr('$MySelf', 0, rindex('$MySelf', '/'));"`
  cd "$ScriptPath"
  ScriptPath=`pwd`
  cd "$MyPWD"
  #X echo "MySelf= $MySelf"
  #X echo "ScriptPath= $ScriptPath"
}
CurrentPath=`pwd`
getScriptPath
Dir_Extensions=$ScriptPath # this script is located inside extensions/
Dir_ExtensionsLocal=${Dir_Extensions}.local

if [ -d extensions.active ]; then #{
  Dir_ExtensionsActive="extensions.active"
  Dir_Additionals="additionals"
#}
else #{
  if [ -d ../extensions.active ]; then
    Dir_ExtensionsActive="../extensions.active"
    Dir_Additionals="../additionals"
  else
    Dir_ExtensionsActive=${ScriptPath}.active
    Dir_Additionals=""
  fi
fi #}



  rm 2>/dev/null $Dir_ExtensionsActive/makefile.100_board_*
  
  # create links into extensions.active/
  createLink $Dir_Extensions/makefile.100_board_$BoardName $Dir_ExtensionsActive/makefile.100_board_$BoardName '' QUIET
  
  activate.200_cpu_stm32f10x.sh QUIET


# back to project folder
cd "$CurrentPath"
if [ -e "$Dir_ExtensionsActive/makefile.100_board_$BoardName" ]; then
  echo "Extension activated: 100_board_olimex_h107"
  if [ "$Quiet" == "" ]; then
    echo ""
    echo "${CurrentPath}/extensions.active/"
    find "$Dir_ExtensionsActive/" -name "makefile.*" -execdir echo '  {}' \; | sort
  fi
else
  echo "ERROR: Failed to activate extension 100_board_olimex_h107 in $CurrentPath (missing '`pwd`/$Dir_ExtensionsActive/makefile.100_board_$BoardName')!"
fi
