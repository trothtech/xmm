/*
 *
 *        Name: xmmdemoc.c (C program source)
 *              demonstrate the message handler called from C
 *        Date: 2023-07-23 (Sun)
 *    See also: xmmdemoc.sh, xmmrexx.rx, MessageDemo.java
 *
 */

#include <stdio.h>
#include <string.h>

#include <xmitmsgx.h>
char *xmmprefix = PREFIX;            /* tells XMM to share our prefix */

int main(int argc,char*argv[])
  {
    int rc;
    char buffer[8192]; char*vector[16];
    struct MSGSTRUCT ts, *ms;
    ms = &ts;

    /* announce ourselves */
    printf("Message Handler demonstration program in C\n");

    /* this is taken from the header not actually from the library    */
    printf("XMITMSGX version %d.%d.%d\n",
      (XMITMSGX_VERSION>>24),(XMITMSGX_VERSION>>16)&0xFF,(XMITMSGX_VERSION>>8)&0xFF);

    /* initialize the messages file of interest */
    rc = xmopen("xmitmsgx",0,ms);
    if (rc != 0) return 1;      /* if that did not work then bail out */

    /* using pfxmaj and pfxmin is definitely outside the XMITMSGX API */
    strncpy(ms->pfxmaj,"XMM",4);
    strncpy(ms->pfxmin,"ANY",4);         /* "any" because it's native */


    rc = xmprint(201,0,NULL,0,ms);

//            MessageService.print(119,msgs);
    /* message number 119 only specifies 9 tokens so #10 is ignored   */
    unsigned char *mv119[] = { "dummy",
          "#1", "#2", "#3", "#4", "#5", "#6", "#7", "#8", "#9", "#10" };
    rc = xmprint(119,11,mv119,0,ms);


    rc = xmstring(buffer,sizeof(buffer),119,11,mv119,ms);
    printf("%s (string)\n",buffer);


    rc = xmclose(ms);

    return 0;
  }


