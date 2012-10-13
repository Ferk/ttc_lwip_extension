#!/bin/bash

#{ SYNOPSIS: compile.sh [<MAIN.C>] [NOFLASH|NOCOMPILE|NOLOG]
# <MAIN.C>    c-file to use as  main.c (create symbolic link)
# NOFLASH     do not flash onto uC
# NOCOMPILE   do not compile or flash
# NOLOG       do not spool compiler output into logfile (directly print to STDOUT)

MAIN_C="$1"
NOFLASH=""
NOCOMPILE=""
NOLOG=""

if [ "$MAIN_C" == "NOFLASH" ]; then
  NOFLASH="1"
  MAIN_C="$2"
else
  if [ "$2" == "NOFLASH" ]; then
    NOFLASH="1"
  fi
fi

if [ "$MAIN_C" == "NOCOMPILE" ]; then
  NOCOMPILE="1"
  MAIN_C="$2"
else
  if [ "$2" == "NOCOMPILE" ]; then
    NOCOMPILE="1"
  fi
fi

if [ "$MAIN_C" == "NOLOG" ]; then
  NOLOG="NOLOG"
  MAIN_C="$2"
else
  if [ "$2" == "NOLOG" ]; then
    NOLOG="NOLOG"
  fi
fi
#}SYNOPSIS

SourceDir=`pwd`          # Where is our source code?
if [ ! -e required_version ]; then #{
  echo "$0 - ERROR: Missing Version-file! Compiling in incorrect folder '$SourceDir'?";
  echo "Create a copy of template project via createNewProject.pl and try again!"
  exit 10
fi #}
RequiredVersion=`cat required_version`

#{ obtain ToolChain_STM32 (if not already done)
cd
if [ ! -d Source ]; then
  mkdir Source
fi
cd Source
if [ -d ToolChain_STM32 ]; then
  mv ToolChain_STM32 ToolChain_STM32_DELETE
fi
rm -f ToolChain_STM32
if [ -e ToolChain_STM32 ]; then
  echo "$0 - ERROR: Cannot move away `pwd`/ToolChain_STM32 !"
  exit 11
fi

Folder="ToolChain_STM32_${RequiredVersion}"
if [ ! -d $Folder ]; then #{ download missing toolchain version
  echo "downloading $Folder.."
  wget -c http://hlb-labor.de/projekte/cortexm3/archive/${Folder}.tar.bz
  tar xjf ${Folder}.tar.bz
  if [ -d ToolChain_STM32 ]; then
    mv ToolChain_STM32 ${Folder}
  fi
fi
#}
ln -s ${Folder} ToolChain_STM32

# update ToolChain files with project local changes
cd "$SourceDir"
[ -e extensions.local/changes.apply.sh ] && { 
    eval extensions.local/changes.apply.sh || exit $?
}

cd "$HOME/Source/ToolChain_STM32/ToolChain/"
if [ ! -e OK.AllInstalls ]; then
  echo "Installation incomplete: Starting toolchain installation.."
  ./installAll.sh
  if [ -e OK.AllInstalls ]; then
    echo "Toolchain was installed successfully."
    cd "$SourceDir"  
    $0
    exit $?
  else
    echo "ERROR: Toolchain installation failed!"
    exit 10
  fi
fi
#}


# Don't use a pager when running from an IDE (not a proper terminal)
# else, add additional options for less
[ $TERM = dumb ] && {
    PAGER=cat
} || {
    LESS="-RX +/makefile:|make:|ld:|collect2:|error:|undefined reference|multiple definition|Warnings";
    ## Activate bell
    #PAGER="{ echo -ne '\a';less; }"
}

cd "$SourceDir"          # cd back to this source code directory
./createLinks.sh         # recreate symbolic links to ToolChain_STM32
source ./_/SourceMe.sh   # update path variables to binary tools
./activate_project.sh    # activate all extensions for current project
./createLinks.sh         # recreate symbolic links to ToolChain_STM32 (setup may have changed during activate_project.sh)
if [ "$NOCOMPILE" == "" ]; then
  ./_/compile.sh $MAIN_C $NOLOG  # compile source
fi

#{ flash onto uC
if [ -e main.bin ]; then
  echo "Compiled successfully."
  if [ "$NOFLASH" == "" ]; then
    echo "Trying to flash binary onto microcontroller.."
    ./_/flash.sh main.bin
    echo -ne '\a'
  fi
fi
#}

#{ Update metadata for IDEs
### create TAGS file (for Emacs, Vi(m), jEdit, Nedit, etc)
if hash emacs 2>&-; then
    CTAGS="ctags -e"
elif hash ctags 2>&-; then
    CTAGS="ctags"
fi
[ "$CTAGS" ] && { 
    echo "$CTAGS: generating tags file..."
    $CTAGS -R --exclude="Demo" --exclude=".??*" --languages="Make,C,C++" .
    # cd QtCreator
    # $CTAGS -e -L "../*.files"
    # cd ..
} 2>&- >> compile.log & # asyncronous, append to log

#}

[ -e main.bin ] # return OK if exists
