/*
 *
 *        Name: xmsgtest.c (C program source)
 *              primary test harness for XMITMSGX library
 *      Author: Rick Troth, rogue programmer, La Casita en la Esquina
 *        Date: 2017-Nov-25 (Sat) Thanksgiving 2017
 *
 */

#include <stdio.h>
#include <string.h>

#include "xmitmsgx.h"
char *xmmprefix = PREFIX; /* installation prefix not application prefix */

int main()
  {
    int rc, msgc, msgn, msgo;
    unsigned char buffer[256], *msgv[16], *_p;
    struct MSGSTRUCT teststruct;
    int xmitmsgx_version = XMITMSGX_VERSION;

    msgc = 4;
    msgv[0] = "ZZZ"; msgv[1] = "AAA"; msgv[2] = "BBB"; msgv[3] = "CCC";
    msgv[4] = "DDD"; msgv[5] = "EEE"; msgv[6] = "FFF"; msgv[7] = "GGG";
    msgn = 2;
    msgo = 0;

    (void) printf("\n");

    /** report version ************************************************/
    (void) printf("xmsgtest: XMITMSGX version %d.%d.%d (from the macro)\n",
      (XMITMSGX_VERSION>>24),(XMITMSGX_VERSION>>16)&0xFF,(XMITMSGX_VERSION>>8)&0xFF);
    (void) printf("xmsgtest: size of MSGSTRUCT is %d bytes\n",sizeof(teststruct));
    (void) printf("xmsgtest: - size of integer is %d\n",sizeof(rc));
    (void) printf("xmsgtest: - size of pointer is %d\n",sizeof(_p));
    (void) printf("xmsgtest: XMITMSGX version %d.%d.%d (from static int)\n",
       (xmitmsgx_version>>24),
      ((xmitmsgx_version>>16)&0xFF),
      ((xmitmsgx_version>>8)&0xFF));

    /** test xmopen() ************************************************/
    (void) memset(&teststruct,0x00,sizeof(teststruct));
    teststruct.prefix = "msgtst";
/*  (void) printf("xmsgtest: ***** xmopen() *****\n");                */
    rc = xmopen("xmitmsgx",0,&teststruct);
    (void) printf("xmsgtest: xmopen() returned %d\n",rc);
    if (rc != 0) return rc;

    (void) printf(" max message number %d\n",teststruct.msgmax);
/*  sprintf(buffer,"%d",teststruct.msgmax); msgv[1] = buffer;         */
/*  rc = xmprint(111,2,msgv,msgo,&teststruct);                        */
/*  if (rc < 0) return rc;                                            */

/*  (void) printf(" raw message 2 '%s'\n",teststruct.msgtable[2]);  */
/*  (void) printf(" msglevel %d\n",teststruct.msglevel);  */

    (void) printf(" esape '%c'\n",*teststruct.escape);
/* 112    I esape character '&' (token introducer)                    */

    (void) printf(" major '%s' minor '%s'\n",teststruct.pfxmaj,teststruct.pfxmin);
/* 113    I code field major '&1' minor '&2'                          */

    (void) printf(" messages file '%s'\n",teststruct.msgfile);
/* 114    I message source file '&1'                                  */

    (void) printf(" locale '%s'\n",teststruct.locale);
/* 115    I detected locale '&1'                                      */

    (void) printf(" applid '%s'\n",teststruct.applid);
/* 116    I detected applid '&1'                                      */

/*  (void) printf(" caller '%s'\n",teststruct.caller);                */
/* 117    I detected caller '&1'                                      */

/*
    char *prefix;
    char *letter;
 */

    /** test xmmake() ************************************************/
/*  (void) printf("xmsgtest: ***** xmmake() *****\n");                */
    teststruct.msgnum = 599;               /* known to be missing ... */
    rc = xmmake(&teststruct);       /* ... so return code should be 2 */
    (void) printf("xmsgtest: xmmake() returned %d\n",rc);

    /** test xmstring() **********************************************/
/*  (void) printf("xmsgtest: ***** xmstring() *****\n");              */
    rc = xmstring(buffer,sizeof(buffer)-1,msgn,msgc,(char**)msgv,&teststruct);
    (void) printf("xmsgtest: xmstring() returned %d\n",rc);
    if (rc < 0) return rc;
    (void) printf(" '%s'\n",buffer);

    /** test xmprint() ***********************************************/
/*  (void) printf("xmsgtest: ***** xmprint() *****\n");               */
    rc = xmprint(msgn,msgc,(char**)msgv,msgo,&teststruct);
    (void) printf("xmsgtest: xmprint() returned %d\n",rc);
    if (rc < 0) return rc;

    /** W, E, S, T ****************************************************/
    (void) printf("xmsgtest: W/E/S/T\n");
    rc = xmprint(4,msgc,(char**)msgv,msgo,&teststruct);
    if (rc < 0) return rc;
    rc = xmprint(8,msgc,(char**)msgv,msgo,&teststruct);
    if (rc < 0) return rc;
    rc = xmprint(12,msgc,(char**)msgv,msgo,&teststruct);
    if (rc < 0) return rc;
    rc = xmprint(16,msgc,(char**)msgv,msgo,&teststruct);
    if (rc < 0) return rc;

    /** test xmwrite() ***********************************************/
/*  (void) printf("xmsgtest: ***** xmwrite() *****\n");               */
    rc = xmwrite(1,msgn,msgc,(char**)msgv,msgo,&teststruct);
    (void) printf("xmsgtest: xmwrite() returned %d\n",rc);
    if (rc < 0) return rc;

    /** test xmclose() ***********************************************/
/*  (void) printf("xmsgtest: ***** xmclose() *****\n");               */
    rc = xmclose(&teststruct);
    (void) printf("xmsgtest: xmclose() returned %d\n",rc);
    if (rc != 0) return rc;

/*  (void) printf("xmsgtest: ***** regression tests passed *****\n"); */
    (void) printf("\n");

    return 0;
  }


