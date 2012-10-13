#!/bin/bash
#
# Start OpenOCD GDB server + connect to it
#
# It's also done in a way so that it's possible to replace the gdb command
# with this script in IDEs and keep the "-i=mi" metadata
#
# written by Fernando Carmona Varo 2012

# Go to the Project folder (this script can be run from anywhere as long as the
# Project main folder is one level below in the path the script was called with).
cd "$(dirname "$0")"/.. >/dev/null
[ -e required_version ] || { echo "error: Not a project folder: $(pwd)"; exit 1;}

echo() {
    # don't do any printing in dumb terminals (useful for the -i=mi switch used by UIs)
    [ $TERM == "dumb" ] && return
    builtin echo -e "\e[33m$0: $@\e[0m"
}

pgrep "openocd" 2>&- >&- || {

    echo "Running server..."
    _/gdb_server.sh 2>&- > gdb_server.log || {
	echo "error found"
	cat gdb_server.log && rm gdb_server.log
	exit
    } #&

    # while ! pgrep "openocd" >/dev/null
    # do sleep 0.5; done
}


echo "Starting gdb"
_/gdb.sh main.elf $@

