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

#define MSGMAX 16

#include "xmitmsgx.h"
char *xmmprefix = PREFIX; /* installation prefix not application prefix */

int main(int argc, char*argv[])
  {
    int rc, msgn, msgc, i, j;
    unsigned char *msgv[MSGMAX];

    /* Open the messages file, read it, get ready for service.        */
    rc = xmopen("errno",0,NULL);
    if (rc != 0) return rc;

    /* Ensure we have enough arguments. */
    if (argc < 2)
      { (void) xmprint(22,0,NULL,0,NULL);
        (void) xmclose(NULL);
        return 1; }

    /* Get message number and line-up any replacement tokens. */
    msgn = atoi(argv[1]);
    msgv[0] = "errno";
    i = 1; j = 2;
    while (i < MSGMAX && j < argc) msgv[i++] = argv[j++];
    msgc = i;

    /* Print to stdout or stderr depending on level, optionally syslog. */
    rc = xmprint(msgn,msgc,msgv,0,NULL);
    if (rc < 0)
      { (void) xmclose(NULL);
        return rc; }

    /* Clear the message repository struct. */
    rc = xmclose(NULL);
    if (rc != 0) return rc;

    return 0;
  }

