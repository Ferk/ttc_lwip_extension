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
rm -f _;                     ln -s $Source/ToolChain_STM32/ToolChain/scripts  _
for Script in makefile clean.sh; do
  if [ ! -e $Script ]; then
    rm -f $Script
    ln -s _/$Script .
  fi
done
rm -f additionals/common;    ln -s $Source/ToolChain_STM32/CommonLibraries/   additionals/common

###
# These are just for this particular project 
# since it's an example that modifies the Toolchain template, I want to keep the Template files updated when I modify it
rm -f additionals/examples;     ln -s $Source/ToolChain_STM32/Template/additionals/examples     additionals/

for file in "$(pwd)"/extensions.local/changes/Template/configs/*; do
    ln -sf "$file" configs/
done
###

Version=`cat _/Version`

# create links to third party modules
source ./_/createLinks.sh >/dev/null

echo "ToolChain_STM32 v$Version"
