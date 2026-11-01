#!/bin/sh
#

find /usr/include/. -type f -iname '*.h' -print \
    | grep -i errno \
    | xargs grep '#define' \
    | awk -F: '{print $2}' \
    | grep '/\*' \
    | awk '{print $2 , "0000" $3 , $0}' \
    | grep '^E' | grep -v '|' \
    | sed 's#/\*#|#g' | sed 's#\*/#|#g' \
    | awk '{print substr($2,length($2)-3,4) "    E " $1 "|" $0}' \
    | awk -F\| '{print $1 "," $3}'
#S=`echo "0000$S" | awk '{print substr($0,length($0)-3,4)}'`

#   1    E EPERM, operation not permitted




exit








#
# extract the metadata and load it into this shell
grep -v '^#' $D/$S.cf | awk '{print "UFT_" $0}' | grep -v '=$' > /tmp/$$.set
RC=$? ; if [ $RC -ne 0 ] ; then rm -f /tmp/$$.set ; exit $RC ; fi
. /tmp/$$.set
rm /tmp/$$.set

#
# file needs a name
T="$2"
if [ -z "$T" ] ; then T="$UFT_NAME" ; fi
if [ -z "$T" ] ; then
    echo "$Z: UFT file $S needs a name"
    exit 1 ; fi

#
# report and receive
echo "$Z: receiving UFT file $S as '$T'"
cat $D/$S.df > $T
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

#
# if that worked then discard the spool file
yes | rm $D/$S.*

exit







exit

UFT_REMOTE=
UFT_SIZE='36K'
UFT_FROM='ltroth3'
UFT_USER='rmt'
UFT_SIZE='36K'
UFT_FROM='ltroth3'
UFT_TYPE='I '
UFT_NAME='uftcreqs.tar'
UFT_DATE='.2024-04-19. 20:22:31 '
UFT_RECFM='F'
UFT_LRECL='512'


