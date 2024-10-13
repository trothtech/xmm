#!/bin/sh
#
#         Name: xmitmivp.sh (shell script)
#         Date: 2023-03-24 (Fri)
#       Author: Rick Troth, rogue programmer
#
#               This script should be run *after* XMITMSGX is installed.
#               It exercises the package header and static library,
#               found in the standard Unix/Linux/POSIX locations.
#

cd `dirname "$0"`
if [ ! -f xmitmivp.c -a -f ../src/xmitmivp.c ] ; then cd ../src ; fi

#LANG=en_US.utf8
#LANG=en_US
#LANG=""
#export LANG

cc -o xmitmivp.o -c xmitmivp.c
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

cc -o xmitmivp xmitmivp.o -lxmitmsgx
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi
rm xmitmivp.o

./xmitmivp
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

rm xmitmivp

exit


