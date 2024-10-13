#!/bin/sh
#
#         Name: xmmdemoc.sh (shell script)
#               demonstrate the message handler called from C
#         Date: 2023-07-23 (Sun)
#

#
# make some detection about this environment
cd `dirname "$0"`
D=`pwd`                         # the directory where these files reside
E=`sh -c ' cd .. ; exec pwd '`  # presumed package root (one level up)
F=/tmp/$$ ; mkdir $F            # a temporary directory

#
# work from the temporary directory
cd $F

#
# establish some environmentals for building/compiling
CC=cc
O=.o
CFLAGS="-I$E/include"
LDFLAGS="-lxmitmsgx -L$E/lib"

#
# compile the demo program
cp -p $D/xmmdemoc.c . 2> /dev/null
$CC $CFLAGS -o xmmdemoc$O -c xmmdemoc.c
RC=$? ; if [ $RC -ne 0 ] ; then cd $D ; rm -r $F ; exit $RC ; fi

#
# link the demo program object deck with the XMITMSGX library
#$CC $LDFLAGS -o xmmdemoc xmmdemoc$O
$CC -o xmmdemoc xmmdemoc$O $LDFLAGS
RC=$? ; if [ $RC -ne 0 ] ; then cd $D ; rm -r $F ; exit $RC ; fi

#
# run the compiled program
#cp -p $D/xmitmsgx.msgs . 2> /dev/null
cp -p $E/share/locale/*/xmitmsgx.msgs . 2> /dev/null
./xmmdemoc
RC=$? ; if [ $RC -ne 0 ] ; then cd $D ; rm -r $F ; exit $RC ; fi

#
# clean-up
cd $D ; rm -r $F

exit


