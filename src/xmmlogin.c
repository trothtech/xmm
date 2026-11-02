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
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
 #include "xmmwin.h"
#endif

#include "xmitmsgx.h"
char *xmmprefix = PREFIX; /* installation prefix not application prefix */

/**********************************************************************/
int main(int argc,char*argv[])
  {
    char *msgv[4], *arg0;
    int msgc, rc, n;
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
                    strcpy(mymsgstruct.pfxmin,"LOG");
                    rc = xmprint(3,2,msgv,0,&mymsgstruct);
                    if (rc < 0) fprintf(stderr,"%s: invalid option %s\n",arg0,argv[n]);
                    xmclose(&mymsgstruct); return 1; }
                break;
/* ------------------------------------------------------------------ */

            default: /* "unknown option"; msgn = 3; msgv[1] = argv[n] */
                xmopen("xmitmsgx",0,&mymsgstruct); msgv[1] = argv[n];
                strcpy(mymsgstruct.pfxmaj,"XMM");
                strcpy(mymsgstruct.pfxmin,"LOG");
                rc = xmprint(3,2,msgv,0,&mymsgstruct);
                if (rc < 0) fprintf(stderr,"%s: invalid option %s\n",arg0,argv[n]);
                xmclose(&mymsgstruct); return 1;
          }
      }



    /* at this point we don't care about argc/argv so proceed         */


    msgc = 0;
    /* this element is reserved and not actually used at this time    */
    msgv[0] = "xmmlogin";
    /* "element number one" is where we stash the username            */
    msgv[1] = getlogin();
    if (msgv[1] != NULL) msgc = 2;

    /* now let's try and use the message formatter                    */
    rc = xmopen("xmitmsgx",MSGFLAG_SYSLOG,&mymsgstruct);
    if (rc != 0) return 1;

    /* there is presently no API function to do this - fix that!      */
    strcpy(mymsgstruct.pfxmaj,"XMM");    /* three-character up-cased applid/major */
    strcpy(mymsgstruct.pfxmin,"LOG");    /* three-character up-cased caller/minor */

    /* so far so good so now write the message to SYSLOG              */
    rc = xmprint(202,msgc,(char**)msgv,MSGFLAG_SYSLOG|MSGFLAG_NOPRINT,&mymsgstruct);

    /* clean-up in all cases whether xmprint() failed or not          */
    xmclose(&mymsgstruct);

    if (rc < 0) return 1;

    return 0;
  }


