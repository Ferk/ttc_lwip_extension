#!/bin/sh
#
#  This script will apply the changes to the ToolChain version being used by the
#  proyect by symlinking them (so that the changes folder stays in sync),
#  keeping also a backup of the files.
#
#

cd "$(dirname "$0")"/..
[ -e required_version ] || { echo "error: Not a project folder: $(pwd)"; exit 1;}


ToolChain="$HOME/Source/ToolChain_STM32"
changes="$(pwd)/extensions.local/changes/"

Version=$(cat "$ToolChain/Version")
[ -z $Version ] && { echo "Toolchain not found."; exit;}

Sufix="~${Version}#"

echo "$0: checking local ToolChain changes"
[ -d "$changes" ] && for i in $(find "$changes" -type f)
do
    [ -z "${i##*[~#]}" ] && continue # skip if it ends in # or ~
    
    dest="$ToolChain/${i##$changes}"
    backup="${dest}${Sufix}"
    diff="extensions.local/changes.v${Version}.diff"
    [ -e "$backup" ] && continue # already replaced
    mkdir -p "${dest%/*}" 
    if [ -f "$dest" ]
    then mv "$dest" "$backup"
    else touch "$backup"
    fi
    ln -sv "$i" "$dest"
    [ -z "$updated" ] && echo "*** New Changes on $(date)" >> "$diff"
    diff -u "$backup" "$dest" >> "$diff"
    updated="yes"
done
[ $updated ] && {
    echo "$0: changes applied, rerun of install and activation scripts might be required"
    rm -f extensions.active/*
    rm -f "$ToolChain/ToolChain/OK.AllInstalls"
}
exit 0

