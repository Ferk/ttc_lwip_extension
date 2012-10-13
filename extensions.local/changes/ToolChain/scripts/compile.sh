#!/bin/bash

MAIN_C="$1"
NOLOG=""

if [ "$MAIN_C" == "NOLOG" ]; then
  NOLOG="1"
  MAIN_C="$2"
else
  if [ "$2" == "NOLOG" ]; then
    NOLOG="1"
  fi
fi

if [ "$MAIN_C" != "" ]; then
  if [ ! -e "mains/$MAIN_C" ]; then
    echo "$0 - ERROR: Cannot find main-file '$MAIN_C'!"
    echo "Available candidates: `ls -C mains/*.c`"
    exit 6
  fi
  rm 2>/dev/null main.c
  ln -s mains/$MAIN_C main.c
fi

if [ ! -e main.c ]; then
  echo "$0 - ERROR: No main.c selected!"
  echo "Available candidates: `ls -C mains/*.c`"
  exit 5
fi

cat <<END_OF_HEADER >extensions.active/makefile
#
#                               ToolChain_STM32
#
# Central makefile.
# This makefile was created by concatenating all makefile.* files in this folder.
# 
# This file has been automatically created by $0 on `date`.
# ALL CHANGES MADE TO THIS FILE WILL GET LOST!
#

END_OF_HEADER
find extensions.active/makefile\.* -exec cat {} >>extensions.active/makefile \;

ProjectFiles=`ls QtCreator/*.files`
for ProjectFile in $ProjectFiles; do
  echo "updating project file $ProjectFile .."
  ./_/updateProjectFiles.pl $ProjectFile ../ >${ProjectFile}.log || Error="1"
done
if [ "$Error" == "1" ]; then
  echo "Compilation canceled due to error"
  exit 10
fi

Log="compile.log"
OK=""
echo "compiling..."
ls -l main.c
make clean >/dev/null 2>&1
if [ "$NOLOG" == "1" ]; then
  make all -j1 && OK="1"
else
  make all -j1 >$Log 2>&1 && OK="1"
fi

if [ "$OK" == "1" ]; then
    if [ "$NOLOG" == "" ]; then
  
        grep warning $Log \
      | grep -v additionals/050_ \
      | grep -v additionals/300_ \
      | grep -v additionals/400_ \
      | grep -v ToolChain/ \
      >compile.warnings
      Warnings=`cat compile.warnings`
      if [ "$Warnings" != "" ]; then
	LESS="-RX +/warning:"
        cat <<END_OF_WARNINGS >>compile.warnings

Warnings occured. You should try to avoid these warnings. Sometimes a warning hints to a real problem.
Good code does compile without warnings.

Type Q to quit from less.
END_OF_WARNINGS
        ${PAGER:-less} compile.warnings
        
      fi
    fi

    echo "Compiled successfully:"
    ls -lh *.bin *.elf
    echo ""
else
    echo "ERROR occured during compilation!"
    if [ "$NOLOG" == "" ]; then
      LESS="-RX +/makefile:|make:|ld:|collect2:|error:|undefined reference|multiple definition|Assembler messages:";
      ${PAGER:-less} $Log
    fi
    exit 10
fi
echo ""

