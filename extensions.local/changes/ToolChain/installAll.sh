#!/bin/bash

#{  Meta install script for STM32-toolchain.
#  written by Gregor Rebel 2011-04-11.
#  
#  This script will automatically run all install_* scripts in current folder.
# 
#  Feel free do distribute and change to your own needs!
#
# Tested successfully on:
# Ubuntu 10.04 Netbook Edition x86
# KUbuntu 11.04 x86
# openSuSE 11.4 x86
# XUbuntu 11.10 x64
#}

Script="$1"

source scripts/installFuncs.sh

if [ "$Script" == "" ]; then #{ run all install-files
  Files=`ls install_*[^~#]`

  #{ prepare for installation
  rm -f   2>/dev/null ErrorOccured OK.AllInstalls
  rm -Rf 2>/dev/null extensions/*
  echo "" >cleanup.sh
  if [ ! -d Logs ]; then
    mkdir Logs
  fi
  for Script in $Files; do
    if [ ! -e Logs/$Script.log ]; then
      echo "" >Logs/$Script.log
    fi
  done
  #}prepare for installation
  #{ createLinks
  cat <<END_OF_LINKS >scripts/createLinks.sh #{
#!/bin/bash
# createLinks.sh created `date` by $0
#
# This script is part of ToolChain_STM32

if [ ! -d extensions.active ]; then
  mkdir extensions.active
fi
cd extensions.active
if [ ! -e readme.txt ]; then
  echo <<"END_OF_README" >readme.txt

                               ToolChain_STM32

                         Extension to central makefile

                       written by Gregor Rebel 2010-`date +%Y`


All makefile.* files in this folder are automatically being included by ../makefile

The idea of this folder is to activate certain extensions by creating symbolic
links to files inside ../extensions

These links get created by several activate-scripts from ../activate_project.sh
You can find these activate scripts inside ../extensions/

END_OF_README
fi

MakefilesPresent=\`ls | grep makefile.\`
cd ..

rm -f 2>/dev/null extensions;  ln -sv \$Source/ToolChain_STM32/ToolChain/extensions  extensions

cd additionals

END_OF_LINKS
#}
  if [ -d ../Template/additionals/ ]; then
    rm -f 2>/dev/null ../Template/additionals/*
  else
    mkdir ../Template/additionals
  fi
  if [ -d extensions/ ]; then
    rm -f 2>/dev/null extensions/*
  else
    mkdir extensions
  fi
  #} createLinks
  #{ copy QtCreator.ini
  
  if [ ! -d ~/.config/ ]; then
    mkdir ~/.config
  fi
  if [ ! -d ~/.config/Nokia/ ]; then
    mkdir ~/.config/Nokia/
  fi
  if [ ! -e ~/.config/Nokia/QtCreator.ini ]; then
    rm -f ~/.config/Nokia/QtCreator.ini
    cp $HOME/ToolChain_STM32/Template/QtCreator/QtCreator.ini ~/.config/Nokia/QtCreator.ini
  fi
  
  #}QtCreator.ini
  #{ run all install scripts
  CurrentVersion=`cat ../Version` 

  LogFolder="`pwd`/Logs/*"

  # spawn xterm to show current log messages..
  Title="Installing ToolChain_STM32 v$CurrentVersion"
  XtermIsRunning=`ps -Af | grep "$Title" | grep -v grep`
  if [ "$XtermIsRunning" == "" ]; then
    xterm -T "$Title" -geometry 1920 -e "tail -f $LogFolder" &
  fi

  echo "Runing all install scripts."
  echo "You may check progress by inspecting log files via"
  echo "  tail -f $LogFolder"
  echo ""

  if [ -d extensions/ ]; then
    rm -f -R extensions/*
  fi
  echo -n "clearing log-files"
  for Script in $Files; do
    echo "" >Logs/$Script.log 
    echo "" >Logs/$Script.err
    echo -n "."
  done
  echo "OK"
  
  for Script in $Files; do
    if [ ! -e ErrorOccured ]; then
      Error=""
      $0 $Script || Error="1"
      if [ "$Error" != "" ]; then
        echo "ERROR occured during $Script !"
        exit 6
      fi
    fi
  done
  #} run all install scripts
  
  cp ../Version scripts/

  #{ remove temporary files
  chmod +x cleanup.sh
  ./cleanup.sh
  #}remove temporary files
  #{ installation finished successfully
  
  touch OK.AllInstalls
  addLine scripts/SourceMe.sh  '# ensure that $PATH contains no . (brings find into trouble)'
  AlreadyPresent=`grep "export PATH=" scripts/SourceMe.sh | grep split | grep join`
  if [ "$AlreadyPresent" == "" ]; then
  cat <<'END_OF_TEXT' >>scripts/SourceMe.sh
export PATH=`perl -e "my @A=split(':', '\$PATH');  print join(':', grep { \\\$_ ne '.' } @A );"`
END_OF_TEXT
  fi
  
  addLine scripts/SourceMe.sh "alias commit='git pull && git commit' #$0"
  addLine scripts/SourceMe.sh "export PATH=\"./extensions.local/:\$HOME/Source/ToolChain_STM32/ToolChain/extensions/:\$HOME/Source/ToolChain_STM32/ToolChain/bin/:\$PATH\" #$0"
  source scripts/SourceMe.sh
  OldDir="`pwd`"
  cd ../Template
  ./createLinks.sh
  cd "$OldDir"
  cd ..
  cd ..
  addLine $HOME/.bashrc "source `pwd`/ToolChain_STM32/ToolChain/scripts/SourceMe.sh"

  
  echo ""
  echo "All tools have been installed successfully."
  echo "Note: Your .bashrc has been updated."
  echo ""
  echo "Next steps:"
  echo "  (1) Connect JTAG programmer (e.g. ARM-USB-TINY-H) to your computer"
  echo "  (2) Connect your ARM prototype board (e.g. STM32-P107) to JTAG programmer"
  echo "  (3) Start a new shell!"
  echo "  (4) cd '`pwd`/ToolChain_STM32_$CurrentVersion'"
  echo "  (5) ./createNewProject.pl Test ItsMe"
  echo "  (6) cd Test"
  echo "  (7) ./compile.sh"
  echo ""
  echo "  Have fun! :-)"
  
  #} installation finished successfully
#} run all install-files
else                         #{ run single install-file
  echo -n "running `pwd`/$Script.."
  dir Logs
  echo "$Script started @`date`" >>Logs/$Script.log
  ./$Script >>Logs/$Script.log 2>>Logs/$Script.err && OK="1"
  if [ "$OK" == "1" ]; then
    echo "OK"
  else
    echo "ERROR"
    echo ""
    echo "Check logfiles:"
    find `pwd`/Logs/ -name $Script.* -exec echo "less {}" \;
    touch ErrorOccured
    exit 5
  fi
fi
#}run single install-file
