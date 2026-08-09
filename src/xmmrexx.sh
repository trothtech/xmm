#!/bin/sh
#
#         Name: xmmrexx.sh (shell script)
#               demonstrate the message handler called from Java
#         Date: 2023-07-21 (Fri)
#

#
# make some detection about this environment
cd `dirname "$0"`
D=`pwd`                         # the directory where these files reside
E=`sh -c ' cd .. ; exec pwd '`  # presumed package root (one level up)

#
# set or expand the loadable library search
if [ -z "$LD_LIBRARY_PATH" ] ; then LD_LIBRARY_PATH="$E/lib"
                               else LD_LIBRARY_PATH="$E/lib:$LD_LIBRARY_PATH" ; fi
if [ -z "$DYLD_LIBRARY_PATH" ] ; then DYLD_LIBRARY_PATH="$E/lib"
                                 else DYLD_LIBRARY_PATH="$E/lib:$DYLD_LIBRARY_PATH" ; fi
if [ -z "$SHLIB_PATH" ] ; then SHLIB_PATH="$E/lib"
                          else SHLIB_PATH="$E/lib:SHLIB_PATH" ; fi
# the rationale here is that we set all known library search variables
# rather than attempt OS detection - more than one way to skin that cat

#
# conditionally add Regina Rexx loadable libraries to the search
for LD in /usr/opt/regina/lib /usr/opt/regina/lib64 \
          /usr/opt/oorexx/lib /usr/opt/oorexx/lib64 ; do
    if [ -d $LD ] ; then
        LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$LD"
        DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH:$LD"
        SHLIB_PATH="$SHLIB_PATH:$LD"
    fi
done
export LD_LIBRARY_PATH DYLD_LIBRARY_PATH SHLIB_PATH

#
# conditionally augment the command search path
if [ -d /usr/opt/regina/bin ] ; then PATH="$PATH:/usr/opt/regina/bin" ; fi
if [ -d /usr/opt/oorexx/bin ] ; then PATH="$PATH:/usr/opt/oorexx/bin" ; fi
export PATH

#
# run the sample program
RX=`which regina 2> /dev/null`
if [ ! -x "$RX" ] ; then RX=rexx ; fi
$RX xmmrexx.rx
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

exit


