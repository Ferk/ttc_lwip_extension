#!/bin/sh
#
# Restore actions performed by changes.apply.sh
#

cd "$(dirname "$0")"/..
[ -e required_version ] || { echo "error: Not a project folder: $(pwd)"; exit 1;}

Source="$HOME/Source/"
Version=$(cat "$Source/ToolChain_STM32/Version")
[ -z $Version ] && { echo "Toolchain not found."; exit;}

Sufix="~${Version}#"

echo "Undoing changes to the Toolchain by restoring backups for versión $Version..."

for old in $(find "$Source/ToolChain_STM32/" -iname "*$Sufix")
do
    echo $old
    
    new="${old%%$Sufix}"
    if [ -s "$old" ]; then
	mv -vf "$old" "$new"
    else
	# if old size is zero, it was a new file, delete it
	rm -vf "$old" "$new"
	rmdir -p "$(dirname "$new")" 2>&-
    fi
done

