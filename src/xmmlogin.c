/*
 *
 *        Name: xmmlogin.c (C program source)
 *              drive SYSLOG with a message from the repository
 *              saying simply "we have signed on"
 *        Date: 2023-06-05 returning from Mackinac
 *
 */

#include <unistd.h>
#include <string.h>

#include "xmitmsgx.h"
char *xmmprefix = PREFIX; /* installation prefix not application prefix */

/**********************************************************************/
int main(int argc,char*argv[])
  {
unsigned
    char *msgv[4];
    int msgc, rc;
    struct MSGSTRUCT mymsgstruct;

    msgc = 0;
   /* this element is reserved and not actually used at this time     */
    msgv[0] = "xmmlogin";
    /* "element number one" is where we stash the username            */
    msgv[1] = getlogin();
    if (msgv[1] != NULL) msgc = 2;

    /* now let's try and use the message handler                      */
    rc = xmopen("xmitmsgx",MSGFLAG_SYSLOG,&mymsgstruct);
    if (rc != 0) return 1;

    /* there is presently no API function to do this - fix that!      */
    strcpy(mymsgstruct.pfxmaj,"XMM");    /* three-character up-cased applid/major */
    strcpy(mymsgstruct.pfxmin,"LOG");    /* three-character up-cased caller/minor */

    /* so far so good so now write the message to SYSLOG              */
    rc = xmprint(202,msgc,msgv,MSGFLAG_SYSLOG|MSGFLAG_NOPRINT,&mymsgstruct);

    /* clean-up in all cases whether xmprint() failed or not          */
    xmclose(&mymsgstruct);

    if (rc < 0) return 1;

    return 0;
  }


