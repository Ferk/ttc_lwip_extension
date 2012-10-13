#!/bin/bash
#
# (c) Gregor Rebel 2010-2011

FILE="$1"
shift

pkill arm-none-eabi-gdb

if [ "$FILE" == "" ]; then
  echo "$0 ELF_BINARY"
  echo "Possible binaries: `ls *.elf`"
  exit 10
fi

if [ ! -e "$FILE" ]; then
  echo "$0 - ERROR: Cannot read binary-file '$FILE' in `pwd`!"
  exit 11
fi

Script="startscript.gdb"
cat >>$Script <<END_OF_SCRIPT

# CHANGE LINE BELOW ACCORDING TO YOUR SETUP!
target remote 127.0.0.1:3333  # connect to openocd runnning on local machine
#target remote eee:3333 # connect to openocd running on another machine (here eee)

# prevents single steping via next/ step: 
#monitor gdb_breakpoint_override hard

monitor reset init
monitor soft_reset_halt
symbol-file $FILE

set debug arm
break Assert if (!Condition)

END_OF_SCRIPT

arm-none-eabi-gdb -x $Script $@
rm $Script
