/*
 *
 *        Name: xmmrexx.c (C program source)
 *              Rexx (Regina) interface for XMITMSGX package
 *        Date: 2023-04-10 (Mon) Choirzilla Studio
 *    See also: xmmrexx.rx
 *
 * Enable with: Call Rxfuncadd 'xmitmsgx', 'xmmrexx', 'RxXmitmsgX'
 *                              funcname,   library,   entryname
 *   Call with: Parse Value xmitmsgx(subfunction,args,...) With rc rs
 *
 *  References: IBM publication SC24-6113
 *
 *  Build with: cc -fPIC -o xmmrexx.o -c xmmrexx.c
 *              cc -shared -o libxmmrexx.so xmmrexx.o
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "xmitmsgx.h"

static struct MSGSTRUCT *rxmsgstruct = NULL;

#define INCL_REXXSAA
#include <rexxsaa.h>

/* ------------------------------------------------------------- VERSION
 * Arguments: none
 * Returns: 0, always
 * Retstr: rc, name of package, version
 * The library version is an integer constant globally readable.
 */
int rxversion(ULONG rxargc, RXSTRING rxargv[],RXSTRING*rxrets)
  {
    int xmitmsgx_version = XMITMSGX_VERSION;

    /* split the version integer at bytes and reformat for display    */
    snprintf(rxrets->strptr,rxrets->strlength,"XMITMSGX version %d.%d.%d",
       (xmitmsgx_version>>24),
      ((xmitmsgx_version>>16)&0xFF),
      ((xmitmsgx_version>>8)&0xFF));
    /* set length of Rexx return string to follow C string length     */
    rxrets->strlength = strlen(rxrets->strptr);
    /* always return without error */
    return 0;
  }

/* ---------------------------------------------------------------- INIT
 * Arguments: file, opts (the latter is not presently implemented)
 * Returns: 0 and sets msgstruct pointer if no error, else rc from xmopen()
 * Retstr: rc, hex address of msgstruct or possible error string
 * Calls: xmopen()
 */
int rxinit(ULONG rxargc, RXSTRING rxargv[],RXSTRING*rxrets)
  {
    int rc, l;
    char file[256], opts[256];
    struct MSGSTRUCT *mymsgstruct, tmpstruct;

    l = rxargv->strlength;
    if (l >= sizeof(file)) l = sizeof(file) - 1;
    strncpy(file,rxargv->strptr,l); file[l] = 0x00;
    rxargv++;  rxargc--;   /* bump count and pointer to next argument */
    l = rxargv->strlength;
    if (l >= sizeof(opts)) l = sizeof(opts) - 1;
    strncpy(opts,rxargv->strptr,l); opts[l] = 0x00;
//printf("xmmrx: INIT '%s' '%s'\n",file,opts);

    mymsgstruct = rxmsgstruct;
    if (rxmsgstruct == NULL)
      { mymsgstruct = malloc(sizeof(tmpstruct));
        if (mymsgstruct == NULL) return /* errno */ -1; }

    /* call xmopen() to initialize the library */
    rc = xmopen(file,0,mymsgstruct);
    if (rc != 0) return rc;
//printf("xmmrx: INIT worked!\n");

    /* using pfxmaj and pfxmin is definitely outside the XMITMSGX API */
    strncpy(mymsgstruct->pfxmaj,"XMM",4);
    strncpy(mymsgstruct->pfxmin,"REX",4);
    /* this really needs to be changed to allow matching the caller */

    /* supply a return string */
    snprintf(rxrets->strptr,rxrets->strlength,"%x",mymsgstruct);
    rxrets->strlength = strlen(rxrets->strptr);

    rxmsgstruct = mymsgstruct;
    return 0;
  }

/* --------------------------------------------------------------- PRINT
 * Arguments: message number, replacement tokens
 * Returns: number of characters printed, negative indicates error
 * Retstr: rc, possible error string if error
 * Calls: xmprint()
 */
