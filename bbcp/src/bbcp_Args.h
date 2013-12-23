#ifndef __BBCP_ARGS_H__
#define __BBCP_ARGS_H__
/******************************************************************************/
/*                                                                            */
/*                           b b c p _ A r g s . h                            */
/*                                                                            */
/* (c) 2002 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*      All Rights Reserved. See bbcp_Version.C for complete License Terms    *//*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC03-76-SFO0515 with the Department of Energy              */
/******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "bbcp_Stream.h"

class bbcp_Opt;
  
class bbcp_Args
{
public:

char *getarg(int newln=0);

char  getopt();

void  Option(const char *optw, int minl, char optv, char opta); // For extended -- options

void  Options(char *opts, int fd, int moa=0);

void  Options(char *opts, int argc, char **argv);

      bbcp_Args(char *etxt=(char *)"Args: ");

    ~bbcp_Args();

char *argval; //value of current option

private:

char            *epfx;//bbcp: 
bbcp_Opt        *optp; //pointer to the the header of option link list
char            *vopts;//all the vaild cmd options
char            *curopt;//current position of cmd processing
int              MOA;
int              inStream;
int              endopts;//flag show if it is the end of the options need to process
int              Argc;
int              Aloc; //index inside a cmd line args (including file list)
char           **Argv;
bbcp_Stream      arg_stream;
};
#endif
