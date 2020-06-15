/***************************************************************************************************************************
This code is written by Sudheendra H, Team Chimera. (c)2020, Team Chimera.
***************************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#ifndef __CHILOG_H__
#define __CHILOG_H__

struct ChiLOG
{
	FILE *lif, *lf;
	time_t bt,cps;
	int num;
	char fileName[32];		 //name of the current log file
	char logInfo[32];             //log info file name (starts with '.' -> hidden)
	char extension[8];              //default log file extension

};

void setNames(struct ChiLOG *cl, char * lifn, char * lfn, char * extn);
void getFileName(struct ChiLOG *cl);
int getFileNum(char *tempName);
void chilogInit(struct ChiLOG *cl);
void chilog(struct ChiLOG *cl, char * type, char * str);
void chilogCSVInit(struct ChiLOG *cl);
void chilogCSV(struct ChiLOG *cl, int dir, int rpm, int err, int bat_v, int mot_i, int mot_t, int ctl_t);

#endif
