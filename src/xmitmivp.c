/*
 *
 *        Name: xmitmivp.c (C program source)
 *        Date: 2023-03-23 (Thu)
 *      Author: Rick Troth, rogue programmer
 *
 */

#include <stdio.h>

#include <xmitmsgx.h>

int main(int argc,char*argv[])
  {
    int rc;
    char buffer[8192]; char*vector[16];
    struct MSGSTRUCT ts, *ms;
    ms = &ts;

    /* this is taken from the header not actually from the library    */
    (void) printf("xmitmivp: XMITMSGX version %d.%d.%d\n",
      (XMITMSGX_VERSION>>24),(XMITMSGX_VERSION>>16)&0xFF,(XMITMSGX_VERSION>>8)&0xFF);

    rc = xmopen("xmitmsgx",0,ms);
    printf("xmopen() returned %d\n",rc);
    if (rc != 0) return 1;      /* if that did not work then bail out */

    ms->msgnum = 0;         /* must be non-zero when we actually used */
    ms->msgbuf = buffer;
    ms->msglen = sizeof(buffer);
    ms->msgv = (unsigned char**) vector;
    ms->msgc = 0;
    ms->msglevel = 0;   /* not used in this example */

    /*  W Warning messages issued                                     */
    ms->msgnum = 4;
    rc = xmmake(ms);
    printf("xmmake() returned %d for message number %d\n",rc,ms->msgnum);

    /*  E Error messages issued                                       */
    ms->msgnum = 8;
    rc = xmmake(ms);
    printf("xmmake() returned %d for message number %d\n",rc,ms->msgnum);

    /*  S Severe error messages issued                                */
    ms->msgnum = 12;
    rc = xmmake(ms);
    printf("xmmake() returned %d for message number %d\n",rc,ms->msgnum);

    /*  T Terminal error messages issued                              */
    ms->msgnum = 16;
    rc = xmmake(ms);
    printf("xmmake() returned %d for message number %d\n",rc,ms->msgnum);

    /*                                                                */
    ms->msgnum = 813; ms->msgbuf = buffer; ms->msglen = sizeof(buffer);
    ms->msgv = (unsigned char**) vector; vector[1] = "xxxxxxxx";
    ms->msgc = 2;
    rc = xmmake(ms);
    printf("xmmake() returned %d for message number %d; result follows\n",rc,ms->msgnum);
    printf("%s\n",buffer);

    /*                                                                */
    ms->msgnum = 814; ms->msgbuf = buffer; ms->msglen = sizeof(buffer);
    ms->msgv = (unsigned char**) vector; vector[1] = "1234";
    ms->msgc = 2;
    rc = xmmake(ms);
    printf("xmmake() returned %d for message number %d; result follows\n",rc,ms->msgnum);
    printf("%s\n",buffer);

    /*  E Invalid or missing message number                           */
    rc = xmprint(405,0,NULL,0,ms);
    printf("xmprint() returned %d for message number 405 (result above)\n",rc);

    /*  E Missing modifier keyword(s).                                */
    rc = xmwrite(1,384,0,NULL,0,ms);
    printf("xmwrite() returned %d (bytes) for message number 384 to FD 1 (result above)\n",rc);

    /*  E Missing operand(s).                                         */
    rc = xmwrite(2,386,0,NULL,0,ms);
    printf("xmwrite() returned %d (bytes) for message number 386 to FD 2 (result above)\n",rc);


    /*  E EAGAIN, try the operation again                             */
    rc = xmstring(buffer,sizeof(buffer),11,0,NULL,ms);
    printf("xmstring() returned %d (bytes) for message number 11; result follows\n",rc);
    printf("%s\n",buffer);

    /* a missing message, not in the repository                       */
    ms->msgnum = 9999;
    rc = xmmake(ms);
    printf("xmmake() returned %d for message number %d\n",rc,ms->msgnum);

    rc = xmclose(ms);
    printf("xmclose() returned %d\n",rc);

    return 0;
  }

/*
  20    E ENOTDIR, argument is not a directory
  21    E EISDIR, argument is a directory and should not be
  22    E EINVAL, invalid parameter or argument
 */


