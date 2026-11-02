/*
 *
 *        Name: xmiterr.c (C program source)
 *              Present error message based on collected errno constants.
 *      Author: Rick Troth, rogue programmer
 *        Date: 2017-Dec-10 (Sun)
 *
 * This program is equivalent to:
 *              xmitmsg --applid errno {msgno [token [token [...]]]}
 *
 */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>

#define MSGMAX 16

#include "xmitmsgx.h"
char *xmmprefix = PREFIX; /* installation prefix not application prefix */

int main(int argc, char*argv[])
  {
    int rc, msgn, msgc, i, j, n;
    char *msgv[MSGMAX], *arg0;
    struct MSGSTRUCT mymsgstruct;

    arg0 = argv[0];

    /* process command-line options                                   */
    for (n = 1; n < argc && argv[n][0] == '-' &&
                            argv[n][1] != 0x00; n++)
      { switch (argv[n][1])
          { case '?':                   /* help                       */
                break;

            case 'v': case 'V':                          /* --version */
                  { int vv, rr, mm;
                    vv =  XMITMSGX_VERSION >> 24;
                    rr = (XMITMSGX_VERSION >> 16) & 0xFF;
                    mm = (XMITMSGX_VERSION >> 8)  & 0xFF;
                    fprintf(stderr,"%s: XMM %d.%d.%d\n",arg0,vv,rr,mm);
                    return 0; }                /* exit from help okay */
                break;

/* ------------------------------------------------------------------ */
            case '-':                          /* long format options */

/*              if (abbrev("--version",argv[i],5) > 0)                */
                if (strcasecmp(argv[n],"--version") == 0)
                  { int vv, rr, mm;
                    vv =  XMITMSGX_VERSION >> 24;
                    rr = (XMITMSGX_VERSION >> 16) & 0xFF;
                    mm = (XMITMSGX_VERSION >> 8)  & 0xFF;
                    fprintf(stderr,"%s: XMM %d.%d.%d\n",arg0,vv,rr,mm);
                    return 0; } else           /* exit from help okay */

                /* "unknown option"; msgn = 3; msgv[1] = argv[n];     */
                  { xmopen("xmitmsgx",0,&mymsgstruct); msgv[1] = argv[n];
                    strcpy(mymsgstruct.pfxmaj,"XMM");
                    strcpy(mymsgstruct.pfxmin,"ERR");
                    rc = xmprint(3,2,msgv,0,&mymsgstruct);
                    if (rc < 0) fprintf(stderr,"%s: invalid option %s\n",arg0,argv[n]);
                    xmclose(&mymsgstruct); return 1; }
                break;
/* ------------------------------------------------------------------ */

            default: /* "unknown option"; msgn = 3; msgv[1] = argv[n] */
                xmopen("xmitmsgx",0,&mymsgstruct); msgv[1] = argv[n];
                strcpy(mymsgstruct.pfxmaj,"XMM");
                strcpy(mymsgstruct.pfxmin,"ERR");
                rc = xmprint(3,2,msgv,0,&mymsgstruct);
                if (rc < 0) fprintf(stderr,"%s: invalid option %s\n",arg0,argv[n]);
                xmclose(&mymsgstruct); return 1;
          }
      }

/*  msgn = atoi(argv[n++]);     ** wait, not even sure we have it yet */
    msgc = argc - n;

    /* Ensure that we have enough arguments.                          */
    if (msgc < 1)
      { xmopen("xmitmsgx",0,&mymsgstruct);
        strcpy(mymsgstruct.pfxmaj,"XMM");
        strcpy(mymsgstruct.pfxmin,"ERR");
        (void) xmprint(405,0,NULL,0,&mymsgstruct); /* missing msg num */
        (void) xmclose(&mymsgstruct); return 1; }     
    /* Ensure we have enough arguments. */
    if (argc < 2)
      { (void) xmprint(22,0,NULL,0,&mymsgstruct);
        (void) xmclose(&mymsgstruct);
        return 1; }

    /* Open the messages file, read it, get ready for service.        */
    rc = xmopen("errno",0,&mymsgstruct);
    if (rc != 0) { if (errno != 0) perror("xmopen()");
      return rc; }
    strcpy(mymsgstruct.pfxmaj,"XMM");
    strcpy(mymsgstruct.pfxmin,"ERR");

    /* Get message number and line-up any replacement tokens. */
    msgn = atoi(argv[n]);
    msgv[0] = "errno";
    i = n; j = i + 1;
    while (i < MSGMAX && j < argc) msgv[i++] = argv[j++];
    msgc = i;

    /* Print to stdout or stderr depending on level, optionally syslog. */
    rc = xmprint(msgn,msgc,(char**)msgv,0,&mymsgstruct);
    if (rc < 0)
      { (void) xmclose(&mymsgstruct);
        return rc; }

    /* Clear the message repository struct. */
    rc = xmclose(&mymsgstruct);
    if (rc != 0) return rc;

    return 0;
  }


