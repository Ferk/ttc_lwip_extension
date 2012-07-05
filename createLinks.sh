#!/bin/bash
#
# Create links to external tools/ sources provided by ToolChain_STM32
# into current directory
#
# Usage:
# cd to yur source-code directory
# isssue createLinks.sh 
#
# Example:
# cd ~/Source/Test123
# ./createLinks.sh
#

Source="$HOME/Source"

echo "$0: regenerating links.."
rm 2>/dev/null _;                     ln -sf $Source/ToolChain_STM32/ToolChain/scripts  _
for Script in makefile clean.sh; do
  if [ ! -e $Script ]; then
    rm 2>/dev/null $Script
    ln -s _/$Script .
  fi
done

rm 2>/dev/null additionals/common;    ln -sf $Source/ToolChain_STM32/CommonLibraries/   additionals/common
rm 2>/dev/null additionals/examples;  ln -sf $Source/ToolChain_STM32/Examples/          additionals/examples

Version=`cat _/Version`

# create links to third party modules
source ./_/createLinks.sh >/dev/null

echo "ToolChain_STM32 v$Version"