int rxprint(ULONG rxargc, RXSTRING rxargv[],RXSTRING*rxrets)
  {
    int rc, i, j, k, l, msgn, msgc;
    unsigned
    char buffer[4096], *msgv[32], *p;
    struct MSGSTRUCT *mymsgstruct;
//printf("xmmrx: PRINT\n");

    mymsgstruct = rxmsgstruct;

    l = rxargv->strlength;
    if (l >= sizeof(buffer)) l = sizeof(buffer) - 1;
    strncpy(buffer,rxargv->strptr,l); buffer[l] = 0x00;
    msgn = atoi(buffer);
    rxargv++;  rxargc--;   /* bump count and pointer to next argument */

    msgc = 0;
//printf("xmmrx: %d replacement tokens\n",rxargc);

    /* copy all Rexx arguments to local storage so we can null term   */
    k = 0;                            /* k and l for offset and limit */
    l = sizeof(buffer) - 1;                  /* limit of buffer space */
    msgc = 1;                       /* initial count of XMM arguments */

    /* for each Rexx argument, copy it to buffer, and append 0x00     */
    while (rxargc > 0)
      {
        p = rxargv->strptr;           /* p is maybe a handier pointer */
        i = 0;                        /* i and j for offset and limit */
        j = rxargv->strlength;               /* limit of input string */
        msgv[msgc++] = &buffer[k];  /* point to vector and incr count */
        while (i < j && k < l) buffer[k++] = p[i++];      /* bytewise */
        buffer[k++] = 0x00;    /* remember the null, that's the point */
        rxargv++;  rxargc--;    /* bump count and pointer to next arg */
      }

//printf("xmmrx: %d replacement tokens, string size %d\n",msgc,rxrets->strlength);
    rc = xmprint(msgn,msgc,msgv,0,mymsgstruct);

    rxrets->strptr[0] = 0x00; rxrets->strlength = 0;

    return rc;
  }

/* -------------------------------------------------------------- STRING
 * Arguments: message number, replacement tokens
 * Returns: number of bytes in string, negative indicates error
 * Retstr: rc, formatted message string or possible error string
 * Calls: xmstring()
 */
int rxstring(ULONG rxargc, RXSTRING rxargv[],RXSTRING*rxrets)
  {
    int rc, i, j, k, l, msgn, msgc;
    unsigned
    char buffer[4096], *msgv[32], *p;
    struct MSGSTRUCT *mymsgstruct;

    mymsgstruct = rxmsgstruct;

    l = rxargv->strlength;
    if (l >= sizeof(buffer)) l = sizeof(buffer) - 1;
    strncpy(buffer,rxargv->strptr,l); buffer[l] = 0x00;
    msgn = atoi(buffer);
    rxargv++;  rxargc--;   /* bump count and pointer to next argument */

    msgc = 0;

    /* copy all Rexx arguments to local storage so we can null term   */
    k = 0;                            /* k and l for offset and limit */
    l = sizeof(buffer) - 1;                  /* limit of buffer space */
    msgc = 1;                       /* initial count of XMM arguments */

    /* for each Rexx argument, copy it to buffer, and append 0x00     */
    while (rxargc > 0)
      {
        p = rxargv->strptr;           /* p is maybe a handier pointer */
        i = 0;                        /* i and j for offset and limit */
        j = rxargv->strlength;               /* limit of input string */
        msgv[msgc++] = &buffer[k];  /* point to vector and incr count */
        while (i < j && k < l) buffer[k++] = p[i++];      /* bytewise */
        buffer[k++] = 0x00;    /* remember the null, that's the point */
        rxargv++;  rxargc--;    /* bump count and pointer to next arg */
      }

    rc = xmstring(rxrets->strptr,rxrets->strlength,msgn,msgc,msgv,mymsgstruct);
    rxrets->strlength = strlen(rxrets->strptr);

    return rc;
  }

/* ---------------------------------------------------------------- QUIT
 * Arguments: hex address of msgstruct (not presently implemented)
 * Returns: 0 and clears msgstruct pointer if no error, else rc from xmclose()
 * Retstr: rc, hex address of msgstruct or possible error string
 * Calls: xmclose()
 */
