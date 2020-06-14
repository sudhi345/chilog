/***************************************************************************************************************************
This code is written by Sudheendra H, Team Chimera.
***************************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef __CHILOG_H__
#define __CHILOG_H__

struct ChiLOG
{
	FILE *lif, *lf;
	int num;
	time_t bt,cps;
	char fileName[31];		 //name of the current log file
	char logInfo[31];             //log info file name (starts with '.' -> hidden)
	char extension[8];              //default log file extension

};

void setNames(struct ChiLOG *cl, char * lifn, char * lfn, char * extn);
void getFileName(struct ChiLOG *cl);
int getFileNum(char *tempName);
void clogInit(struct ChiLOG *cl);
void clog(struct ChiLOG *cl, char * type, char * str);


#endif
