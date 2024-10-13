/*
 *
 *        Name: xmitmsg.c (C program source)
 *              "transmit message" for POSIX
 *              read and present enumerated messages from the repository
 *      Author: Rick Troth, Rice University, Information Systems
 *              Rick Troth, rogue programmer, La Casita en la Esquina
 *        Date: 1993-May-24
 *              2003-Sep-03
 *     Rewrite: 2023-03-15 (Wednesday)
 */

#include <stdlib.h>
#include <strings.h>

#include <stdio.h>
#include <errno.h>

#define MSGMAX 16

#include "xmitmsgx.h"
char *xmmprefix = PREFIX; /* installation prefix not application prefix */

/* development */
#include <stdio.h>

/* ------------------------------------------------------------------ */
int main(int argc,char*argv[])
  {
    int rc, msgn, msgc, i, j, n;
    unsigned char *applid, *caller, *letter, buffer[256], *msgv[MSGMAX];
    struct MSGSTRUCT ts, *ms;

    /* establish some defaults */
    msgv[0] = applid = "xmitmsgx";
    letter = caller = "";
    ms = &ts;

/*  perror("perror()");                                               */

    /* parse command options */
    for (n = 1; n < argc && argv[n][0] == '-'; n++)
      {
        if (strcasecmp(argv[n],"--applid") == 0)
          { if (++n < argc) applid = argv[n]; } else
        if (strcasecmp(argv[n],"--caller") == 0)
          { if (++n < argc) caller = argv[n]; } else
        if (strcasecmp(argv[n],"--letter") == 0)
          { if (++n < argc) letter = argv[n]; } else
        /* unknown option ** msgn = 3; msgv[1] = argv[n]             */
          { xmopen("xmitmsgx",0,NULL); msgv[1] = argv[n];
            (void) xmprint(3,2,msgv,0,NULL);
            (void) xmclose(NULL); return 1; }
      }

/*  msgn = atoi(argv[n++]);     ** wait, not even sure we have it yet */
    msgc = argc - n;

    /* Ensure that we have enough arguments.                          */
    if (msgc < 1)
      { xmopen("xmitmsgx",0,NULL);
        (void) xmprint(405,0,NULL,0,NULL);  /* missing message number */
        (void) xmclose(NULL); return 1; }

    /* Open the messages file, read it, get ready for service.        */
    rc = xmopen(applid,0,ms);
    /* But if that failed try to report *why* it failed.              */
    if (rc != 0)
      { perror("xmopen()");
        xmopen("xmitmsgx",0,NULL); msgv[1] = applid;
        (void) xmprint(813,2,msgv,0,NULL);
        (void) xmclose(NULL); return 1; }

    msgn = atoi(argv[n++]);
    msgc = argc - n;

    /* Get message number and queue-up any replacement tokens.        */
    msgv[0] = applid;
    i = 1; j = n;
    while (i < MSGMAX && j < argc) msgv[i++] = argv[j++];
    msgc = i;

    /* Call xmmake() and then printf() to force all to stdout here    */
    ms->msgnum = msgn;
    ms->msgc = msgc;               /* count of tokens from the caller */
    ms->msgv = msgv;                   /* token array from the caller */
    ms->msgbuf = buffer;    /* output buffer supplied by this routine */
    ms->msglen = sizeof(buffer) - 1;     /* size of the output buffer */
    ms->caller = caller;

    ms->msglevel = 0;             /* zero means set level from letter */
/*  ms->msgopts |= msgopts;                                           */

    rc = xmmake(ms);                              /* make the message */
    if (rc == 814)
      { xmopen("xmitmsgx",0,NULL);
        snprintf(buffer,sizeof(buffer),"%d",msgn); msgv[1] = buffer;
        (void) xmprint(814,2,msgv,0,NULL);
        (void) xmclose(NULL); return 1; }
/*  else perror("second");                                            */

if (rc == 0)
    rc = printf("%s\n",ms->msgbuf);

    /* Clear the message repository struct. */
    rc = xmclose(ms);
    if (rc != 0) return rc;

    return 0;
  }


/*

XMITMSG --applid applid [options] msgnumber [substitution [substitution [...]]]

--FORmat msgfmt
--LINE number

--LETter letter
--CALLER caller

--HEADer # This is the default.
--NOHEADer

--SYSLANG lang

--APPLID applid

      The message header consists of:

         xxxmmmnnns
         xxxmmmnnnns

      Where:

         xxx      specifies the application ID
         mmm      specifies the CALLER name
         nnn or nnnn  specifies the message number
         s        specifies the severity code

3. You can use XMITMSG from CMS to display a repository message on your
   screen; this is useful when you want to verify the content of a repository.

4. For more information on how to issue messages from assembler programs, see
   the APPLMSG macro in z/VM: CMS Macros and Functions Reference.

08750101E Attempt to divide by &1 is invalid
08750201E Attempt to &2 by &1 is invalid
08760101E Error &1.  rc = &3.
   | | ||
   | | ||________severity code
   | | |______line of message
   | |_____format of message
   |___number of message

Messages DMS813E and DMS814E can be displayed (depending on what you have
specified for the ERRMSG, DISPLAY/NODISPLAY, and VAR options) if XMITMSG
encounters an error when it attempts to retrieve the requested message.

*  DMS065E option option specified twice [RC=24]
*  DMS066E option1 and option2 are conflicting options [RC=24]
*  DMS080E Invalid numtype number [RC=24]
*  DMS109S Virtual storage capacity exceeded [RC=104]
*  DMS405E Invalid or missing message number [RC=24]
*  DMS408E Number of substitutions exceeds 20 [RC=24]
*  DMSnnnE Number of substitutions exceeds 9 [RC=24]
*  DMS631E XMITMSG must be invoked from an EXEC 2 or REXX exec or as a CMS command [RC=24]
*  DMS813E repos repository not found, message nnnn cannot be retrieved [RC=16]
*  DMS814E Message number nnnn, format nn, line nn, was not found; it was called from routine in application applid [RC=12]
*  DMS2045E Invalid substitution value - blank or parenthesis [RC=24]

 */