int rxquit(ULONG rxargc, RXSTRING rxargv[],RXSTRING*rxrets)
  {
    int rc;
    struct MSGSTRUCT *mymsgstruct;
//printf("xmmrx: QUIT\n");

    if (rxmsgstruct == NULL) return 0;
    mymsgstruct = rxmsgstruct;

    rc = xmclose(mymsgstruct);
    if (rc != 0) return rc;

    free(mymsgstruct);

//  strncpy(rxrets->strptr,"I quit!",rxrets->strlength);
    rxrets->strptr[0] = 0x00; rxrets->strlength = 0;

    rxmsgstruct = NULL;
    return 0;
  }

/* ------------------------------------------------------------------ *
 * Regina Calling Convention:                                         *
 *      name == name by which this function was called (C string)     *
 *    rxargc == number of REXX arguments supplied on the call         *
 *      rxargv == array of arguments (of type RXSTRING)               *
 * queuename == name of the current queue (C string)                  *
 *    retstr == 256 return buffer (of type RXSTRING)                  *
 * Type RXSTRING:                                                     *
 *    ->strlength long int                                            *
 *    ->strptr char pointer                                           *
 * ------------------------------------------------------------------ */
APIRET APIENTRY
RxXmitmsgX(CONST CHAR *name,
           ULONG rxargc, RXSTRING rxargv[],
           CONST UCHAR *queuename, RXSTRING *retstr)
  {
    int rc, rl;
    char *rs, *sf;
    RXSTRING rxrets;
    char rsdata[4096];

    /* establish a return string for all internal subfunction calls   */
    rxrets.strptr = rsdata; rxrets.strlength = sizeof(rsdata) - 1;
//strcpy(rsdata,"Rexx Rocks!"); rxrets.strlength = 12;

    /* all subfunctions require at least one argument                 */
    if (rxargc < 1) return RXFUNC_BADTYPE; /* Incorrect call to routine */

    /* the first argument is the subfunction (sf)                     */
    if (rxargv->strlength < 1) return RXFUNC_BADTYPE;
    sf = rxargv->strptr;         /* we only care about the first byte */
    rxargv++;  rxargc--;   /* bump count and pointer to next argument */

    /* switch based on subfunction: I, P, S, Q, V                     */
    switch (*sf) {
      case 'I': case 'i': /* init */
        if (rxargc < 1) return RXFUNC_BADTYPE;
        if (rxargc > 2) return RXFUNC_BADTYPE;
        rxrets.strptr = rsdata; rxrets.strlength = sizeof(rsdata) - 1;
        rc = rxinit(rxargc,rxargv,&rxrets);
        if (rc > 0) rc = 0 - rc;       /* force errors to be negative */
        break;
      case 'P': case 'p': /* print */
        if (rxargc < 1) return RXFUNC_BADTYPE;
        rxrets.strptr = rsdata; rxrets.strlength = sizeof(rsdata) - 1;
        rc = rxprint(rxargc,rxargv,&rxrets);
        break;
      case 'S': case 's': /* string */
        if (rxargc < 1) return RXFUNC_BADTYPE;
        rxrets.strptr = rsdata; rxrets.strlength = sizeof(rsdata) - 1;
        rc = rxstring(rxargc,rxargv,&rxrets);
        break;
      case 'Q': case 'q': /* quit */
        rxrets.strptr = rsdata; rxrets.strlength = sizeof(rsdata) - 1;
        rc = rxquit(rxargc,rxargv,&rxrets);
        if (rc > 0) rc = 0 - rc;       /* force errors to be negative */
        break;
      case 'V': case 'v': /* quit */
        rxrets.strptr = rsdata; rxrets.strlength = sizeof(rsdata) - 1;
        rc = rxversion(rxargc,rxargv,&rxrets);
        if (rc > 0) rc = 0 - rc;       /* force errors to be negative */
        break;
      default: /* uh oh! */
        return RXFUNC_BADTYPE;           /* Incorrect call to routine */
              }

//  retstr->strptr[retstr->strlength] = 0x00;
    if (rc < 0) rc = 0 - rc;      /* force negative RC to be positive */
           else rc = 0;            /* but positive RC is not an error */
    sprintf(retstr->strptr,"%d %s",rc,rxrets.strptr);
//  snprintf(retstr->strptr,retstr->strlength,"%d %s",rc,rxrets.strptr);
    retstr->strlength = strlen(retstr->strptr);

    /* return the results */
    return RXFUNC_OK;
  }


