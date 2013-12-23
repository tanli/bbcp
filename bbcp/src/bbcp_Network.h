#ifndef __BBCP_Network_H__
#define __BBCP_Network_H__
/******************************************************************************/
/*                                                                            */
/*                        b b c p _ N e t w o r k . h                         */
/*                                                                            */
/* (c) 2002 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*      All Rights Reserved. See bbcp_Version.C for complete License Terms    *//*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

#include <unistd.h>
#include <sys/socket.h>
#include "bbcp_Link.h"
#include "bbcp_Pthread.h"

// The bbcp_Network class defines a generic network where we can define common
// tcp/ip operations. This class is used by the global network object.
//
class bbcp_Network
{
public:

bbcp_Link   *Accept();

int          AutoTune() {return ATune;}

int          Bind(int minport, int maxport, int tries=1, int timeout=-1);

bbcp_Link   *Connect(char *host, int port, int retries=0, int rwait=1);

void         findPort(int &minport, int &maxport);

void         Flow(int isSRC);

char        *FullHostName(char *host=0, int asipaddr=0);

int          MaxWSize(int isSink);

int          MaxSSize() {return maxSegment;}

int          getWBSize(int xfd, int srwant);

int          QoS(int newQoS=-1);

static int   setPorts(int pnum1, int pnum2);

int          setWindow(int wsz, int noAT=0);

void         unBind() {if (iofd >= 0) {close(iofd); iofd = Portnum = -1;}}

             bbcp_Network();
            ~bbcp_Network() {unBind();}

private:

static int pFirst;
static int pLast;

int        accWait;
int        ATune;//flag to show if linux auto tune is on
int        iofd; //Socket fd
int        maxRcvBuff;//Max recv buffer size limit
int        maxSndBuff;//Max send buffer size limit
int        maxSegment;//Max seg size
int        netQoS;
int        Portnum;
int        protID;//protocol numnber of tcp in current system
int        Sender;// flag denotes if this is the sender side
int        Window;//TCP window size
int        WinSOP;//WinSOP = (isSrc ? SO_SNDBUF : SO_RCVBUF)

int   getHostAddr(char *hostname, struct sockaddr_in &InetAddr);
char *getHostName(struct sockaddr_in &addr);
int   Retry(int retries, int rwait);
void  setOpts(const char *who, int iofd);
int   setSegSz(const char *who, int iofd);
char *Peername(int snum);
};
#endif
